/* license:BSD-3-Clause
 * copyright:Joakim Larsson Edstrom
 *  
 * SeNedPip
 *
 * Det här exemplet visar hur linjesensorerna används
 */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4LineSensors linjeSensorer;
Zumo32U4Buzzer buzzer;

void setup()
{
  linjeSensorer.initFiveSensors(); /* Det finns 5 linjesensorer fram */
}

void loop()
{
  uint16_t linjeSensorVarden[5];
  
  // Läs linjesensorerna.
  linjeSensorer.read(linjeSensorVarden, QTR_EMITTERS_ON);


  // Pip lite
  if (linjeSensorVarden[0] < 500){ buzzer.playFrequency(440, 50, 15); }
  if (linjeSensorVarden[4] < 500){ buzzer.playFrequency(880, 50, 15); }

  // Skriv ut på LCD display
  lcd.clear();
  lcd.gotoXY(0, 0);
  lcd.print("V: ");
  lcd.print(linjeSensorVarden[0]); // Linjesensorn längst till vänster
  lcd.gotoXY(0, 1);
  lcd.print("H: ");
  lcd.print(linjeSensorVarden[4]); // Linjesensorn längst till höger 

  // Skriv ut seriellt på terminalen också
  Serial.print(linjeSensorVarden[0]);
  Serial.print(" ");
  Serial.print(linjeSensorVarden[4]);
  Serial.print("\n");
  
  delay(100); // Vänta 1/10 sekund = 100mS
}



