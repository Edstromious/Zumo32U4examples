/* license:BSD-3-Clause
   copyright:Joakim Larsson Edstrom

   StudsaRuntTemplate

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
  larver.setLeftSpeed(200);
  larver.setRightSpeed(200);
}

void loop()
{
  uint16_t linjeSensorVarden[5];

  // Läs linjesensorerna
  //... kopiera kod från tidigare exempel

  // Använd en if-sats för att testa värdet på linjesensorn
  //... se labinstruktion för hur en if-sats ser ut
  {

    // Bromsa
    larver.setLeftSpeed(0);    // Stanna sedan!
    larver.setRightSpeed(0);
    delay(50);                 // Vänta lite på att motorn stannar

    // Snurra lite
    larver.setLeftSpeed(200);
    larver.setRightSpeed(-200);
    delay(500);                  // Vänta tills vi snurrat klart

    // Så sätter vi fart igen
    // Kör framåt
    larver.setLeftSpeed(200);
    larver.setRightSpeed(200);
  }
}

