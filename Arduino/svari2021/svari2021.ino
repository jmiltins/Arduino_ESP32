// https://makersportal.com/blog/2019/5/12/arduino-weighing-scale-with-load-cell-and-hx711

#include <Q2HX711.h> // Scale Library
#include <EEPROM.h> // To save in EEPROM

#include <SPI.h> // Communication with display?
#include <TFT_eSPI.h> // Display
#include "DHT.h" // Temperature sensor
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
//#include <WiFiClient.h>

//For NTP time
#include <NTPClient.h>  //for time
WiFiUDP ntpUDP;
// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

long ntpTime = 0;
unsigned long timeSyncMillis = 0;
unsigned long oneMinuteMillis = 0;
boolean feederTimerEnable = true;

String ntpTimeStr = "";
String timeStrToPrint = "";

const unsigned long maxMillis = 4294967295; // when millis rolls over
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

// You can specify the time server pool and the offset, (in seconds)
// additionally you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// Blynk data start

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME "Guntara Feed"
char auth[] = "";
BlynkTimer timer;

int feedButtonState = 1;
int treatButtonState = 1;
int waterButtonState = 1;
int resetState = 1;

int foodDose = 0;

long feedTimer1 = 0;
int timer1Hours = 0;
int timer1Minutes = 0;

long feedTimer2 = 0;
int timer2Hours = 0;
int timer2Minutes = 0;

long feedTimer3 = 0;
int timer3Hours = 0;
int timer3Minutes = 0;

//Blynk data end

// Your WiFi credentials.
char ssid[] = "";
char pass[] = "";

// Weight data start
const byte hx711_data_pin = 12; // define weight data and clk pins
const byte hx711_clock_pin = 13;
float yy1 = 302.0; // calibrated mass to be added
long x1 = 0L;
long x0 = 0L;
float avg_size = 1.0; // amount of averages for each mass measurement
Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711

int mass = 0;
int weightStopMass = 200;

// Weight data End

// TFT data start
TFT_eSPI tft = TFT_eSPI(135, 240);

// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */
// TFE data end

//DHT 11 data start
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float temp = 0; // global
// DHT 11 data end


//Ultrasonic sensor start
int trigPin = 37;    // Trigger
int echoPin = 38;    // Echo
long duration, cm;
int remaining = 0;
// Ultrasonic sensor end

// EEPROM start
# define EEPROM_SIZE 16
// EEPROM end

// Buttons
#define BUTTON_1        0
#define BUTTON_2        35
// Relays

#define RELAY_1 32 //Feed
#define RELAY_2 33 //Treat
#define RELAY_3 25 //Water
// Blynk start
void SendSensorData()
{
  Blynk.virtualWrite(V1, mass);//sending
  Blynk.virtualWrite(V2, remaining);//sending
  Blynk.virtualWrite(V3, temp);//sending
  Blynk.virtualWrite(V5, millis() / 1000 / 3600, 1000);
  Blynk.virtualWrite(V4, timeStrToPrint); //sending time string
}

BLYNK_CONNECTED() // To sync with the server
{
  Blynk.syncVirtual(V10); // feed button
  Blynk.syncVirtual(V11); // Treat button
  Blynk.syncVirtual(V12); // Watter button
  Blynk.syncVirtual(V14); // resset
  Blynk.syncVirtual(V15); // feed timer 1
  Blynk.syncVirtual(V16); // feed timer 2
  Blynk.syncVirtual(V17); // feed timer 3
}

/// blynk receive and write param to ints or str
BLYNK_WRITE(V10) // doose
{
  foodDose = param.asInt();
}
BLYNK_WRITE(V11) // feed button
{
  feedButtonState = param.asInt();
}
BLYNK_WRITE(V12) // treat
{
  treatButtonState = param.asInt();
}
BLYNK_WRITE(V13) // Water button
{
  waterButtonState = param.asInt();
}
BLYNK_WRITE(V14) // resetState button
{
  resetState = param.asInt();
}
BLYNK_WRITE(V15) // feed at timer1
{
  feedTimer1 = param.asInt();
}
BLYNK_WRITE(V16) // feed at timer2
{
  feedTimer2 = param.asInt();
}
BLYNK_WRITE(V17) // feed at timer3
{
  feedTimer3 = param.asInt();
}
// Blynk END



