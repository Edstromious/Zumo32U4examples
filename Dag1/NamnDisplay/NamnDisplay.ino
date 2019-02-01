/* Det här exemplet visar hur man skriver på displayen */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD fisk;

void setup()
{

}

void loop()
{
  // Sudda allt på displayen
  fisk.clear();

  // Skriv ut din robots namn
  fisk.print("Fisken");

  // Man kan skriva nästa rad också
  fisk.gotoXY(0, 1);

  // Ett nummer t.ex.
  fisk.print(1234567);

  delay(1000);
}

