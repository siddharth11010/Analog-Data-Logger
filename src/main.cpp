#include<Arduino.h> 
#include <config.h>
#include<ADC_config.h>
#include <Functions.h>
#include<RTClib.h>
//#include<Ch376msc.h>
#include <SPI.h>
#include <SD.h>
// #include<comms.h>

#define SD_CS 5


RTC_DS3231 rtc;

volatile bool uploadButtonPressed = false;

void IRAM_ATTR uploadButtonISR() {
  // Serial.println("Upluad button pressed.");
    uploadButtonPressed = true;
}





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
  pinMode(UPLOAD_BUTTON, INPUT_PULLUP);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);


attachInterrupt(digitalPinToInterrupt(UPLOAD_BUTTON), uploadButtonISR, FALLING);


if(!LittleFS.begin(true)){
        DEBUG_PRINTLN("FATAL: LittleFS Mount Failed. Cannot serve web pages.");
    } else {
        DEBUG_PRINTLN("LittleFS Initialized.");
    }

 if (! rtc.begin()) {
    DEBUG_PRINTLN("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  /*if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    
   //rtc.adjust(DateTime(F(_DATE), F(TIME_)));
    
  }*/

 if (!SD.begin(SD_CS, SPI)) {
    DEBUG_PRINTLN("Card Mount Failed!");
    SwitchOff(LEDR, LEDG);
    sdCardIsReady = false;
    while (1);
  }
  else{
  DEBUG_PRINTLN("SD Card initialized.");
  sdCardIsReady = true;
  loadConfigFromLittleFS();
  swapLoggingGroups();
}
  mutex = xSemaphoreCreateMutex();
 WiFi.onEvent(onStationDisconnected, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
  DEBUG_PRINTLN("Ready. Press button to start AP.");
}


TaskHandle_t Channel1TaskHandle = NULL;
TaskHandle_t Channel2TaskHandle = NULL;
TaskHandle_t Channel3TaskHandle = NULL;
TaskHandle_t Channel4TaskHandle = NULL;
TaskHandle_t IndicatorTaskhandle = NULL;
TaskHandle_t APTaskHandle = NULL;

void APTask(void *parameter) {
  while (1) {

    // React to interrupt flag
    if (uploadButtonPressed) {
      uploadButtonPressed = false;   // clear flag

      if (!isApActive) {
        Serial.printf("Free Heap BEFORE WiFi: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Max Alloc Block: %d bytes\n", ESP.getMaxAllocHeap());
        swapLoggingGroups();
        startAccessPoint();
      }
      // else {
      //     stopAccessPoint();
      //     for (int ch = 1; ch <= 4; ch++) {
      //         clearLogFile(ch);
      //     }
      // }
    }

    // Keep AP services running
    if (isApActive) {
      dnsServer.processNextRequest();
      checkApTimeout();
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);  // allow scheduler to run
  }
}


void IndicatorTask(void* parameter){
 while(1){
   if(!digitalRead(SWITCH)){
    SwitchOn(LEDR, LEDG);
  }
  else{
    SwitchOff(LEDR, LEDG);
  }
  vTaskDelay(50/portTICK_PERIOD_MS);
}}

void Channel1Task(void *parameter) {
  while(!digitalRead(SWITCH)){
  C1Data = C1.Read(); 
  if(xSemaphoreTake(mutex, 0) == pdTRUE){
    DateTime now = rtc.now();
   
    Day = now.day();
    Month = now.month();
    Year = now.year();
    Hour = now.hour();
    Minute = now.minute();
    Second = now.second();
    writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, currentLogFiles[0]);
   xSemaphoreGive(mutex);
  }
        DEBUG_PRINTLN("Entry added to CSV. - 1" );
    vTaskDelay(C1.samplingRate/portTICK_PERIOD_MS);
  }

}

void Channel2Task(void *parameter) {
  while(!digitalRead(SWITCH)){
  C1Data = C2.Read(); 
  if(xSemaphoreTake(mutex, 0) == pdTRUE){
    DateTime now = rtc.now();
   
    Day = now.day();
    Month = now.month();
    Year = now.year();
    Hour = now.hour();
    Minute = now.minute();
    Second = now.second();
    
     writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, currentLogFiles[1]);
    xSemaphoreGive(mutex);
    }
  DEBUG_PRINTLN("Entry added to CSV. - 2" );
  vTaskDelay(C2.samplingRate/portTICK_PERIOD_MS);
  }
}

void Channel3Task(void *parameter) {
  while(!digitalRead(SWITCH)){

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
    writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, currentLogFiles[2]);
    }
    DEBUG_PRINTLN("Entry added to CSV. - 3" );
    vTaskDelay(C3.samplingRate/portTICK_PERIOD_MS);
  }  
}

void Channel4Task(void *parameter) {
while(!digitalRead(SWITCH)){
C1Data = C4.Read(); 
  if(xSemaphoreTake(mutex, 0) == pdTRUE){
    DateTime now = rtc.now();
   
    Day = now.day();
    Month = now.month();
    Year = now.year();
    Hour = now.hour();
    Minute = now.minute();
    Second = now.second();
    
    
    writeCSV(Year, Month, Day, Hour, Minute, Second, C1Data, currentLogFiles[3]);
    xSemaphoreGive(mutex);
  }
    DEBUG_PRINTLN("Entry added to CSV. - 4" );
    vTaskDelay(C4.samplingRate/portTICK_PERIOD_MS);

}}

void loop(){
  

xTaskCreatePinnedToCore(
  APTask,
  "APTask",
  10240,
  NULL,
  1,
  &APTaskHandle,
  1
);

// xTaskCreatePinnedToCore(
//   Channel1Task,
//   "Channel1Task",
//   10000,
//   NULL,
//   1,
//   &Channel1TaskHandle,
//   1);

//   xTaskCreatePinnedToCore(
//   Channel2Task,
//   "Channel2Task",
//   10000,
//   NULL,
//   1,
//   &Channel2TaskHandle,
//   1);

//   xTaskCreatePinnedToCore(
//   Channel3Task,
//   "Channel3Task",
//   10000,
//   NULL,
//   1,
//   &Channel3TaskHandle,
//   1);

//   xTaskCreatePinnedToCore(
//   Channel4Task,
//   "Channel4Task",
//   10000,
//   NULL,
//   1,
//   &Channel4TaskHandle,
//   1);

  xTaskCreatePinnedToCore(
  IndicatorTask,
  "IndicatorTask",
  2048,
  NULL,
  1,
  &IndicatorTaskhandle,
  1);

}