/*  Det här exemplet kör roboten fram ca 50 cm och tilllbaka */

#include <Wire.h>
#include <Zumo32U4.h> 

Zumo32U4Motors larver;

void setup()
{
}

void loop()
{
  int snigel;
  
  // Kör framåt - öka farten från 0 till 400
  for (snigel = 0; snigel <= 400; snigel++)
  {
    larver.setLeftSpeed(snigel);
    larver.setRightSpeed(snigel);
    delay(1);
  }
  // Kör framåt - minska farten från 400 till 0
  for (snigel = 400; snigel >= 0; snigel--)
  {
    larver.setLeftSpeed(snigel);
    larver.setRightSpeed(snigel);
    delay(1);
  }

  // Kör bakåt - öka farten bakåt från 0 till -400 (OBS minus!!)
  for (snigel = 0; snigel >= -400; snigel--)
  {
    larver.setLeftSpeed(snigel);
    larver.setRightSpeed(snigel);
    delay(1);
  }
  // Kör bakåt - minska farten bakåt från -400 till 0 
  for (snigel = -400; snigel <= 0; snigel++)
  {
    larver.setLeftSpeed(snigel);
    larver.setRightSpeed(snigel);
    delay(1);
  }
  delay(500);
}
