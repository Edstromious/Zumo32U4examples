/* license:BSD-3-Clause
   copyright:Joakim Larsson Edstrom

   SeLjuset

   Det här exemplet visar hur avståndssensoren fram kan se skillnad på höger och vänster
*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ProximitySensors avstandsSensorer;

void setup()
{
  avstandsSensorer.initThreeSensors(); /* Det finns 3 avstånsdsensorer */
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

  // Skriv ut på LCD display
  lcd.clear();
  lcd.print("FV:");
  lcd.print(avstandsSensorVarden[0]);
  lcd.gotoXY(0, 1);
  lcd.print("FH:");
  lcd.print(avstandsSensorVarden[1]);

  delay(100); // Vänta 1/10 sekund = 100mS
}



