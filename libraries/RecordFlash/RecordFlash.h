#ifndef RecordFlash_h
#define RecordFlash_h
#include "Arduino.h"
#include <EEPROM.h> 
#include "SoundCod.h"

extern int adress;
const int size_buffer = 20;
struct MyObject 
{
    char button_D_Buff[size_buffer];
    char button_E_Buff[size_buffer];
    char button_C_Buff[size_buffer];
    char button_A_Buff[size_buffer];
    char button_N_Buff[size_buffer];
    char button_O_Buff[size_buffer];
};
class record
{
    public:
        record(int adress);
        int Record(int option, char *instructionBuff);
        bool readRecord();
    private:
        int _eeAddress;
        int option;        
        
};
#endif