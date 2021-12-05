/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how to send values to the Blynk App,
  when there is a widget, attached to the Virtual Pin and it
  is set to some frequency

  Project setup in the app:
    Value Display widget attached to V5. Set any reading
    frequency (i.e. 1 second)
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

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
  
  }
int pinValue;  
BLYNK_WRITE(V0)
{
  pinValue = param.asInt();
  servo.write(param.asInt());
  }


void setup()
{
  // Debug console
  pinMode(13,OUTPUT);
  pinMode(A4,INPUT);
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

timer.setInterval(1000L,SendSensorData);
servo.attach(21);
}

void loop()
{
  Blynk.run();
  timer.run();
  Serial.print("Sensor data ");
 Serial.println(analogRead(A4)); 
  if(pinValue)
    {digitalWrite(13,HIGH);}
  else 
    {digitalWrite(13,LOW);}
 }
