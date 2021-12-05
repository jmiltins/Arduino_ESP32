/*vai šis ir 
komentāras?*/
//vienas rindas komentaars
int a[] = {0,1,2,3,4,5,6,7,8,9};
int x = 0;
int skaititajs = 1;
#define LED_PIN 13
#define ledOn digitalWrite(LED_PIN, HIGH)
#define ledOff digitalWrite(LED_PIN, LOW)
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

pinMode (LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:


ledOn; // iesleeedz lampinju
delay(skaititajs * 100);
ledOff;
delay(skaititajs * 100);
x +=1;
if (x >10)
{
  x = 0;
  skaititajs = skaititajs + 1;
}
Serial.println(x);
if (skaititajs = 10)
{
  Serial.print("Beigas!");
  skaititajs = 1;
  x = 0;
  }
}