/////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  //Define inputs and outputs

  /// Buttons
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //  remaining = distance();

  //Relays
  pinMode(RELAY_1, OUTPUT);
  digitalWrite(RELAY_1, LOW);
  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_2, LOW);
  pinMode(RELAY_3, OUTPUT);
  digitalWrite(RELAY_3, LOW);

  // TFT
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_SKYBLUE);
  tft.setTextSize(2);

  /// Welcome screen
  tft.setCursor(5, 5);
  tft.println("FeedAndTrain");
  tft.print("Starting");
  //tft.println(WiFi.localIP()); // Print ip address, Not working

  // DHT 11
  dht.begin(); // Temperature sensor

  //WiFi begin
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) //Print "...", while WiFi connecting
  {
    delay(300);
    tft.print(".");
  }

  // Blynk begin
  //Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
  Blynk.connect();
  timer.setInterval(1000L, SendSensorData);

  Serial.begin(115200); // prepare serial port

  EEPROM.begin(EEPROM_SIZE); // prepare EEPROM
  // EEPROM read x1 and yy1
  x0 = EEPROM.readLong(0);
  x1 = EEPROM.readLong(8);

  // Time client
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(10800);
  timeClient.begin();
  getTime(); // Update time
  ntpTimeStrToInts(); // Save time to global int hours, minutes, seconds

  ////////////////////////////////////////////////////////////////////////////////////////////////////
}

/// f-tions
int ntpTimeStrToInts()
// ftion splits ntps time string and assigns to global variables
{
  // split string to strings
  String hr = ntpTimeStr.substring(0, 2);   //captures first data String myString.substring(from, to)

  String mins = ntpTimeStr.substring(3, 5);   //captures second data String

  String sec = ntpTimeStr.substring(6, 8);
  //Time we are converting. This can be passed from another function.

  // change strings to int and save to globals
  hours = hr.toInt();                                                        //Number of seconds in an hour

  minutes = mins.toInt();                                              //Remove the number of hours and calculate the minutes.

  seconds = sec.toInt();                                            //Remove the number of hours and minutes, leaving only seconds.

}
int currentTime()
//takes ntpTime, adds current millis - timeSyncMillis, updates timeSyncMillis, updates
// globals (hours, minutes and seconds) current time
{
  unsigned long deltaTime = (millis() - timeSyncMillis);
  timeSyncMillis = millis();
  float deltaSeconds = deltaTime / 1000;
  timeSyncMillis -= deltaTime - round(deltaSeconds) * 1000; // to compensate those sec, what was lost during rounding
  seconds += round(deltaSeconds); // Use round to get more precise time, or looses ~3 min on 3hours
  while (seconds > 59)
  { seconds -= 60;
    minutes += 1;
  }
  while (minutes > 59)
  { minutes -= 60;
    hours += 1;
  }
  while (hours > 23)
  { hours -= 24;
    days += 1;
  }
}
void printTime()
// Takes globals (hours, minutes and seconds) of current time, changes to Strings,
// adds 0 to value less than 10 and Prints to tft
{
  String hr = "";
  String mins = "";
  String sec = "";
  //Adds 0 to seconds string< than 10
  if (seconds < 10)
  {
    sec = ("0" + String(seconds));
  }
  else {
    sec = (String(seconds));
  }

  //Adds 0 to minutes string< than 10
  if (minutes < 10)
  {
    mins = ("0" + String(minutes));
  }
  else {
    mins = (String(minutes));
  }

  //Adds 0 to hours string < than 10
  if (hours < 10)
  {
    hr = ("0" + String(hours));
  }
  else {
    hr = (String(hours));
  }


  timeStrToPrint = (hr + ":" + mins + ":" + sec);  //Converts to HH:MM:SS string. This can be returned to the calling function.
  tft.print("Time:");
  tft.println (timeStrToPrint);
}

int getTime()
// Gets time from server and syncs ntpSyncMillis with millis()
{
  timeClient.update();
  ntpTimeStr = timeClient.getFormattedTime();
  timeSyncMillis = millis();
}

