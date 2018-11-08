#ifndef ButtonCod_h
#define ButtonCod_h
#include "Arduino.h"
#include "SoundCod.h"


class button
{
    public:
        button(int* pin, int* Bot_D, int* Bot_E, int* Bot_C, int* Bot_A, int* Bot_N, int* Bot_O);
        int readbutton(char _LQ0);//L-->Long Pressed //Q-->Quick Pressed
        char pressedtime();

    private:
        int* _Bot_pin, _Bot_D, _Bot_E, _Bot_C, _Bot_A, _Bot_N, _Bot_O;
        char LQ0 = '0';//L-->Long Pressed //Q-->Quick Pressed
        int buttonState;
        bool lastButtonState = LOW;
        unsigned long lastDebounceTime = 0;
        unsigned long debounceLongPressed = 1000;
        unsigned long debounceQuickPressed = 10;
        int reading;
};  
#endif