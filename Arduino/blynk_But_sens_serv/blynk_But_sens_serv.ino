
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

//Globals
const int trigPin = 27;
const int echoPin = 33;
long duration;
int distance;
int pinValue; 
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ri3i36XEV9p7g3bvq5fh-jx0Z1dft3u4";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Cisco";
char pass[] = "Test123123";

BlynkTimer timer;
Servo servo;

//int pinValue;
// Use Virtual pin 5 for uptime display
#define PIN_UPTIME V5

// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)
BLYNK_READ(PIN_UPTIME)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_UPTIME, millis() / 1000);
}

void SendSensorData()
{
  int sensorData = analogRead(A4);//reading
  Blynk.virtualWrite(V1, sensorData);//sending
  Blynk.virtualWrite(V5, millis(),1000);
  Blynk.virtualWrite(V3, distance);//sending
  }
void SendDistanceData()
{
   //HC-SR04
  digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance

  int distance= duration*0.034/2;//reading
  Blynk.virtualWrite(V5, millis(),1000);
  Blynk.virtualWrite(V2, distance);//sending
 Serial.print("distance ");
 Serial.println(distance); 
  
  }
BLYNK_WRITE(V0)
{
  pinValue = param.asInt();
  servo.write(param.asInt());
  }
BLYNK_WRITE(V3)
{
  servo.write(param.asInt());
  }

void setup()
{
  // Debug console
  pinMode(32,OUTPUT);
  pinMode(A4,INPUT);
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

timer.setInterval(1000L,SendSensorData);
timer.setInterval(1000L,SendDistanceData);
// SERVO
servo.attach(21,500,2400);

//HC-SR04
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop()
{
  Blynk.run();
  timer.run();
  Serial.print("Sensor data ");
 Serial.println(analogRead(A4)); 
  if(pinValue)
    {digitalWrite(32,HIGH);}
  else 
    {digitalWrite(32,LOW);}

 }
