#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//#define I2C_SDA 33
//#define I2C_SCL 32


#define OLED_RESET     -1 // Reset pin 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//Globals

float temp;
uint16_t light;
byte ledPin=32;
byte servoPin=21;
String ledState;
//servo
int pos = 0;
Servo servo_21;


void setup() 
{
servo_21.attach(21,500,2400);
pinMode (ledPin, OUTPUT);
pinMode (servoPin, OUTPUT);
Serial.begin(9600);
//Wire.begin(I2C_SDA, I2C_SCL); //citi porti
Wire.begin();
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize (1);
display.setTextColor(WHITE);
//Welcome actions
display.setCursor(0,0);
display.println("Hello!!!");
display.display();
digitalWrite(ledPin, HIGH);
servo_21.write(180);
  delay(2000); // Pause for 2 seconds
digitalWrite(ledPin, LOW);
servo_21.write(0);
delay(1000); // Pause for 1 seconds
}

void loop() 
{
display.clearDisplay();
light= analogRead(A5); 
Serial.print(analogRead(A1));
float volt= analogRead(A1)/4095;
Serial.println(volt);
temp=(volt-10);
Serial.println(temp);
//servo map

pos=map(temp, 1,4095,0,180);
servo_21.write(pos);
Header();

LightPrint();

TempPrint();

LedStatePrint();

ServoStatePrint();

if(light<100)
{
  digitalWrite(ledPin, HIGH);
  ledState="IESLEGTA";
  }
else 
{
  digitalWrite(ledPin, LOW);
  ledState="    IZSLEGTA";
}
display.display();
delay(200);
}
void Header()
{  
display.setCursor(0,0);
display.print("*** SUPER IEKARTA ***");
}
void LightPrint()
{  
display.setCursor(0,10);
display.print("Gaismas ir ");
display.println(light);
}

void TempPrint()
{
display.setCursor(0,20);
display.print("Temperatura ");
display.println(temp);
}

void LedStatePrint()
{
display.setCursor(0,30);
display.print("LED ir ");
display.print(ledState);
}
void ServoStatePrint()
{
display.setCursor(0,40);
display.print("SERVO poz ir ");
display.print(pos);
}
