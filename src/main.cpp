#include<Arduino.h> 
#include <config.h>
#include<ADC_config.h>
#include <Functions.h>
#include<RTClib.h>
//#include<Ch376msc.h>
#include <SPI.h>
#include <SD.h>
RTC_DS3231 rtc;


#define SD_CS 5

File myfile;

ADC C1(34, 12);
ADC C2(35, 12);
ADC C3(32, 12);
ADC C4(33, 12);


uint16_t Year;
uint8_t Month;
uint8_t Day;
uint8_t Hour;
uint8_t Minute;
uint8_t Second;
uint16_t C1Data;
static SemaphoreHandle_t mutex;
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

  /*if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    
   //rtc.adjust(DateTime(F(_DATE), F(TIME_)));
    
  }*/

 if (!SD.begin(SD_CS, SPI)) {
    Serial.println("Card Mount Failed!");
    while (1);
  }
  Serial.println("SD Card initialized.");
  mutex = xSemaphoreCreateMutex();
}
TaskHandle_t Channel1TaskHandle = NULL;
TaskHandle_t Channel2TaskHandle = NULL;
TaskHandle_t Channel3TaskHandle = NULL;
TaskHandle_t Channel4TaskHandle = NULL;

void Channel1Task(void *parameter) {
  while(1){
  C1Data = C1.Read(); 
  if(xSemaphoreTake(mutex, 0) == pdTRUE){
    DateTime now = rtc.now();
   
    Day = now.day();
    Month = now.month();
    Year = now.year();
    Hour = now.hour();
    Minute = now.minute();
    Second = now.second();
    writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, myfile, "/C1Data.csv");
   xSemaphoreGive(mutex);
  }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }

}

void Channel2Task(void *parameter) {
  while(1){
  C1Data = C2.Read(); 
  if(xSemaphoreTake(mutex, 0) == pdTRUE){
    DateTime now = rtc.now();
   
    Day = now.day();
    Month = now.month();
    Year = now.year();
    Hour = now.hour();
    Minute = now.minute();
    Second = now.second();
    
     writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, myfile, "/C2Data.csv");
    xSemaphoreGive(mutex);
    }
  vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void Channel3Task(void *parameter) {
  while(1){

C1Data = C3.Read();
    if(xSemaphoreTake(mutex, 0) == pdTRUE){
    DateTime now = rtc.now();
  
    Day = now.day();
    Month = now.month();
    Year = now.year();
    Hour = now.hour();
    Minute = now.minute();
    Second = now.second();
     xSemaphoreGive(mutex);
    writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, myfile, "/C3Data.csv");
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }  
}

void Channel4Task(void *parameter) {
while(1){
C1Data = C4.Read(); 
  if(xSemaphoreTake(mutex, 0) == pdTRUE){
    DateTime now = rtc.now();
   
    Day = now.day();
    Month = now.month();
    Year = now.year();
    Hour = now.hour();
    Minute = now.minute();
    Second = now.second();
    
    
    writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, myfile, "/C4Data.csv");
    xSemaphoreGive(mutex);
  }
    vTaskDelay(1000/portTICK_PERIOD_MS);

}}

void loop(){
xTaskCreatePinnedToCore(
  Channel1Task,
  "Channel1Task",
  10000,
  NULL,
  1,
  &Channel1TaskHandle,
  1);

  xTaskCreatePinnedToCore(
  Channel2Task,
  "Channel2Task",
  10000,
  NULL,
  1,
  &Channel2TaskHandle,
  1);

  xTaskCreatePinnedToCore(
  Channel3Task,
  "Channel3Task",
  10000,
  NULL,
  1,
  &Channel3TaskHandle,
  1);

  xTaskCreatePinnedToCore(
  Channel4Task,
  "Channel4Task",
  10000,
  NULL,
  1,
  &Channel4TaskHandle,
  1);

}