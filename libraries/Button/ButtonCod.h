#ifndef ButtonCod_h
#define ButtonCod_h
#include "Arduino.h"
#include "SoundCod.h"


class button
{
    public:
        button(int pin, int Bot_D, int Bot_E, int Bot_C, int Bot_A, int Bot_N, int Bot_O);
        int readbutton(char LQ0);
        char pressedtime();

    private:
        int _Bot_pin;
        int _Bot_D;
        int _Bot_E;
        int _Bot_C;
        int _Bot_A;
        int _Bot_N;
        int _Bot_O;
        int buttonState;
        char LQ0 = '0';
        bool lastButtonState = LOW;
        unsigned long lastDebounceTime = 0;
        unsigned long debounceLongPressed = 1000;
        unsigned long debounceQuickPressed = 10;
        int reading;
};  
#endif