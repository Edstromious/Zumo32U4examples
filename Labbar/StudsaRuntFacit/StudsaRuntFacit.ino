/* license:BSD-3-Clause
   copyright:Joakim Larsson Edstrom

   StudsaRuntFacit

   Exempel på hur man kan lösa uppgiften StudsaRunt
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

  // Kör framåt - testa med andra värden än 200, byt alla 200 mot 400 i hela programmet!
  larver.setLeftSpeed(200);
  larver.setRightSpeed(200);
}

void loop()
{
  uint16_t linjeSensorVarden[5];

  delay(10); // Läs inte sensorerna för ofta så vänta 1/100 sek

  // Läs linjesensorerna
  linjeSensorer.read(linjeSensorVarden, QTR_EMITTERS_ON);

  // Använd en if-sats för att testa värdet på vänster linjesensor
  if (linjeSensorVarden[0] < 500) { // Testen antar svart underlag och vit linje

  // Testa både höger och vänster linjesensor
  // Byt ut if-satsen ovan mot följande rad om roboten kör ut från banan av sig självt ofta
  // if (linjeSensorVarden[0] < 500 || linjeSensorVarden[4] < 500) { 

    // Bromsa
    
    //    larver.setLeftSpeed(-200); // Back full fart - Dessa rader behövs om roboten kör ut ibland!
    //    larver.setRightSpeed(-200);
    //    delay(200);                // Men bara en kort stund, 2/10 sekund

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

