/*
  Tutorial: Control Stepper Motor With Blynk App Using ESP32
  
  Board:
  - TTGO T-Display ESP32
  
  Output:
  - 12V 28BYJ-48 Stepper Motor
    https://my.cytron.io/c-motor-and-motor-driver/c-dc-motor/c-stepper-motor/p-12v-28byj-48-stepper-motor-plus-uln2003-driver-board

  Library Manager:
  - TFT_eSPI by Bodmer Version 1.4.20
  - Blynk by Volodymyr Shymanskyy Version 0.6.1
*/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
const char ssid[] = ""; // WiFi name
const char password[] = ""; // WiFi password

#include <BlynkSimpleEsp32.h>
char auth[] = "G89q9q0DFdT431UK_BTpnOO_ZBgu3hhl"; // Blynk Authentication Token
WiFiServer server(80);

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI(); // Resolution 135 x 240
#define FF17 &FreeSans9pt7b
#define FF21 &FreeSansBold9pt7b
#define ROW1 0,16
#define ROW2 0,38
#define ROW3 0,60
#define ROW4 0,82
#define ROW5 0,104
#define ROW6 0,126

#define BUTTON1 35
#define BUTTON2 0

#define STEPPER_IN1 27
#define STEPPER_IN2 26
#define STEPPER_IN3 25
#define STEPPER_IN4 33

#define REVOLUTION_STEP 2048 // 1 revolution

boolean stepperDirection = false;
int stepperStep = 0;
int stepperStepCount = 0;
boolean stepperMove = false;
long prevMillisStepper = 0;
int intervalStepper = 4; // Minimum is 2

boolean button1Pressed = false;
boolean button2Pressed = false;

BLYNK_WRITE(V0) // Button Widget is writing to pin V0
{
  stepperDirection = param.asInt();

  tft.fillRect(120, 65, 120, 25, TFT_BLACK);
  tft.setCursor(120, 82);
  if (stepperDirection) {
    tft.print("CCW");
  }
  else {
    tft.print("CW");
  }
}

BLYNK_WRITE(V1) // Button Widget is writing to pin V1
{
  int stepperSpeed = param.asInt();

  tft.fillRect(120, 87, 120, 25, TFT_BLACK);
  tft.setCursor(120, 104);
  if (stepperSpeed == 1) {
    intervalStepper = 4;
    tft.print("Low");
  }
  else if (stepperSpeed == 2) {
    intervalStepper = 3;
    tft.print("Medium");
  }
  else if (stepperSpeed == 3) {
    intervalStepper = 2;
    tft.print("High");
  }
}

BLYNK_WRITE(V2) // Button Widget is writing to pin V2
{
  stepperMove = true;
  stepperStepCount = 0;
  stepperStep = 1;

  tft.fillRect(120, 109, 120, 25, TFT_BLACK);
  tft.setCursor(120, 126);
  tft.print("Run");
}

void setup()
{
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  
  pinMode(STEPPER_IN1, OUTPUT);
  pinMode(STEPPER_IN2, OUTPUT);
  pinMode(STEPPER_IN3, OUTPUT);
  pinMode(STEPPER_IN4, OUTPUT);

  Serial.begin(115200);
  Serial.print("Initialize Blynk.");

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  tft.setFreeFont(FF21);
  tft.setTextColor(TFT_BLUE);
  tft.setCursor(ROW1);
  tft.print("Blynk Status:");
  
  tft.setFreeFont(FF17);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(120, 16);
  tft.print("Initialize...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, password);

  tft.fillRect(120, 0, 120, 35, TFT_BLACK);
  tft.setCursor(120, 16);
  tft.print("Ready!");

  tft.setFreeFont(FF21);
  tft.setTextColor(TFT_RED);
  tft.setCursor(ROW3);
  tft.print("STEPPER");
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(ROW4);
  tft.print("Direction:");
  tft.setCursor(ROW5);
  tft.print("Speed:");
  tft.setCursor(ROW6);
  tft.print("Status:");

  tft.setFreeFont(FF17);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(120, 82);
  tft.print("CW");
  tft.setCursor(120, 104);
  tft.print("Low");
  tft.setCursor(120, 126);
  tft.print("Stop");

  Blynk.virtualWrite(0, 0);
  Blynk.virtualWrite(1, 1);
  Blynk.virtualWrite(2, 0);
}

void loop()
{
  Blynk.run();
  
  if (digitalRead(BUTTON1) == LOW &&
      button1Pressed == false) {
    button1Pressed = true;
    
    stepperDirection = false;
    stepperMove = true;
    stepperStepCount = 0;
    stepperStep = 1;
  }
  else if (digitalRead(BUTTON1) == HIGH &&
           button1Pressed == true) {
    button1Pressed = false;
  }
  
  if (digitalRead(BUTTON2) == LOW) {
    
    stepperDirection = true;
    stepperMove = true;
    stepperStepCount = 0;
    stepperStep = 1;
  }
  else if (digitalRead(BUTTON2) == HIGH &&
           button2Pressed == true) {
    button2Pressed = false;
  }
  
  if (millis() - prevMillisStepper > intervalStepper) {
    
    if (stepperMove == true) {
      if (stepperDirection) {
        if (stepperStep++ >= 3) {
          stepperStep = 0;
        }
      }
      else {
        if (stepperStep-- == 0) {
          stepperStep = 3;
        }
      }

      if (stepperStepCount++ == REVOLUTION_STEP) {
        stepperMove = false;
        stepperStep = 4;

        Blynk.virtualWrite(2, 0);
        
        tft.fillRect(120, 109, 120, 25, TFT_BLACK);
        tft.setCursor(120, 126);
        tft.print("Stop");
      }

      switch (stepperStep) {
        case 0:
          digitalWrite(STEPPER_IN1, HIGH);
          digitalWrite(STEPPER_IN2, LOW);
          digitalWrite(STEPPER_IN3, LOW);
          digitalWrite(STEPPER_IN4, LOW);
          break;

        case 1:
          digitalWrite(STEPPER_IN1, LOW);
          digitalWrite(STEPPER_IN2, HIGH);
          digitalWrite(STEPPER_IN3, LOW);
          digitalWrite(STEPPER_IN4, LOW);
          break;

        case 2:
          digitalWrite(STEPPER_IN1, LOW);
          digitalWrite(STEPPER_IN2, LOW);
          digitalWrite(STEPPER_IN3, HIGH);
          digitalWrite(STEPPER_IN4, LOW);
          break;

        case 3:
          digitalWrite(STEPPER_IN1, LOW);
          digitalWrite(STEPPER_IN2, LOW);
          digitalWrite(STEPPER_IN3, LOW);
          digitalWrite(STEPPER_IN4, HIGH);
          break;

        default:
          digitalWrite(STEPPER_IN1, LOW);
          digitalWrite(STEPPER_IN2, LOW);
          digitalWrite(STEPPER_IN3, LOW);
          digitalWrite(STEPPER_IN4, LOW);
          break;
      }
    }

    prevMillisStepper = millis();
  }
}
