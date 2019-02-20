/* Det här exemplet visar hur man startar en Sumo match och stannar efteråt */
#include <Wire.h>
#include <Zumo32U4.h>
#include "RemoteConstants.h"
#include "RemoteDecoder.h"

const uint16_t messageTimeoutMs = 115;
bool messageActive = false;
uint16_t lastMessageTimeMs = 0;
RemoteDecoder decoder;
Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;

Zumo32U4Motors larver;
Zumo32U4ProximitySensors avstandsSensorer;
Zumo32U4LineSensors linjeSensorer;
Zumo32U4ButtonA buttonA;

// Lite namn på platser i avstandsSensorVarden gör det lättare att förstå programmet
enum {
  FV = 0, // Främre avståndssensor med ljus från vänster sida
  FH = 1, // Främre avståndssensor med ljus från höger sida
  H  = 2, // Höger avståndssensor med ljus från höger sida
  V  = 3  // Vänster avståndssensor med ljus från vänster sida
};

// Lite namn på platser i linjeSensorVarden gör det lättare att förstå programmet
enum {
  LV = 0, // Linjesensorn längst till vänster
  LH = 4  // Linjesensorn längst till höger
};

// Lite namn på hastigheter, ändra inom 100-400 området
enum {
  MIN = 100,
  NORM = 150,
  MAX = 200
};

void setup()
{
  decoder.init();
  lcd.clear();
  lcd.print(F("Waiting"));

  avstandsSensorer.initThreeSensors();          // Det finns 3 avstånsdsensorer
  linjeSensorer.initFiveSensors();
  while (!buttonA.getSingleDebouncedPress()); // Vänta på att knappen A ska tryckas
  delay(1000);
}

void loop()
{
  uint16_t avstandsSensorVarden[4];
  uint16_t linjeSensorVarden[5];

  decoder.service();
  ledYellow(messageActive);
  ledRed(decoder.criticalTime());
  if (!decoder.criticalTime())
  {
    processRemoteEvents();

    avstandsSensorer.read();   // Läs avståndssensorerna.
    avstandsSensorVarden[FV] = avstandsSensorer.countsFrontWithLeftLeds();
    avstandsSensorVarden[FH] = avstandsSensorer.countsFrontWithRightLeds();

    // Först kollar vi linjen så vi inte trillar av planen
    linjeSensorer.read(linjeSensorVarden, QTR_EMITTERS_ON); // Läs linjesensorerna
    if (linjeSensorVarden[LH] < 500 || linjeSensorVarden[LV] < 500)
    {
      // Snabbstopp så vi inte ramlar över linjen
      larver.setLeftSpeed(-MAX); // Back full fart
      larver.setRightSpeed(-MAX);
      delay(300 * (400 / MAX));  // Men bara en kort stund
      if (random(0, 100) > 50)
        larver.setLeftSpeed(MAX);  // Snurra höger!
      else
        larver.setRightSpeed(MAX);  // Snurra vänster!
      delay(random(100, 500) * (400 / MAX));  // en kort stund
    }
    else if (avstandsSensorVarden[FV] - avstandsSensorVarden[FH] > 2)
    { // Snurra höger                   // Prova att byta 2 mot 1 eller 0, vad händer?
      larver.setLeftSpeed(MAX); // Prova att höja farten från 200 till 400, vad händer?
      larver.setRightSpeed(-MAX);
    }
    // Om ljuset från höger syns mer än ljuset till vänster
    else if (avstandsSensorVarden[FH] - avstandsSensorVarden[FV] > 2)
    {
      // Snurra vänster
      larver.setLeftSpeed(-MAX);
      larver.setRightSpeed(MAX);
    }
    //
    // Om något finns rakt framför knuffar vi!
    //
    else if ( avstandsSensorVarden[FV] > 2 && avstandsSensorVarden[FH] > 2)
    {
      larver.setLeftSpeed(MAX); // Full fart!
      larver.setRightSpeed(MAX);

      // Knuffa!
      do
      {
        delay(10);
        linjeSensorer.read(linjeSensorVarden, QTR_EMITTERS_ON);  // Läs linjesensorerna
      } while ( linjeSensorVarden[LH] > 500 && linjeSensorVarden[LV] > 500); // &&   // Vi attackerar tills den vita linjen syns
    }
    //
    // Annars kollar vi om något finns på sidorna
    //
    else if (avstandsSensorVarden[H] > 1)   // Om ljuset från höger studsar på något
    {
      // Snurra vänster
      larver.setLeftSpeed(NORM);
      larver.setRightSpeed(-NORM);
      delay(500 * (200 / NORM));    // Vi vill snurra en stund så inte målet hamnar i döda vinkeln!
    }
    else if (avstandsSensorVarden[V] > 1)   // Om ljuset från vänster studsar på något
    {
      // Snurra höger
      larver.setLeftSpeed(-NORM);
      larver.setRightSpeed(NORM);
      delay(500 / (200 / NORM));    // Vi vill snurra en stund så inte målet hamnar i döda vinkeln!
    }
    else
    {
      // Leta efter motståndaren
      larver.setLeftSpeed(MIN);
      larver.setRightSpeed(-MIN);
      delay(100);
    }
    // Check how long ago the current message was last verified.
    // If it is longer than the timeout time, then the message has
    // expired and we should stop executing it.
    if (messageActive && (uint16_t)(millis() - lastMessageTimeMs) > messageTimeoutMs)
    {
      messageActive = false;
      stopCurrentCommand();
    }
    delay(10); // Vänta 1/100 sekund = 10mS
  }
}

