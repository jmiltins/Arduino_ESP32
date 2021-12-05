#include <Q2HX711.h> // Scale Library
#include <EEPROM.h> // To save in EEPROM


// Buttons
#define BUTTON_1        35
#define BUTTON_2        0
// Relays

#define RELAY_1 32
#define RELAY_2 33
// 

// EEPROM start
# define EEPROM_SIZE 16
// EEPROM end

// Weight data start
const byte hx711_data_pin = 12; // define weight data and clk pins
const byte hx711_clock_pin = 13;
float yy1 = 303.0; // calibrated mass to be added
long x1 = 0L;
long x0 = 0L;
float avg_size = 1.0; // amount of averages for each mass measurement
Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711
// Weight data End

void setup() 
{
/// Buttons
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  
  pinMode(RELAY_1, OUTPUT);
  digitalWrite(RELAY_1, HIGH);
  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_2, HIGH);
  
  Serial.begin(115200); // prepare serial port
  
EEPROM.begin(EEPROM_SIZE); // prepare EEPROM
 // EEPROM read x1 and yy1
  x0 = EEPROM.readLong(0);
  x1 = EEPROM.readLong(8);
   
}
void loop() 
{
  
//tare procedure
if (digitalRead(BUTTON_1) == LOW)
{
  tare(); 
}

// Push button 2, to start relay
if (digitalRead(BUTTON_2) == LOW)
{
  digitalWrite(RELAY_2, LOW); 
}
else
{
  digitalWrite(RELAY_2, HIGH); 
  }
int mass = measure();

  Serial.print(mass);
  Serial.println(" g");
}

int measure()
{
    // averaging reading
  long reading = 0;
  for (int jj=0;jj<int(avg_size);jj++){
    reading+=hx711.read();
  }
  reading/=long(avg_size);
  
  // calculating mass based on calibration and linear fit
  float ratio_1 = (float) (reading-x0);
  float ratio_2 = (float) (x1-x0);
  float ratio = ratio_1/ratio_2;
  float mass = yy1 * ratio;
  
  return mass;
  }

void tare()
{
  delay(1000); // allow load cell and hx711 to settle
  x1 = 0L;
  x0 = 0L;
  
  for (int ii=0;ii<int(avg_size);ii++)
  {
    delay(10);
    x0+=hx711.read();
  }
  x0/=long(avg_size);
  //Save in EEPROM
  EEPROM.writeLong(0,x0);
  //EEPROM.commit();
  
  Serial.println("Add Calibrated Mass");
  // calibration procedure (mass should be added equal to yy1)
  int ii = 1;
  while(true)
  {
    if (hx711.read()<x0+10000){
    } else {
      ii++;
      delay(2000);
      for (int jj=0;jj<int(avg_size);jj++){
        x1+=hx711.read();
      }
      x1/=long(avg_size);
        //Save in EEPROM

      EEPROM.writeLong(8,x1);
      EEPROM.commit();
      break;
    }
  }
  Serial.println("Calibration Complete");
    
  }
