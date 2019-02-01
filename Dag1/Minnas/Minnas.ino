/*  Det här exemplet visar hur man lagrar data 
 *  i minnet och sedan plockar fram det och 
 *  skriver det på displayen. */
#include <EEPROM.h>
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD fisk;


// Körs en gång vid start av Zumo
void setup() { 
  // Lagra namnet i minnet 
  EEPROM.put(0, "Bettan"); 
}

// Körs gång på gång efter start av Zumo
void loop() {
  // Buffer för att hämta namnet från EEPROM
  // Måste vara längre än namnet 
  // 10 lika med max 9 bokstäver i namnet.
  char ZumosNamn[10];
  
  // Hämta namnet från EEPROM
  EEPROM.get(0, ZumosNamn);

  // Sudda displayen fisk
  fisk.clear();

  // Skriv Zumos namn på display
  fisk.print(ZumosNamn);

  delay(1000);
}


