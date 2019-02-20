/* Det här exemplet visar hur avståndssensorerna används  för att bestämma 
   vilket håll roboten ska vrida sig */
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
  
  avstandsSensorer.read();   // Läs avståndssensorerna.
  avstandsSensorVarden[FV] = avstandsSensorer.countsFrontWithLeftLeds();
  avstandsSensorVarden[FH] = avstandsSensorer.countsFrontWithRightLeds();

  // Om ljuset från höger syns mer än ljuset till vänster
  if (avstandsSensorVarden[FV] - avstandsSensorVarden[FH] > 2) 
  {
    larver.setLeftSpeed(200); // Snurra höger
    larver.setRightSpeed(-200);
  }
  // Om ljuset från höger syns mer än ljuset till vänster
  else if (avstandsSensorVarden[FH] - avstandsSensorVarden[FV] > 2)
  {
    larver.setLeftSpeed(-200); // Snurra vänster
    larver.setRightSpeed(200);
  }
  else if (avstandsSensorVarden[FV] > 4)
  {
    larver.setLeftSpeed(200);   // Knuffa!
    larver.setRightSpeed(200);
    delay(1000);
  }
  else
  {
    // Stå still!
    larver.setLeftSpeed(0); 
    larver.setRightSpeed(0);
  }
  delay(10); // Vänta 1/100 sekund = 10mS
}




