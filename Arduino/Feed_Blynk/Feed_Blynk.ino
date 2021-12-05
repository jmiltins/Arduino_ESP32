
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "sFaO5Jv5HNjP7E7a6XQj6xD09LsIK--B";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TP_LINK";
char pass[] = "dodinternetu";


#define Feeder 32    // Output pins - feeder relejs
int pinValue = 0;
int foodDose = 0;
int feedTimes = 0;                                    // Cik x barots
int feedReset = 0;
#define Feeder 2    // Output pins - feeder relejs

BLYNK_WRITE(V0)                                         // Feeder button uz virtuālā PIN0. Izmantojas, lai definētu izsniegtās barības daudzumu - laiks, cik ilgi novirina.
{
  foodDose = param.asInt();                             // assigning incoming value from pin V to a variable
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  Serial.println(pinValue);
  Serial.println(foodDose);
  //lcd.print(0,0,pinValue);
}
