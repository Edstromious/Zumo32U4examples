/* license:BSD-3-Clause
     copyright:Joakim Larsson Edstrom

     SeVart

    Det här exemplet visar hur avståndssensorerna används
    för att bestämma vilket håll roboten ska vrida sig
*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors larver;
Zumo32U4ProximitySensors avstandsSensorer;
Zumo32U4ButtonA buttonA;

// Lite namn på platser i avstandsSensorVarden gör det lättare att förstå programmet
enum {
  FV = 0, // Främre avståndssensor med ljus från vänster sida
  FH = 1, // Främre avståndssensor med ljus från höger sida
};

void setup()
{
  avstandsSensorer.initThreeSensors();          // Det finns 3 avstånsdsensorer
  while (!buttonA.getSingleDebouncedPress()); // Vänta på att knappen A ska tryckas
  delay(1000);
}

void loop()
{
  uint16_t avstandsSensorVarden[4];

  // Läs avståndssensorerna.
  avstandsSensorer.read();

  // Avståndssensorn fram med ljus från vänster
  avstandsSensorVarden[FV] = avstandsSensorer.countsFrontWithLeftLeds();

  // Avståndssensorn fram med ljus från höger
  avstandsSensorVarden[FH] = avstandsSensorer.countsFrontWithRightLeds();

  // Kolla skillnaden mellan höger och vänster sensorvärde.
  // Att skillnaden ska vara mer än 2 kallas hysteres och gör roboten mindre nervös

  // Om ljuset från vänster syns mer än ljuset till höger
  if (avstandsSensorVarden[FV] - avstandsSensorVarden[FH] > 2) // Prova att byta 2 mot 1 eller 0, vad händer?
  {
    // Snurra höger
    larver.setLeftSpeed(200); // Prova att höja farten från 200 till 400, vad händer?
    larver.setRightSpeed(-200);
  }
  // Om ljuset från höger syns mer än ljuset till vänster
  else if (avstandsSensorVarden[FH] - avstandsSensorVarden[FV] > 2)
  {
    // Snurra vänster
    larver.setLeftSpeed(-200);
    larver.setRightSpeed(200);
  }
  else
  {
    // Stå still
    larver.setLeftSpeed(0);
    larver.setRightSpeed(0);
  }
  delay(10); // Vänta 1/100 sekund = 10mS
}




