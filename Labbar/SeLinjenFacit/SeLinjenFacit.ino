/* license:BSD-3-Clause
   copyright:Joakim Larsson Edstrom

   SeLinjenFacit

   Exempel på hur man kan lösa uppgiften SeLinjen

*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors larver;
Zumo32U4LineSensors linjeSensorer;
Zumo32U4ButtonA buttonA;

void setup()
{
  linjeSensorer.initFiveSensors(); // Starta sensorerna

  while (!buttonA.getSingleDebouncedPress()); // Vänta på att knappen A ska tryckas
  delay(1000);

  // Kör framåt
  larver.setLeftSpeed(100);
  larver.setRightSpeed(100);
}

void loop()
{
  uint16_t linjeSensorVarden[5];

  delay(10); // Läs inte sensorerna för ofta så vänta 1/100 sek

  // Läs linjesensorerna
  linjeSensorer.read(linjeSensorVarden, QTR_EMITTERS_ON);

  // Använd en if-sats för att testa värdet på den vänstra linjesensorn
  if ( linjeSensorVarden[0] < 500 )
  { // Testen antar svart underlag och vit linje
    larver.setLeftSpeed(0);    // Stanna!
    larver.setRightSpeed(0);
  }
}


