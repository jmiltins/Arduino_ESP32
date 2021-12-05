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

void setup() {
  // set up the LCD:
  lcd.begin();
}

void loop() {
  lcd.clear();
  lcd.home();

  // Print a message to the LCD.
  lcd.print("hello, world!");
  delay(1000);

  // scroll 13 positions (string length) to the left
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // scroll 20 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }

  // scroll 20 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // delay at the end of the full loop:
  delay(1000);

}

