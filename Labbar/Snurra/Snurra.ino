/* license:BSD-3-Clause
 * copyright:Joakim Larsson Edstrom
 *  
 * Snurra
 *
 * Det här exemplet snurrar roboten fram och tillbaka 
 */

#include <Wire.h>
#include <Zumo32U4.h> 

Zumo32U4Motors larver;
Zumo32U4ButtonA buttonA;

void setup()
{
  // Vänta på att knappen A ska tryckas
  while (!buttonA.getSingleDebouncedPress());
  delay(1000);
}

void loop()
{

  // Kör framåt
  larver.setLeftSpeed(100);
  larver.setRightSpeed(-100); // <--- MINUS

  delay(500);

  // Stanna
  larver.setLeftSpeed(0);
  larver.setRightSpeed(0);

  delay(500);

  // Kör bakåt
  larver.setLeftSpeed(-100); // <--- MINUS
  larver.setRightSpeed(100);

  delay(500);

  // Stanna
  larver.setLeftSpeed(0);
  larver.setRightSpeed(0);

  delay(500);
}
