/* Det här exemplet visar hur roboten knuffar på motståndaren utan att trilla av planen */
#include <Wire.h>
#include <Zumo32U4.h>

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
  avstandsSensorer.initThreeSensors();          // Det finns 3 avstånsdsensorer
  linjeSensorer.initFiveSensors();
  while (!buttonA.getSingleDebouncedPress()); // Vänta på att knappen A ska tryckas
  delay(1000);
}

void loop()
{
  uint16_t avstandsSensorVarden[4];
  uint16_t linjeSensorVarden[5];

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
    larver.setLeftSpeed(MAX);  // Snurra sedan!
    delay(200 * (400 / MAX));  // en kort stund
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
  delay(10); // Vänta 1/100 sekund = 10mS
}


