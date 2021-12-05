/*
  This is an Example for the FaBo LCD mini I2C Brick.

 LiquidCrystal Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 compatible library 2 Mar 2016
 by Hideki Yamauchi

 This example code is in the public domain.

 http://fabo.io/213.html
*/

// include the library code:
#include <Wire.h>
#include <FaBoLCDmini_AQM0802A.h>

// initialize the library
FaBoLCDmini_AQM0802A lcd;

int thisChar = 'a';

void setup() {
  // set up the LCD:
  lcd.begin();
  // turn on the cursor:
  lcd.cursor();
}

void loop() {
  // reverse directions at 'h':
  if (thisChar == 'h') {
    // go right for the next letter
    lcd.rightToLeft();
  }
  // reverse again at 'o':
  if (thisChar == 'o') {
    // go left for the next letter
    lcd.leftToRight();
  }
  // reset at 'v':
  if (thisChar > 'v') {
    delay(500);
    lcd.clear();
    // go to (0,0):
    lcd.home();
    // start again at 0
    thisChar = 'a';
  }
  // print the character
  lcd.write(thisChar);
  // wait a second:
  delay(500);
  // increment the letter:
  thisChar++;
}