int validateData(int dataToCheck, int minFrom, int maxTo)
// this f-tion checks data to be within max and min values, or returns 0
{
  if (dataToCheck > maxTo)// ||  (dataToCheck > maxTo)
  {
    return 0;
  }
  else
  {
    return dataToCheck;
  }
}

int fillBowl()
{
  remaining = 5;//distance();
  long startTime = millis(); // Records the start time
  int tempWeightStopMass = 0; // Correction for food overdose

  while (measure() < (foodDose - tempWeightStopMass))//||
  {
    tempWeightStopMass = weightStopMass;
    if (remaining > 0 ) // Remaining check in tank
    {
      digitalWrite(RELAY_1, HIGH); //open feeder gate
    }
    else
    {
      break;
    }
    if ((millis() - startTime ) > 5000) // STOP RELAY if five seconds
    {
      break;
    }
  }
  digitalWrite(RELAY_1, LOW); //sTOPS FEEDING
}
void reconnectBlynk()
{
  //Blynk.begin(auth, ssid, pass);
  Blynk.connect();
  timer.setInterval(1000L, SendSensorData);
}
int chechBlynkConnection()
{
  if ( ! Blynk.connected() )
  {
    //    delay ( 500 ); // For stabillity and display
    //    tft.fillScreen(TFT_BLACK);
    //    tft.setCursor(5, 5);
    //    tft.println("FEED AND TRAIN");
    tft.print("No Blynk connection");
    //    tft.println(".");
    reconnectBlynk();
  }
}

int chechWifiConnection()
{
  if ( WiFi.status() != WL_CONNECTED )
  {
    //    delay ( 500 );
    //    tft.fillScreen(TFT_BLACK);
    //    tft.setCursor(5, 5);
    //    //tft.println("FEED AND TRAIN");
    tft.print("WiFi: Disconnected");
    //    tft.println(".");
    return false;
  }
  else {
    return true;
  }
}

int distance()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.

  pinMode(echoPin, INPUT);
  //pin: the number of the Arduino pin on which you want to read the pulse. Allowed data types: int.
  //value: type of pulse to read: either HIGH or LOW. Allowed data types: int.
  //timeout (optional): the number of microseconds to wait for the pulse to start; default is one second. Allowed data types: unsigned long.
  duration = pulseIn(echoPin, HIGH, 3500); // 3500 ms to wait for the feedback

  // Convert the time into a distance
  cm = (duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
  return cm;
}
int measure()
{
  // averaging reading
  long reading = 0;
  for (int jj = 0; jj < int(avg_size); jj++) {
    reading += hx711.read();
  }
  reading /= long(avg_size);

  // calculating mass based on calibration and linear fit
  float ratio_1 = (float) (reading - x0);
  float ratio_2 = (float) (x1 - x0);
  float ratio = ratio_1 / ratio_2;
  float mass = yy1 * ratio;

  Serial.print("Raw: ");
  Serial.print(reading);
  Serial.print(", ");
  Serial.println(mass);
  return mass;
}

void tare()
{
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5, 60);
  tft.println("Calibration. Remove all from weights");
  delay(1000); // allow load cell and hx711 to settle
  x1 = 0L;
  x0 = 0L;

  for (int ii = 0; ii < int(avg_size); ii++)
  {
    delay(10);
    x0 += hx711.read();
  }
  x0 /= long(avg_size);
  //Save in EEPROM
  EEPROM.writeLong(0, x0);
  //EEPROM.commit();

  Serial.println("Add Calibrated Mass");
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5, 60);
  tft.println("Add Calibrated Mass");
  // calibration procedure (mass should be added equal to yy1)
  int ii = 1;
  while (true)
  {
    if (hx711.read() < x0 + 10000) {
    } else {
      ii++;
      delay(2000);
      for (int jj = 0; jj < int(avg_size); jj++) {
        x1 += hx711.read();
      }
      x1 /= long(avg_size);
      //Save in EEPROM

      EEPROM.writeLong(8, x1);
      EEPROM.commit();
      break;
    }
  }
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5, 60);
  tft.println("Calibration Complete");
  Serial.println("Calibration Complete");
}

