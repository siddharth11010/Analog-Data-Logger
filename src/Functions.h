#include <Arduino.h>
  void SwitchOn(int red, int green){
        digitalWrite(green, HIGH);
        digitalWrite(red, LOW);
  }

  void SwitchOff(int red, int green){
        digitalWrite(red, HIGH);
        digitalWrite(green, LOW);
  }