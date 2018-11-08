#include "Arduino.h"
#include <EEPROM.h> 
#include "SoundCod.h"
#include "RecordFlash.h"

record::record(int adress)
{
	_adress = adress;
}

bool record::writeRecord(int option, char *instructionBuff)
{
	EEPROM_writeMany(0, (char*)instructionBuff, sizeof(instructionBuff));
	return 1;
}

void record::EEPROM_writeMany(unsigned addressOffset, char* array, unsigned size) 
{
    for (int i = 0; i < size; ++i)
        EEPROM.update(addressOffset+i, array[i]);
}
void record::EEPROM_readMany(unsigned addressOffset, char* array, unsigned size) 
{
    for (int i = 0; i < size; ++i)
        EEPROM.read(2);
		//https://www.arduino.cc/en/Tutorial/EEPROMGet
        //https://www.arduino.cc/en/Tutorial/EEPROMPut
}


