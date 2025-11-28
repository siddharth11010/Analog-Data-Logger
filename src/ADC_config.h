#include<Arduino.h>
#include<cstring>

class ADC{
    public:
    int pin;
    int id;
    String name;
    bool enabled;
    int resolution;
    int samplingRate;
    

    ADC(int p, int w, String ChannelName, int ChannelID, bool status){
        pin = p;
        name = ChannelName;
        id = ChannelID;
        enabled = status;
        pinMode(pin, INPUT);
        if(9<=w && w<=12){
            resolution = w;
        }
        else{
            resolution = 9;
        }
    }

    void SetResolution(int r){
        if(9<=r<=12){
            resolution = r;
        }
        else{
            resolution = 9;
        }
    }

    uint16_t Read(){
        analogSetWidth(resolution);
        return analogRead(pin);
    }


};

ADC C1(34, 12, "Channel 1", 1, true);
ADC C2(35, 12, "Channel 2", 2, true);
ADC C3(32, 12, "Channel 3", 3, true);
ADC C4(33, 12, "Channel 4", 4, true);