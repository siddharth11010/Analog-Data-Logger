#include<Arduino.h>

class ADC{
    public:
    int pin;
    int width;
    ADC(int p){
        pin = p;
        pinMode(pin, INPUT);
        width = 9;
    }

    ADC(int p, int w){
        pin = p;
        pinMode(pin, INPUT);
        if(9<=w && w<=12){
            width = w;
        }
        else{
            width = 9;
        }
    }

    void SetResolution(int r){
        if(9<=r<=12){
            width = r;
        }
        else{
            width = 9;
        }
    }

    int Read(){
        analogSetWidth(width);
        return analogRead(pin);
    }


};