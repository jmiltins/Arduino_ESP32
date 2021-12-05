void setup() {
 Serial.begin(9600);
pinMode (ledPin,OUTPUT);
Serial.println("Input number 1-10");
}

void loop() {
  // put your main code here, to run repeatedly:

}
void loop()          
{
 int numMails = 0;
 delay(2000);
 if (Serial.available() > 0) 
 {
 switch (Serial.available()) {
   /* Serial.available() is the number of bytes waiting.  Convert from
    * ASCII val to an int.  Intentional switch-case fall through below. */
   case 3:
     numMails = Serial.read();
     numMails -= 48;
     numMails *= 10;
   case 2:
     numMails += Serial.read();
     numMails -= 48;
     numMails *= 10;
   case 1:
     numMails += Serial.read();
     numMails -= 48;
     break;
   default:
     /* If >3 chars, just clear out the incoming buffer. */
     while (Serial.available())
       Serial.read();
     return;
 }
     led.blink(400, numMails);
}
}
