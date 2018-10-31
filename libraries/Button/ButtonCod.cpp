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
	static int time_button = 0;
	value = analogRead(_Bot_pin);
	
	if (value >= 100)
	{
		time_button++;
	}
	else
	{
		option = 0;
		time_button = 0;
	}
	delay(10);

	//quick pressed
	if((time_button < 20) && (time_button > 0))
	{
		if ((value > 100) && (value < _Bot_D))
		{
			option = 1;
			buzzer.fineBeep();
			time_button = 0;
			#if debug
				Serial.println(option);
			#endif	
		}
		else if ((value > _Bot_D ) && (value < _Bot_E + 5))
		{
			option = 2;
			buzzer.fineBeep();
			time_button = 0;
			#if debug
			Serial.println(option);
			#endif
		}
		else if ((value > _Bot_E ) && (value < _Bot_C + 5))
		{
			option = 3;
			buzzer.fineBeep();
			time_button = 0;
			#if debug
				Serial.println(option);
			#endif
		}
		else if ((value > _Bot_C ) && (value < _Bot_A + 5))
		{
			option = 4;
			buzzer.fineBeep();
			time_button = 0;
			#if debug
				Serial.println(option);
			#endif
		}
		else if ((value > _Bot_A ) && (value < _Bot_N + 5))
		{
			option = 5;
			buzzer.fineBeep();
			time_button = 0;
			#if debug
				Serial.println(option);
			#endif
		}
		else if ((value > _Bot_N ) && (value < _Bot_O + 5))
		{
			option = 6;
			buzzer.fineBeep();
			time_button = 0;
			#if debug
				Serial.println(option);
			#endif
		}
		else if (value < 100)
		{
			option = 0;
			time_button = 0;
		}
		delay(5);
	}
	//long pressed
	else if((time_button > 30))
	{
		option = 7;
		buzzer.fineBeep();
		time_button = 0;
	}	
	return option;
}