#include <avr/wdt.h>
byte led=13;
uint8_t loopCount=0;
uint8_t setupCount=0;

void setup(){
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  Serial.print("Setup started : ");
  Serial.println(setupCount++);
  // make a delay before enable WDT 
  // this delay help to complete all initial tasks
  delay(2000);
  wdt_enable(WDTO_1S);
  
}

void loop(){
  byte state;
  Serial.println("LOOP started ! ");
  for(int i=0; i<=5; i++){
    digitalWrite(13,state);
    state=!state;
    Serial.print("Loop : ");
    Serial.print(i);
    Serial.println();
    delay(1000);
    wdt_reset();
  }
  //infinity loop to hang MCU
  while(1)
  {
        digitalWrite(13,1);
    delay(50);
    digitalWrite(13,0);
    delay(50);
    Serial.print("Loop infinnity ");
    Serial.println(loopCount++);
  }
}
