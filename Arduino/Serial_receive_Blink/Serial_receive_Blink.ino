
int incomingByte = 0; // for incoming serial data
byte ledPin=11;

void setup() 
{
Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
pinMode (ledPin,OUTPUT);
Serial.println("Input number 1-10");
}

void loop() 
{
    // reply only when you receive data:
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();
    }

if  (incomingByte==49||incomingByte==13) 
  {
  Serial.print("I received: ");
    incomingByte=10;
    Serial.println(incomingByte);
    } 
 
   else if (incomingByte<10&&incomingByte>1)
    {incomingByte=incomingByte-48;}
   
   Serial.parseInt();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  
 
  // do something different depending on the range value:
 if  (incomingByte>10||incomingByte<0)
 {
  Serial.println("Out of range for LED");
  incomingByte=0;
  }
 for (int i=0;i<incomingByte;i++)
 {
  digitalWrite (ledPin,HIGH);
  delay(100);
  digitalWrite (ledPin,LOW);
  delay(100);
  }
 
 /* long brightness = map(incomingByte, 0, 3, 0, 1023);
  
 switch (incomingByte) 
  {
    case 0:    // your hand is on the sensor
      Serial.println("dark");
      break;
    case 1:    // your hand is close to the sensor
      Serial.println("dim");
      break;
    case 2:    // your hand is a few inches from the sensor
      Serial.println("medium");
      break;
    case 3:    // your hand is nowhere near the sensor
      Serial.println("bright");
      break;
 }
 Serial.println(brightness);
  analogWrite (ledPin,brightness)
 */ 
  ;
  delay(500);        // delay in between reads for stability



}
