/* license:BSD-3-Clause
   copyright:Joakim Larsson Edstrom

   SeLjuset

   Det här exemplet visar hur avståndssensorerna används
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
  uint16_t avstandsSensorVarden[3];

  // Läs linjesensorerna.
  avstandsSensorer.read();
  avstandsSensorVarden[0] = avstandsSensorer.countsLeftWithLeftLeds();
  avstandsSensorVarden[1] = avstandsSensorer.countsFrontWithLeftLeds();
  avstandsSensorVarden[2] = avstandsSensorer.countsRightWithRightLeds();

  // Skriv ut på LCD display
  lcd.clear();
  lcd.print("L:");
  lcd.print(avstandsSensorVarden[0]); // Avståndssensorn längst till vänster
  lcd.print(" R:");
  lcd.print(avstandsSensorVarden[2]); // Avståndssensorn i mitten
  lcd.gotoXY(0, 1);
  lcd.print("F:");
  lcd.print(avstandsSensorVarden[1]); // Avståndssensorn längst till höger

  delay(100); // Vänta 1/10 sekund = 100mS
}



