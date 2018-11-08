#ifndef Sound_Cod
#define Sound_Cod
#include "Arduino.h"



class sound
{
    public:
        sound(int* pin);//Buzzer
        void soundHome();
        void error();
        void Beep();
        void fineBeep();
        void Beeplong();
        void soundOk();
        void soundEnd();
        void soundRecording();
        int expotpin();
        
    private:
        int* _buzzer;
};
#endif