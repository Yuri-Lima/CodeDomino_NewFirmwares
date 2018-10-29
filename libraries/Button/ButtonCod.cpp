#include "Arduino.h"
#include "SoundCod.h"
#include "ButtonCod.h"

#define buzzerPin 3
#define debug 1

button::button(int pin, int Bot_D, int Bot_E, int Bot_C, int Bot_A, int Bot_N, int Bot_O)
{
    pinMode(pin, INPUT);
    _Bot_pin = pin;
    _Bot_D = Bot_D;
    _Bot_E = Bot_E;
    _Bot_C = Bot_C;
    _Bot_A = Bot_A;
    _Bot_N = Bot_N;
    _Bot_O = Bot_O;
}

int button::readbutton()
{	
	sound buzzer(buzzerPin);
	short int option, value = 0, sample = 5;
	value = analogRead(_Bot_pin);
	if ((value > 100) && (value < _Bot_D))
	{
		option = 1;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif	
	}
	else if ((value > _Bot_D ) && (value < _Bot_E + 5))
	{
		option = 2;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((value > _Bot_E ) && (value < _Bot_C + 5))
	{
		option = 3;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((value > _Bot_C ) && (value < _Bot_A + 5))
	{
		option = 4;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((value > _Bot_A ) && (value < _Bot_N + 5))
	{
		option = 5;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((value > _Bot_N ) && (value < _Bot_O + 5))
	{
		option = 6;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if (value < 100) option = 0;
	delay(5);
	return option;
}