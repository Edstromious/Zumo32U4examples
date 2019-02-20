/* license:BSD-3-Clause
     copyright:Joakim Larsson Edstrom
  
     SeVartFacit2
  
    Det här exemplet visar hur avståndssensorerna på sidorna används 
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
  while (!buttonA.getSingleDebouncedPress()); // Vänta på att knappen A ska tryckas
  delay(1000);
}

void loop()
{
  uint16_t avstandsSensorVarden[4];
  
  // Läs avståndssensorerna.
  avstandsSensorer.read();
  // Avståndssensorerna fram med ljus från vänster
  avstandsSensorVarden[0] = avstandsSensorer.countsFrontWithLeftLeds();
   // Avståndssensorn fram med ljus från höger
  avstandsSensorVarden[1] = avstandsSensorer.countsFrontWithRightLeds();
   // Avståndssensorn höger med ljus från höger
  avstandsSensorVarden[2] = avstandsSensorer.countsRightWithRightLeds();
   // Avståndssensorn fram med ljus från höger
  avstandsSensorVarden[3] = avstandsSensorer.countsLeftWithLeftLeds();

  // Om ljuset från vänster syn mer än ljuset till höger
  if (avstandsSensorVarden[0] - avstandsSensorVarden[1] > 2)
  {
    // Snurra höger
    larver.setLeftSpeed(200); 
    larver.setRightSpeed(-200);
  }
  // Om ljuset från höger syn mer än ljuset till vänster
  else if (avstandsSensorVarden[1] - avstandsSensorVarden[0] > 2)
  {
    // Snurra vänster
    larver.setLeftSpeed(-200); 
    larver.setRightSpeed(200);
  }
  // Om ljuset från höger studsar på något
  else if (avstandsSensorVarden[2] > 1)
  {
    // Snurra vänster
    larver.setLeftSpeed(150); 
    larver.setRightSpeed(-150);
    delay(150);    
  }
  // Om ljuset från vänster studsar på något
  else if (avstandsSensorVarden[3] > 1)
  {
    // Snurra höger
    larver.setLeftSpeed(-150); // Prova att öka farten, vad händer?
    larver.setRightSpeed(150);
    delay(150);    
  }
  else
  {
    // Stå still
    larver.setLeftSpeed(0); 
    larver.setRightSpeed(0);
  }

  delay(10); // Vänta 1/100 sekund = 10mS
}




