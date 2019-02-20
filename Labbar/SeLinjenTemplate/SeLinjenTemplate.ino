/* license:BSD-3-Clause
   copyright:Joakim Larsson Edstrom

   SeLinjenTemplate

   Modifiera det här programmet att stanna vid den tejpade linjen
   Byt ut raderna med prickar '...' mot er egen kod

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
  //... kopiera kod från tidigare exempel

  // Använd en if-sats för att testa värdet på linjesensorerna
  //... se labinstruktion för hur en if-sats ser ut
  {
    larver.setLeftSpeed(0);    // Stanna!
    larver.setRightSpeed(0);
  }
}

