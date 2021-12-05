/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
    Downloads, docs, tutorials: http://www.blynk.cc
/*************************************************************/
//#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE DHT11   // DHT 11

char auth[] = "sFaO5Jv5HNjP7E7a6XQj6xD09LsIK--B";

// Your WiFi credentials.
char ssid[] = "TP_LINK";
char pass[] = "dodinternetu";

int foodDose = 0;
int feedTimes = 0;                                    // Cik x barots
int feedReset = 0;
#define Feeder 2    // Output pins - feeder relejs

DHT dht(DHTPIN, DHTTYPE);

BLYNK_WRITE(V0)                                         // Feeder button uz virtuālā PIN0. Izmantojas, lai definētu izsniegtās barības daudzumu - laiks, cik ilgi novirina.
{
  foodDose = param.asInt();                             // assigning incoming value from pin V to a variable
}

BLYNK_WRITE(V3)                                         // if =1, reset feedTimes
{
  feedReset = param.asInt();
  if (feedReset = 1){
    feedTimes = 0;                             // reset counter
    Blynk.virtualWrite(V1,feedTimes);
  }
}
int dht() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.print(f);
  Serial.print(F("F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("C "));
  Serial.print(hif);
  Serial.println(F("F"));

  return t;
}
void setup()
{
  Serial.begin(115200);                                // Debug console
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(Feeder, OUTPUT);
  digitalWrite(Feeder,HIGH);                             
}

void loop()
{
  Blynk.run();
  dht();
  Blynk.virtualWrite(V1,feedTimes);                                   // v1 value display, cik x barots
      }
}
