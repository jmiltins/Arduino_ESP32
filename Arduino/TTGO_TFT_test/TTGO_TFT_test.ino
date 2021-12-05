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


// NeoPixel Values
#define PIXELPIN   5
#define NUMPIXELS  5
#define pixlux    20  //saturation level for NeoPixels colors
TFT_eSPI tft = TFT_eSPI(135, 240);

void setup() {
  tft.begin();
  tft.setRotation(1); // 1 LAndscape, 0 Portrait
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_SKYBLUE);//0x5E85
  tft.setTextSize(2);


}

void loop() 
{
tft.fillScreen(TFT_BLACK); // Clear and set background color

tft.fillRect(50, 60, 120, 22, TFT_VIOLET); // (from right, from top, lenght, height(from top), color)
delay(1000);
tft.setCursor(50, 60); // x - from right, y - from top
tft.setTextColor(TFT_SKYBLUE,TFT_GREEN);//0x5E85
tft.println("KASPARS");

tft.println("WiFi connected.");
delay(1000);

tft.setCursor(50, 60,1); // x - from right, y - from top, fpnt size(not secessery)
tft.setTextColor(TFT_RED);//0x5E85

tft.print("KASPARS");

delay(1000);



}
