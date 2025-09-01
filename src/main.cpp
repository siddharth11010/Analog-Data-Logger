#include <config.h>
#include<ADC_config.h>
#include <Functions.h>
#include<RTClib.h>
#include<Ch376msc.h>

RTC_DS3231 rtc;


ADC C1(4, 12);
unsigned long CurrentTime = 0;
unsigned long PreviousTime = 0;
unsigned long interval = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);


 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    
   // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
  }
}

void loop() {
  CurrentTime = millis();
  if(digitalRead(SWITCH) == LOW){
    SwitchOn(LEDR, LEDG);
  if(CurrentTime>=(PreviousTime + interval)){
    DateTime now = rtc.now();
   
    
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print("   ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
     Serial.print("      Channel 1 reading = ");
    Serial.println(C1.Read());
    PreviousTime = CurrentTime;
  }
  else{
    SwitchOff(LEDR, LEDG);
  }
}

}