int dht11()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  return t;
}
int atLeastSecond()
{
  if ((millis() - timeSyncMillis) > 1000)
  {
    return true;
  }
  else
  {
    return false;
  }
}
//////////// feeder timer f-tions
/*
  int setFeedTimer1ints()
  // ftion splits ntps time string and assigns to global variables
  {
   // split string to strings
   String hr = feedTimer1.substring(0, 2);   //captures first data String myString.substring(from, to)
   String mins = feedTimer1.substring(3, 5);   //captures second data String

  // change strings to int
  timer1Hours = hr.toInt();                                                        //Number of seconds in an hour
  timer1Minutes = mins.toInt();                                              //Remove the number of hours and calculate the minutes.

  }
*/
int compareTimerAndTime(int timerHours, int timerMinutes)
//compares timer hours and minutes and returns true or false
{
  if ((timerHours == hours && timerMinutes == minutes) && (timerHours != 0 && timerMinutes != 0))
  {
    return true; //fillBowl();
  }
  else
  {
    return false;
  }
}

int oneMinuteCounter()
// deactivate feederTimerEnable(false) feeding for one minute
{
  if (millis() - oneMinuteMillis > 60001)
  {
    feederTimerEnable = true;
  }
  else
  {
    feederTimerEnable = false;
  }
}
void timerFeedExe(int timer)
// if feeredTimer is true and timeHours and minutes mach, execute feeding
{
  int timerHours = timer / 3600;                                                        //Number of seconds in an hour
  int timerMinutes = (timer - timerHours * 3600) / 60 ;

  if (feederTimerEnable && (timerHours, timerMinutes))
  {
    feederTimerEnable = false;
    oneMinuteMillis = millis();
    fillBowl();
  }
}
void resetESP()
{
  // resset
  if (resetState == 0) // Fill bowl immedialtey from blynk button
  {


    tft.fillScreen(TFT_BLACK);
    tft.setCursor(5, 60);
    tft.println(resetState);
    tft.println("reseting......");

    resetState = 1;
    Blynk.virtualWrite(V14, 1);
    ESP.restart();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////
void loop()
{

  unsigned long loopLenght = millis() - loopLenght ;
  oneMinuteCounter(); // 1 min timer for feeder timer, to not allow repreating bowlfill untill one minute has passed

  mass = measure();
  temp = dht11();

  //remaining = distance();

  // blynk
  Blynk.run();
  timer.run();



  if (atLeastSecond()) // if at least one second has passed
  {
    currentTime(); //Calculate time
    // Display info
    tft.fillScreen(TFT_BLACK); // Clear and set background color
    tft.setCursor(5, 5);
    tft.println("FeedAndTrain");
    printTime(); //Print time to tft
    tft.print(mass);
    tft.println(" g");
    tft.print(remaining);
    tft.println(" %");
    tft.print(temp);
    tft.println(" C");
    tft.print("Food doose ");
    tft.print(foodDose);
    tft.println(" g");
    tft.print("Tmr1 ");
    tft.print(feederTimerEnable);
    tft.print(" ");
    tft.print(timer1Hours);
    tft.print(" ");
    tft.print(timer1Minutes);
  }

  chechWifiConnection();
  chechBlynkConnection();

  //tare procedure, when button 1 is pressed
  if (digitalRead(BUTTON_1) == LOW)
  {
    tare();
  }
  // Push button 2, to start relay and feed mannualy
  if (digitalRead(BUTTON_2) == LOW)
  {
    fillBowl();
  }
  //Blynk Button
  if (feedButtonState == 0) // Fill bowl immedialtey from blynk button
  {
    feedButtonState = 1;
    Blynk.virtualWrite(V11, 1); // Turn the widget attached to V1 Off
    fillBowl();
  }
  // if resetState = 0, reset!
  resetESP();

  // blynk timer  feeding

  //setFeedTimer1ints();
  //calculateFeedTimer1Ints();

  timerFeedExe(feedTimer1);

  timerFeedExe(feedTimer2);

  timerFeedExe(feedTimer3);

}
