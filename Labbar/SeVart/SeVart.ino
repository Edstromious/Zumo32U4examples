/* license:BSD-3-Clause
   copyright:Joakim Larsson Edstrom

   Blinka

   Det här exemplet visar hur avståndssensorerna används
   för att bestämma vilket håll roboten ska vrida sig
*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors larver;
Zumo32U4ProximitySensors avstandsSensorer;
Zumo32U4ButtonA buttonA;

void setup()
{
  avstandsSensorer.initThreeSensors();          // Det finns 3 avstånsdsensorer
  while (!buttonA.getSingleDebouncedPress());   // Vänta på att knappen A ska tryckas
  delay(1000);                                  // Vänta en sekund till
}

void loop()
{
  uint16_t avstandsSensorVarden[4];

  // Läs avståndssensorerna.
  avstandsSensorer.read();
  // Avståndssensorn fram med ljus från vänster
  avstandsSensorVarden[0] = avstandsSensorer.countsFrontWithLeftLeds();
  // Avståndssensorn fram med ljus från höger
  avstandsSensorVarden[1] = avstandsSensorer.countsFrontWithRightLeds();

  if (avstandsSensorVarden[0] < avstandsSensorVarden[1])
  {
    // Snurra höger
    larver.setLeftSpeed(200);
    larver.setRightSpeed(-200);
  }
  else
  {
    // Snurra vänster
    larver.setLeftSpeed(-200);
    larver.setRightSpeed(200);
  }

  delay(100); // Vänta 1/10 sekund = 100mS
}