void processRemoteEvents()
{
  if (decoder.getAndResetMessageFlag())
  {
    // The remote decoder received a new message, so record what
    // time it was received and process it.
    lastMessageTimeMs = millis();
    messageActive = true;
    processRemoteMessage(decoder.getMessage());
  }

  if (decoder.getAndResetRepeatFlag())
  {
    // The remote decoder receiver a "repeat" command, which is
    // sent about every 109 ms while the button is being held
    // down.  It contains no data.  We record what time the
    // repeat command was received so we can know that the
    // current message is still active.
    lastMessageTimeMs = millis();
  }
}

void processRemoteMessage(const uint8_t * message)
{
  // Print the raw message on the first line of the LCD, in hex.
  // The first two bytes are usually an address, and the third
  // byte is usually a command.  The last byte is supposed to be
  // the bitwise inverse of the third byte, and if that is the
  // case, then we don't print it.
  lcd.clear();
  char buffer[9];
  if (message[2] + message[3] == 0xFF)
  {
    sprintf(buffer, "%02X%02X %02X ",
            message[0], message[1], message[2]);
  }
  else
  {
    sprintf(buffer, "%02X%02X%02X%02X",
            message[0], message[1], message[2], message[3]);
  }
  lcd.print(buffer);

  // Go to the next line of the LCD.
  lcd.gotoXY(0, 1);

  // Make sure the address matches what we expect.
  if (message[0] != remoteAddressByte0 ||
      message[1] != remoteAddressByte1)
  {
    lcd.print(F("bad addr"));
    return;
  }

  // Make sure that the last byte is the logical inverse of the
  // command byte.
  if (message[2] + message[3] != 0xFF)
  {
    lcd.print(F("bad cmd"));
    return;
  }

  stopCurrentCommand();
  processRemoteCommand(message[2]);
}

// Start running the new command.
void processRemoteCommand(uint8_t command)
{
  switch (command)
  {
    case remoteUp:
      lcd.print(F("up"));
      //motors.setSpeeds(400, 400);
      break;

    case remoteDown:
      lcd.print(F("down"));
      //motors.setSpeeds(-400, -400);
      break;

    case remoteLeft:
      lcd.print(F("left"));
      //motors.setSpeeds(-250, 250);
      break;

    case remoteRight:
      lcd.print(F("right"));
      //motors.setSpeeds(250, -250);
      break;

    case remoteStopMode:
      lcd.print(F("stop"));
      break;

    case remoteEnterSave:
      lcd.print(F("enter"));
      break;

    case remoteVolMinus:
      lcd.print(F("vol-"));
      break;

    case remoteVolPlus:
      lcd.print(F("vol+"));
      break;

    case remotePlayPause:
      lcd.print(F("play"));
      break;

    case remoteSetup:
      lcd.print(F("setup"));
      break;

    case remoteBack:
      lcd.print(F("back"));
      break;

    case remote0:
      lcd.print(F("0"));
      break;

    case remote1:
      lcd.print(F("1"));
      buzzer.playNote(NOTE_C(4), 200, 15);
      break;

    case remote2:
      lcd.print(F("2"));
      buzzer.playNote(NOTE_D(4), 200, 15);
      break;

    case remote3:
      lcd.print(F("3"));
      buzzer.playNote(NOTE_E(4), 200, 15);
      break;

    case remote4:
      lcd.print(F("4"));
      break;

    case remote5:
      lcd.print(F("5"));
      break;

    case remote6:
      lcd.print(F("6"));
      break;

    case remote7:
      lcd.print(F("7"));
      break;

    case remote8:
      lcd.print(F("8"));
      break;

    case remote9:
      lcd.print(F("9"));
      break;
  }
}

// Stops the current remote control command.  This is called when
// a new command is received or if the current command has
// expired.
void stopCurrentCommand()
{
  //motors.setSpeeds(0, 0);
  buzzer.stopPlaying();
}
