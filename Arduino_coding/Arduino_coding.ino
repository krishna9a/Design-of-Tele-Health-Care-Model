#include <BlynkSimpleStream.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

#define BLYNK_TEMPLATE_ID "TMPLodqdHWKO"
#define BLYNK_DEVICE_NAME "Design of Tele Health  Care Model"
#define BLYNK_AUTH_TOKEN "dwcQ7xaZpLU7J510EHQ_x68TVOmEr9LA"
char auth[] = "dwcQ7xaZpLU7J510EHQ_x68TVOmEr9LA";



LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int temp;
int tempPin = A0;  // temperature Sensor Pin
int HBSensor = 4;  // Sensor Pin
int HBCount = 0;  // for heart beat count 
int HBCheck = 0; // for check heart  beat sensor on/off
int TimeinSec = 0; // for time
int HBperMin = 0; // heart beat per min
int HBStart = 2;  // button 
int HBStartCheck = 0; // for check heart beat sensor is start or not

void timerIsr()
{
  if(HBStartCheck == 1)
  {
      TimeinSec = TimeinSec + 1;
    
  }
}
void setup() {
 
   Serial.begin(9600);
   Blynk.begin(Serial, auth);
  lcd.begin(20, 4);
  pinMode(2, OUTPUT); // Initialise digital pin 2 as an output pin
  pinMode(HBSensor, INPUT);
  pinMode(HBStart, INPUT_PULLUP);
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Body Temp  : "); 
  lcd.setCursor(0,2);
   lcd.print("HB per Min  : 0.0");
  
  
  
}

void loop() {
   Blynk.run();
  if(digitalRead(HBStart) == LOW){
       lcd.setCursor(0,3);
       lcd.print("HB Counting ...");
        HBStartCheck = 1;
    }
   
  if(HBStartCheck == 1)
  {
      if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
      {
        HBCount = HBCount + 1;
        HBCheck = 1;
    
      }
      if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
      {
        HBCheck = 0;   
      }
      if(TimeinSec == 10)
      {
          HBperMin = HBCount * 6;
          HBStartCheck = 0;
         Blynk.virtualWrite(V4, HBperMin);
          lcd.setCursor(14,2);
          lcd.print(HBperMin);
          lcd.print(" ");
         
          HBCount = 0;
          TimeinSec = 0;      
      }
  }

 // temperature sensor coding
   temp = analogRead(tempPin);
  float mv = (temp/1024.0)*5000;
  float cel = mv/10;
  Blynk.virtualWrite(V1, cel);
  lcd.setCursor(14,0);
  lcd.print(cel);
  lcd.print(" ");
  delay(100);
          
}
