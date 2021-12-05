/*
  SimpleMQTTClient.ino
  The purpose of this exemple is to illustrate a simple handling of MQTT and Wifi connection.
  Once it connects successfully to a Wifi network and a MQTT broker, it subscribe to a topic and send a message to it.
  It will also send a message delayed 5 seconds later.
*/

#include "EspMQTTClient.h"
#
EspMQTTClient client(
  "Cisco",
  "Test123123",
  "192.168.3.127",  // MQTT Broker server ip
  
  /*"",   // Can be omitted if not needed
  "",   // Can be omitted if not needed
  */
 // "miot.ditf.rtu.lv",  // MQTT Broker server ip
  //"viaa_iot_kurss",   // Can be omitted if not needed
 // "",   // Can be omitted if not needed
  "janisM",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);
const byte phPin=34;
void setup()
{
  Serial.begin(115200);
pinMode (phPin, INPUT);
  // Optionnal functionnalities of EspMQTTClient : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  /*// Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("mytopic/test", [](const String & payload) {
    Serial.println(payload);
  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
    Serial.println(topic + ": " + payload);
  });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/test", "This is a message sent 5 seconds later");
  });*/
}
int phSensor;
void loop()
{
  client.loop();

phSensor=analogRead(phPin);
client.publish("janisM/phSensor",String(phSensor));
Serial.print("Gaisma ir ");
Serial.println(phSensor);
delay(500);
  
}
