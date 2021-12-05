#include <HCSR04.h> // Ultrasonic sensor library
#include <SoftwareSerial.h> // Alternative serial port library
SoftwareSerial mySerial(2, 3); // RX, TX - Alternative serial ports

#define lMotorsUzPrieksu 5
#define lMotorsAtpakal 6
#define kMotorsUzPrieksu 7
#define kMotorsAtpakal 8
#define trig 9
#define echo 10

HCSR04 hc(trig, echo);//initialisation class HCSR04 (trig pin , echo pin)
//Mainiigie
int incomingByte = 0;
int distance = 0;
long randNumber;

void setup() {
  mySerial.begin(9600);//BT - serial
  Serial.begin(9600); //Upload un monitor
  randomSeed(analogRead(0));
  
  pinMode(lMotorsUzPrieksu, OUTPUT);
  pinMode(lMotorsAtpakal, OUTPUT);
  pinMode(kMotorsUzPrieksu, OUTPUT);
  pinMode(kMotorsAtpakal, OUTPUT);
}

void loop() {
  // gaida burtu
  if (mySerial.available() > 0) {
    incomingByte = mySerial.read();
    Serial.println(incomingByte);
  }

  //uz prieksu
  if (incomingByte == 'w') {
    distance = hc.dist();// nomerit distanci
    Serial.println(distance);
    if (distance > 10) {
      uz_Prieksu();
    }
    incomingByte = 0;
  }

  //pa kreisi
  if (incomingByte == 'a') {

    paKreisi();
    incomingByte = 0;
  }

  //pa labi
  if (incomingByte == 'd') {
    paLabi();
    incomingByte = 0;
  }

  //atpakal
  if (incomingByte == 's') {
    atpakal();
    incomingByte = 0;
  }

  //dejo
  if (incomingByte == 'f') {
    incomingByte = 0;
    grozasReiz(5);
    uz_Prieksu(); uz_Prieksu(); uz_Prieksu();
    atpakal(); atpakal(); atpakal();
    grozasReiz(5);
    paLabi(); paLabi();
    uz_Prieksu(); uz_Prieksu(); uz_Prieksu();
    atpakal(); atpakal(); atpakal();
    grozasReiz(5);
    paKreisi(); paKreisi(); paKreisi(); paKreisi(); paKreisi();
  }

  // Automatiskaa Robota f-cija
  while (incomingByte == 'r') {
    merotUzPrieksu();
    skatitiesApkart();
    if (mySerial.available() > 0) {
      incomingByte = 0;
      break;
    };
  }
}

//// funkcijas
void grozasReiz(int reizes) {
  for (int i = 0; i < reizes ; i++) {
    if (mySerial.available() > 0) {
      incomingByte = 0;
      break;
    }
    paLabi(); paLabi();
    paKreisi(); paKreisi();
  }
}

void paKreisi() {
  digitalWrite(lMotorsUzPrieksu, HIGH);
  digitalWrite(kMotorsAtpakal, HIGH);
  delay(100);
  digitalWrite(lMotorsUzPrieksu, LOW);
  digitalWrite(kMotorsAtpakal, LOW);
}
//
void paLabi() {
  digitalWrite(kMotorsUzPrieksu, HIGH);
  digitalWrite(lMotorsAtpakal, HIGH);
  delay(100);
  digitalWrite(kMotorsUzPrieksu, LOW);
  digitalWrite(lMotorsAtpakal, LOW);
}
//
void atpakal() {
  //atpakal
  digitalWrite(lMotorsAtpakal, HIGH);
  digitalWrite(kMotorsAtpakal, HIGH);
  delay(300);
  digitalWrite(lMotorsAtpakal, LOW);
  digitalWrite(kMotorsAtpakal, LOW);

}
void uz_Prieksu() {
  digitalWrite(kMotorsUzPrieksu, HIGH);
  digitalWrite(lMotorsUzPrieksu, HIGH);
  delay(300);
  digitalWrite(kMotorsUzPrieksu, LOW);
  digitalWrite(lMotorsUzPrieksu, LOW);

}
void merotUzPrieksu() {
  distance = hc.dist();
  Serial.println(distance);
  while (distance > 10) {
    if (mySerial.available() > 0) {
      incomingByte = 0;
      break;
    }
    uz_Prieksu();
    distance = hc.dist();
  }
  atpakal();

}
void skatitiesApkart() {
  // peec random skaitla vai uz vienu pusi, vai uz otru
  distance = hc.dist();
  randNumber = random(2);//no 0 - 1
  if (randNumber == 0) {
    while (distance < 20) {
      //incomingByte = checkSerialForByte();
      if (mySerial.available() > 0) {
        incomingByte = 0;
        break;
      }
      paLabi();
      distance = hc.dist();
    }
  }
  else {
    while (distance < 20) {
      //incomingByte = checkSerialForByte();
      if (mySerial.available() > 0) {
        incomingByte = 0;
        break;
      }
      paKreisi();
      distance = hc.dist();
    }
  }
}
char checkSerialForByte() {
  if (mySerial.available() > 0) {
    byte incomingByte2 = mySerial.read();
    Serial.println(incomingByte2);
    return incomingByte2;
  }
}
