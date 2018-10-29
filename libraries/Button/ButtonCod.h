#ifndef ButtonCod_h
#define ButtonCod_h
#include "Arduino.h"
#include "SoundCod.h"

class button
{
    
    public:
        button(int pin, int Bot_D, int Bot_E, int Bot_C, int Bot_A, int Bot_N, int Bot_O);
        int readbutton();

    private:
        int _Bot_pin;
        int _Bot_D;
        int _Bot_E;
        int _Bot_C;
        int _Bot_A;
        int _Bot_N;
        int _Bot_O;
        
};
#endif