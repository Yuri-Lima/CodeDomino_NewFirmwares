#include "Arduino.h"
#include <EEPROM.h> 
#include "SoundCod.h"
#include "RecordFlash.h"
/*
Conteudo estudado 
    https://youtu.be/SAhR1h3LpDY?list=PL8iN9FQ7_jt45to9g75vZPFYlnjH3wXJS
    
    https://www.arduino.cc/en/Tutorial/EEPROMGet
    https://www.arduino.cc/en/Tutorial/EEPROMPut
*/

MyObject customVar;//CRIA O OBJETO QUE CHAMA OS VETORES DE CADA BOTÃO DA STRUCT

record::record(int adress)
{
	_eeAddress = adress;
    _eeAddress += sizeof(customVar.button_D_Buff); //Move o endereço a cara tamanho do vetor button_D_Buff, GENERICA.
}
bool record::writeRecord(int option, char *instructionBuff)
{
    switch (option)
	{
        case 1:
        break;
        case 2:
            EEPROM_readMany(sizeof(customVar.button_E_Buff), &customVar);
            strcpy(instructionBuff, customVar.button_E_Buff);
            Serial.print("EEPROM_readMany: ");
            Serial.println(instructionBuff);
        break;
        case 3:
        break;
        case 4:
            memset(instructionBuff, 0, sizeof(instructionBuff));
            Serial.print("EEPROM_readMany_instructionBuff: ");
            Serial.println(instructionBuff);
        break;
        case 5:
        break;
        case 6:
        break;
		case 7:
            for (int i = 0 ; i < EEPROM.length() ; i++) EEPROM.write(i, 0); Serial.println("EEPROM Apagada");
		break;
		case 8:
            Serial.println("customVar.buttons: ");
			strcpy(customVar.button_D_Buff, instructionBuff);
            strcpy(customVar.button_E_Buff, instructionBuff);
            strcpy(customVar.button_C_Buff, instructionBuff);
            strcpy(customVar.button_A_Buff, instructionBuff);
            strcpy(customVar.button_N_Buff, instructionBuff);
            strcpy(customVar.button_O_Buff, instructionBuff);
            Serial.println(customVar.button_D_Buff);
            Serial.println(customVar.button_E_Buff);
            Serial.println(customVar.button_C_Buff);
            Serial.println(customVar.button_A_Buff);
            Serial.println(customVar.button_N_Buff);
            Serial.println(customVar.button_O_Buff);
            EEPROM_writeMany(sizeof(customVar.button_E_Buff), &customVar);
		break;
		case 9:
	
    	break;
		case 10:
			
		break;
		case 11:
					
		break;
		case 12:
					
		break;
		default:
	
    	break;
	}
	return 1;
}
void record::EEPROM_writeMany(unsigned addressOffset, MyObject *customVar) 
{
    EEPROM.put(addressOffset, customVar);
    Serial.println("EEPROM PUT");
    Serial.print("addressOffset: ");
    Serial.println(addressOffset);
    Serial.print("customVar: ");
    Serial.println(customVar->button_E_Buff);
    for (int i = 0; i < addressOffset; ++i)
+        EEPROM.update(addressOffset+i, customVar->button_E_Buff);
}
void record::EEPROM_readMany(unsigned addressOffset, MyObject *customVar) 
{
    for (int i = 0; i < addressOffset; ++i)
+        EEPROM.read(addressOffset+i);
    Serial.println("EEPROM GET");
    Serial.println(customVar->button_D_Buff);
    Serial.println(customVar->button_E_Buff);
    Serial.println(customVar->button_C_Buff);
    Serial.println(customVar->button_A_Buff);
    Serial.println(customVar->button_N_Buff);
    Serial.println(customVar->button_O_Buff);
}


