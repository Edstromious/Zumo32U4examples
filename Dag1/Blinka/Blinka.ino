/* Det här exemplet visar hur man blinkar med lysdioderna på Zumo 32U4. */

#include <Wire.h>
#include <Zumo32U4.h>

void setup()
{

}

void loop()
{
  // Tänd lysdioderna
  ledRed(1);
  ledYellow(1);
  ledGreen(1);

  // Vänta en sekund
  delay(1000);

  // Släck lysdioderna
  ledRed(0);
  ledYellow(0);
  ledGreen(0);

  // Vänta en sekund
  delay(1000);
}
