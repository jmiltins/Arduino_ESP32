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

#define FF17 &FreeSans9pt7b
#define FF21 &FreeSansBold9pt7b
#define ROW1 0,16
#define ROW2 0,38
#define ROW3 0,60
#define ROW4 0,82
#define ROW5 0,104
#define ROW6 0,126

#include <SPI.h> // Communication with display?
#include <TFT_eSPI.h> // Display

#include <WiFi.h>


// Your WiFi credentials.
char ssid[] = "";
char pass[] = "";


#include <NTPClient.h>  //for time

//For NTP time
// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionally you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);



long ntpTime = 0;
unsigned long timeSyncMillis;
String ntpTimeStr = "";
String timeStrToPrint = "";

const unsigned long maxMillis = 4294967295; // when millis rolls over
int days = 0;
int hours = 0;                                                        
int minutes = 0;                                              
int seconds = 0;



// NeoPixel Values
#define PIXELPIN   5
#define NUMPIXELS  5
#define pixlux    20  //saturation level for NeoPixels colors
TFT_eSPI tft = TFT_eSPI(135, 240);

void setup() 
{
  tft.begin();
  tft.setRotation(1); // 1 LAndscape, 0 Portrait
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_SKYBLUE);//0x5E85
  tft.setTextSize(2);

  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    tft.print(".");
  }


// Time client
// Initialize a NTPClient to get time
  timeClient.begin(); 
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
timeClient.setTimeOffset(10800);   /*EDDITTTTTTTTTTTTTTTTTTTTTTTT                      */

timeClient.begin();
getTime();
ntpTimeStrToInts();
}

void loop() 
{
tft.fillScreen(TFT_BLACK); // Clear and set background color
tft.setCursor(5, 5);

//checkNtpsStringUpdate();

tft.println(ntpTimeStr);
tft.println(millis());
tft.println(timeSyncMillis);


currentTime();
printTime();
tft.print("Time ints:");
tft.print(hours);
tft.print(minutes);
tft.println(seconds);
delay(1000);



}
//// ftions
void checkNtpsStringUpdate()
{
  
  if (hours == timeClient.getHours() && minutes == timeClient.getMinutes()) 
    {
      tft.println("Karamba");
    }
 else //(ntpTimeStr == "01:00:10" || ntpTimeStr == "00:00:10") 
    {
      getTime();
      ntpTimeStrToInts();
    }
  }

int ntpTimeStrToInts()
// ftion splits ntps time string and assigns to global variables
{
   // split string to strings
   String hr = ntpTimeStr.substring(0, 2);   //captures first data String myString.substring(from, to)
 tft.print(hr);
   String mins = ntpTimeStr.substring(3, 5);   //captures second data String
 tft.print(mins); 
   String sec = ntpTimeStr.substring(6, 8);
 tft.println(sec);                                                          //Time we are converting. This can be passed from another function.
  
 // change strings to int and save to globals 
  hours = hr.toInt();                                                        //Number of seconds in an hour
  tft.print(hr);
  minutes = mins.toInt();                                              //Remove the number of hours and calculate the minutes.
  tft.print(mins);
  seconds = sec.toInt();                                            //Remove the number of hours and minutes, leaving only seconds.
  tft.println (sec);
}
int currentTime()
//takes ntpTime, adds current millis - timeSyncMillis, updates timeSyncMillis, updates 
// globals (hours, minutes and seconds) current time
{
  unsigned long deltaTime = (millis() - timeSyncMillis);
  timeSyncMillis = millis();
  float deltaSeconds = deltaTime/1000;
  timeSyncMillis -= deltaTime - round(deltaSeconds) * 1000; // to compensate those sec, what was lost during rounding
   seconds += round(deltaSeconds); // Use round to get more precise time, or looses ~3 min on 3hours
   while (seconds >59)
    {seconds -= 60;
    minutes+=1;}
  while (minutes >59)
    {minutes -= 60;
    hours+=1;}
  while (hours > 23)
    {hours -= 24;
    days +=1;}
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
  {sec = ("0" + String(seconds));}
  else {sec = (String(seconds));}

  //Adds 0 to minutes string< than 10
  if (minutes < 10)
  {mins = ("0" + String(minutes));}
  else {mins = (String(minutes));}

  //Adds 0 to hours string < than 10
  if (hours < 10)
  {hr = ("0" + String(hours));}
  else {hr = (String(hours));}

  
  timeStrToPrint = (hr + ":" + mins + ":" + sec);  //Converts to HH:MM:SS string. This can be returned to the calling function.
  tft.print("String Time:");
  tft.println (timeStrToPrint);
  }


int getTime() 
// Gets time from server and syncs ntpSyncMillis with millis()
{
  timeClient.update();
  ntpTimeStr = timeClient.getFormattedTime();
  timeSyncMillis = millis();
  }
