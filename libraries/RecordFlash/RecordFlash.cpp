#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif

#include <EEPROM.h> 
#include "SoundCod.h"
#include "RecordFlash.h"
/*
Conteudo estudado 
    https://youtu.be/SAhR1h3LpDY?list=PL8iN9FQ7_jt45to9g75vZPFYlnjH3wXJS
    
    https://www.arduino.cc/en/Tutorial/EEPROMGet
    https://www.arduino.cc/en/Tutorial/EEPROMPut
*/
#define debug_record 0
#define debug_record_cases_read 0
#define debug_record_cases_write 1
const int buzzer_pin = 3;
MyObject customVar;//CRIA O OBJETO QUE CHAMA OS VETORES DE CADA BOTÃO DA STRUCT

record::record(int adress)
{
	_eeAddress = adress;
    _eeAddress += sizeof(customVar.button_D_Buff); //Move o endereço a cara tamanho do vetor button_D_Buff, GENERICA.
}
int record::Record(int option, char *instructionBuff)
{
    sound buzzer(&buzzer_pin);
    switch (option)
	{
        /*
        1° Faixa de opções pressionado curto
        */
        case 1:
            return 1;
        break;
        case 2:
            EEPROM.get(20, customVar.button_E_Buff);
            strcpy(instructionBuff, customVar.button_E_Buff);
            #if debug_record_cases_read
            Serial.print("EEPROM_read_Button_E: ");
            Serial.println(instructionBuff);
            #endif
        break;
        case 3:
            EEPROM.get(40, customVar.button_C_Buff);
            strcpy(instructionBuff, customVar.button_C_Buff);
            #if debug_record_cases_read
            Serial.print("EEPROM_read_Button_C: ");
            Serial.println(instructionBuff);
            #endif
        break;
        case 4:
            EEPROM.get(60, customVar.button_A_Buff);
            strcpy(instructionBuff, customVar.button_A_Buff);
            #if debug_record_cases_read
            Serial.print("EEPROM_read_Button_A: ");
            Serial.println(instructionBuff);
            #endif
        break;
        case 5:
            EEPROM.get(80, customVar.button_N_Buff);
            strcpy(instructionBuff, customVar.button_N_Buff);
            #if debug_record_cases_read
            Serial.print("EEPROM_read_Button_N: ");
            Serial.println(instructionBuff);
            #endif
        break;
        case 6:
            EEPROM.get(100, customVar.button_O_Buff);
            strcpy(instructionBuff, customVar.button_O_Buff);
            #if debug_record_cases_read
            Serial.print("EEPROM_read_Button_O: ");
            Serial.println(instructionBuff);
            #endif
        break;
        /*
        2° Faixa de opções pressionado longo
        */
        case 7://Correspondecia com button 1
            for (int i = 0 ; i < EEPROM.length() ; i++) 
            EEPROM.write(i, 0); 
            buzzer.soundEnd(); 
            #if debug_record_cases_write
            Serial.println("EEPROM Apagada");
            Serial.print("EEPROM_write_Button_D: ");
            #endif
		break;
		case 8://Correspondecia com button 2
            strcpy(customVar.button_E_Buff, instructionBuff);
            EEPROM.put(20, customVar.button_E_Buff);
            delay(500);
            buzzer.soundRecording();
            #if debug_record_cases_write
            Serial.print("EEPROM_write_Button_E: ");
            #endif
		break;
		case 9://Correspondecia com button 3
            strcpy(customVar.button_C_Buff, instructionBuff);
            EEPROM.put(40, customVar.button_C_Buff);
            delay(500);
            buzzer.soundRecording();
            #if debug_record_cases_write
            Serial.print("EEPROM_write_Button_C: ");
            #endif
    	break;
		case 10:
			strcpy(customVar.button_A_Buff, instructionBuff);
            EEPROM.put(60, customVar.button_A_Buff);
            delay(500);
            buzzer.soundRecording();
            #if debug_record_cases_write
            Serial.print("EEPROM_write_Button_A: ");
            #endif
		break;
		case 11:
			strcpy(customVar.button_N_Buff, instructionBuff);
            EEPROM.put(80, customVar.button_N_Buff);
            delay(500);
            buzzer.soundRecording();
            #if debug_record_cases_write
            Serial.print("EEPROM_write_Button_N: ");
            #endif	
		break;
		case 12:
			strcpy(customVar.button_O_Buff, instructionBuff);
            EEPROM.put(100, customVar.button_O_Buff);
            delay(500);
            buzzer.soundRecording();
            #if debug_record_cases_write
            Serial.print("EEPROM_write_Button_O: ");
            #endif
		break;
		default:
	        
    	break;
	}
	return 0;
}


