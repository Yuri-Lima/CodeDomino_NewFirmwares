#ifndef BatLevel_Cod
#define BatLevel_Cod
#include "Arduino.h"



class BatLevelCod
{
    public:
        BatLevelCod(bool pin);//Buzzer
        long readVcc(byte samples);
    private:
        
};
#endif