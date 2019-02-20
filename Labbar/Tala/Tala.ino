/* license:BSD-3-Clause
 * copyright:Joakim Larsson Edstrom
 *  
 * Tala
 *
 * Det här exemplet visar hur man spelar enkla toner på Zumo 32U4
 */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Buzzer buzzer;

void setup()
{
}

void loop()
{
  // Spela en 440 Hz ton i 200mS
  buzzer.playFrequency(440, 200, 7);

  // Vänta en sekund (1000mS)
  delay(1000);

  // Spela tonen A i fjärde oktaven i 200mS
  buzzer.playNote(NOTE_A(4), 200, 7);  

  // Vänta en sekund igen (1000mS)
  delay(1000); 
}
