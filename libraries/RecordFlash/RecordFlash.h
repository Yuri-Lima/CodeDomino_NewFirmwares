#ifndef RecordFlash_h
#define RecordFlash_h
#include "Arduino.h"
#include <EEPROM.h> 
#include "SoundCod.h"

extern int baz;
class record
{
    public:
        record(int adress);
        bool writeRecord(int option, char *instructionBuff);
        bool readRecord();
    private:
        void EEPROM_writeMany(unsigned addressOffset, char* array, unsigned size);
        void EEPROM_readMany(unsigned addressOffset, char* array, unsigned size);
        int _adress;
};
#endif