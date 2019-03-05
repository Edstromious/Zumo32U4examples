/* license:BSD-3-Clause
     copyright:Joakim Larsson Edstrom

     Knuffa6

     Det här exemplet visar hur roboten knuffar på motståndaren utan att trilla av planen
     Ny feature: Slump
*/
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
  NORM = 175,
  MAX = 300
};


int dataIn = 13;
int load = 14;
int clock = 1;

int maxInUse = 1;    //change this variable to set how many MAX7219's you'll use

int e = 0;           // just a variable

// define max7219 registers
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;


void putByte(byte data) {
  byte i = 8;
  byte mask;
  while (i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask) {           // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    } else {
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

void maxSingle( byte reg, byte col) {
  //maxSingle is the "easy"  function to use for a single max7219

  digitalWrite(load, LOW);       // begin
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(load, LOW);       // and load da stuff
  digitalWrite(load, HIGH);
}

void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin
  for ( c = 1; c <= maxInUse; c++) {
    putByte(reg);  // specify register
    putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  }
  digitalWrite(load, LOW);
  digitalWrite(load, HIGH);
}

void maxOne(byte maxNr, byte reg, byte col) {
  //maxOne is for addressing different MAX7219's,
  //while having a couple of them cascaded

  int c = 0;
  digitalWrite(load, LOW);  // begin

  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data

  for ( c = maxNr - 1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(load, LOW); // and load da stuff
  digitalWrite(load, HIGH);
}

const byte kryss[] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};
const byte skalle[] = {0x54, 0x54, 0x54, 0x82, 0xaa, 0x82, 0x44, 0x38};
const byte vanster[] = {0x54, 0x54, 0x54, 0x82, 0x95, 0x82, 0x44, 0x38};
const byte hoger[] = {0x54, 0x54, 0x54, 0x82, 0x52, 0x82, 0x44, 0x38};

void setup ()
{
  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);

  digitalWrite(clock, HIGH);
  //initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
  for (e = 1; e <= 8; e++) { // empty registers, turn all LEDs off
    maxAll(e, 0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
  // range: 0x00 to 0x0f
#if 0
  maxSingle(1, 0x81);                      //  + - - - - - - -
  maxSingle(2, 0x42);                      //  - + - - - - - -
  maxSingle(3, 0x24);                      //  - - + - - - - -
  maxSingle(4, 0x18);                      //  - - - + - - - -
  maxSingle(5, 0x18);                     //  - - - - + - - -
  maxSingle(6, 0x24);                     //  - - - - - + - -
  maxSingle(7, 0x42);                     //  - - - - - - + -
  maxSingle(8, 0x81);                    //  - - - - - - - +
#endif
  for (int i = 0; i < 8; i++)
    maxSingle(i + 1, skalle[i]);                      //  + - - - - - - -

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
    for (int i = 0; i < 8; i++)
      maxSingle(i + 1, skalle[i]);                      //  + - - - - - - -
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
    for (int i = 0; i < 8; i++)
      maxSingle(i + 1, hoger[i]);                      //  + - - - - - - -
    // Snurra vänster
    larver.setLeftSpeed(NORM);
    larver.setRightSpeed(-NORM);
    delay(500 * (200 / NORM));    // Vi vill snurra en stund så inte målet hamnar i döda vinkeln!
  }
  else if (avstandsSensorVarden[V] > 1)   // Om ljuset från vänster studsar på något
  {
    for (int i = 0; i < 8; i++)
      maxSingle(i + 1, vanster[i]);                      //  + - - - - - - -
    // Snurra höger
    larver.setLeftSpeed(-NORM);
    larver.setRightSpeed(NORM);
    delay(500 / (200 / NORM));    // Vi vill snurra en stund så inte målet hamnar i döda vinkeln!
  }
  else
  {
    // Leta efter motståndaren
    larver.setLeftSpeed(NORM/2);
    larver.setRightSpeed(-NORM);
    delay(100);
  }
  delay(10); // Vänta 1/100 sekund = 10mS
}
