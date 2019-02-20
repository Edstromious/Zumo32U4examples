/* license:BSD-3-Clause
 * copyright:Joakim Larsson Edstrom
 *  
 * Trycka
 *
 * Det här exemplet visar hur man använder knapparna på Zumo32U4
 */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4Buzzer buzzer;

void setup()
{
}

void loop()
{

  if (buttonA.isPressed())
  {
    // Spela en 220 Hz ton i 200mS
    buzzer.playFrequency(220, 25, 15);
  }
  if (buttonB.isPressed())
  {
    // Spela en 440 Hz ton i 200mS
    buzzer.playFrequency(440, 25, 15);
  }
  if (buttonC.isPressed())
  {
    // Spela en 880 Hz ton i 200mS
    buzzer.playFrequency(880, 25, 15);
  }
}
