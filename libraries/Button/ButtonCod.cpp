#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif
#include "SoundCod.h"
#include "ButtonCod.h"


#define debug 0
#define erro_1 10
#define erro_2 5
const int buzzer_pin = 3;
        

button::button(int* pin, int* Bot_D, int* Bot_E, int* Bot_C, int* Bot_A, int* Bot_N, int* Bot_O)
{	pinMode(*pin, INPUT);
    _Bot_pin = *pin;
    _Bot_D = *Bot_D;
    _Bot_E = *Bot_E;
    _Bot_C = *Bot_C;
    _Bot_A = *Bot_A;
    _Bot_N = *Bot_N;
    _Bot_O = *Bot_O;
}

int button::readbutton(char _LQ0)
{	
	sound buzzer(&buzzer_pin);
	int option, sample = 5;

	if ((reading  > 100) && (reading  < _Bot_D + erro_1) && (_LQ0 == 'Q'))
	{
		option = 1;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif	
	}
	else if ((reading  > _Bot_D + erro_1) && (reading  < _Bot_E + erro_2) && (_LQ0 == 'Q'))
	{
		option = 2;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_E + erro_1) && (reading  < _Bot_C + erro_2) && (_LQ0 == 'Q'))
	{
		option = 3;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_C + erro_1) && (reading  < _Bot_A + erro_2) && (_LQ0 == 'Q'))
	{
		option = 4;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_A + erro_1) && (reading  < _Bot_N + erro_2) && (_LQ0 == 'Q'))
	{
		option = 5;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_N + erro_1) && (reading  < _Bot_O + erro_2) && (_LQ0 == 'Q'))
	{
		option = 6;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}if ((reading  > erro_1) && (reading  < _Bot_D + erro_1) && (_LQ0 == 'L'))
	{
		option = 7;
		buzzer.soundOk();
		#if debug
			Serial.println(option);
		#endif	
	}
	else if ((reading  > _Bot_D + erro_1) && (reading  < _Bot_E + erro_2) && (_LQ0 == 'L'))
	{
		option = 8;
		buzzer.soundOk();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_E + erro_1) && (reading  < _Bot_C + erro_2) && (_LQ0 == 'L'))
	{
		option = 9;
		buzzer.soundOk();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_C + erro_1 ) && (reading  < _Bot_A + erro_2) && (_LQ0 == 'L'))
	{
		option = 10;
		buzzer.soundOk();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_A + erro_1) && (reading  < _Bot_N + erro_2) && (_LQ0 == 'L'))
	{
		option = 11;
		buzzer.soundOk();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading  > _Bot_N + erro_1) && (reading  < _Bot_O + erro_2) && (_LQ0 == 'L'))
	{
		option = 12;
		buzzer.soundOk();
		#if debug
			Serial.println(option);
		#endif
	}
	else if (reading  < 100) option = 0;
	//reading = 0;
	delay(5);
	return option;
}
char button::pressedtime()
{
	sound buzzer(&buzzer_pin);
	bool flag = false,flag2 =true;
	reading = analogRead(_Bot_pin);
	buttonState = bool(reading > 100);
	while(buttonState)
	{
		buttonState = bool(analogRead(_Bot_pin) > 100);
		if (buttonState != lastButtonState) 
		{
			// if the state has changed, increment the counter
    		if (buttonState == HIGH) 
			{
    			// if the current state is HIGH then the button went from off to on:
    			LQ0 = 'Q';
				delay(10);
				#if debug
    				Serial.print("Rapido");Serial.print(" - ");Serial.println(reading);
				#endif	
				flag = true;
				buzzer.Beeplong();
  	  		}
    		// Delay a little bit to avoid bouncing
    		delay(2);
		}

		if (buttonState != lastButtonState)
		{
    		lastDebounceTime = millis();
		}
		if (((millis() - lastDebounceTime) > debounceLongPressed) && flag2)
		{
			LQ0 = 'L';
			#if debug
				Serial.println("Longo");
			#endif
			flag2 = false;
			buzzer.Beeplong();
		}
		lastButtonState = buttonState;
	}
		if(flag == true) return LQ0;
		else return '0';//QuickPressed	
	
}

