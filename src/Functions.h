#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include<config.h>
  void SwitchOn(int red, int green){
        digitalWrite(green, HIGH);
        digitalWrite(red, LOW);
  }

  void SwitchOff(int red, int green){
        digitalWrite(red, HIGH);
        digitalWrite(green, LOW);
  }

  void writeCSV(uint16_t Y,uint8_t M, uint8_t D, uint8_t H, uint8_t Mi, uint8_t S, uint16_t D1, File myFile, String FileName) {
  myFile = SD.open(FileName, FILE_APPEND);
  if (myFile) {
    
    myFile.print(D);
    myFile.print("-");
    myFile.print(M);
    myFile.print(",");
    myFile.print(Y);
    myFile.print(",");
    myFile.print(H);
    myFile.print(":");
    myFile.print(Mi);
    myFile.print(":");
    myFile.print(S);
    myFile.print(",");
    myFile.print(D1);
    myFile.print("\n");

    myFile.close();
    Serial.println("Entry added to CSV. " + FileName);
  } else {
    SwitchOff(LEDR, LEDG);
    Serial.println("Error opening file for writing.");
  }
}

bool checkSdCardSpace() {
    // if (!SD.begin()) {
    //     return; // Stop if SD card initialization fails
    // }

    // SD.cardSize() returns the total size of the card in bytes.
    uint64_t card_size = SD.cardSize();
    
    // SD.totalBytes() returns the total number of bytes available on the card (same as cardSize() for SD).
    uint64_t total_bytes = SD.totalBytes();
    
    // SD.usedBytes() returns the number of used bytes.
    uint64_t used_bytes = SD.usedBytes();
    
    // Calculate free space
    uint64_t free_bytes = total_bytes - used_bytes;

    Serial.println("\n--- SD Card Storage Information ---");
    // Display card size (in bytes, converted to MB or GB for readability)
    Serial.printf("Total Size: %.2f GB\n", (double)card_size / (1024 * 1024 * 1024));
    Serial.printf("Used Space: %.2f MB\n", (double)used_bytes / (1024 * 1024));
    Serial.printf("Free Space: %.2f MB\n", (double)free_bytes / (1024 * 1024));

    // To check if it's "full", you can define a low threshold (e.g., less than 10MB free)
    const uint64_t LOW_SPACE_THRESHOLD_MB = 10;
    const uint64_t LOW_SPACE_THRESHOLD_BYTES = LOW_SPACE_THRESHOLD_MB * 1024 * 1024;

    if (free_bytes < LOW_SPACE_THRESHOLD_BYTES) {
      return false;
    } else {
return true;    }
    Serial.println("-----------------------------------");
}