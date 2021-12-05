/*
SparkFun Inventor’s Kit
Circuit 4A-HelloWorld

The LCD will display the words "Hello World" and show how many seconds have passed since
the RedBoard was last reset.

This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
This code is completely free for any use.

View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40
Download drawings and code at: https://github.com/sparkfun/SIK-Guide-Code
*/

#include <LiquidCrystal.h>          //the liquid crystal libarry contains commands for printing to the display

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);   // tell the RedBoard what pins are connected to the display

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


void setup() {                     
Serial.begin(9600);
  lcd.begin(16, 2);                 //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();                      //clear the display
  mySwitch.enableReceive(0);
}

void loop() {
if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      lcd.setCursor(0,0);
      
    } else {
      lcd.print("Received ");
      lcd.setCursor(0,1);
      lcd.print( mySwitch.getReceivedValue() );
      lcd.print(" / ");
      lcd.print( mySwitch.getReceivedBitlength() );
      lcd.println( mySwitch.getReceivedProtocol() );
    }
}
    mySwitch.resetAvailable();
  //lcd.setCursor(0,0);               //set the cursor to the 0,0 position (top left corner)
 // lcd.print("8888888888888888");       //print hello, world! starting at that position

 // lcd.setCursor(0,1);               //mvoe the cursor to the first space of the bottom row
 // lcd.print("8888888888888888");         //print the number of seconds that have passed since the last reset
}  
