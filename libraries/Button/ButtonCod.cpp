#include "Arduino.h"
#include "SoundCod.h"
#include "ButtonCod.h"

#define buzzerPin 3
#define debug 0

button::button(int pin, int Bot_D, int Bot_E, int Bot_C, int Bot_A, int Bot_N, int Bot_O)
{	pinMode(pin, INPUT);
    _Bot_pin = pin;
    _Bot_D = Bot_D;
    _Bot_E = Bot_E;
    _Bot_C = Bot_C;
    _Bot_A = Bot_A;
    _Bot_N = Bot_N;
    _Bot_O = Bot_O;
}

int button::readbutton(char LQ0)
{	
	sound buzzer(buzzerPin);
	short int option, sample = 5;
	//Serial.println(LQ0);
	if ((reading[1]  > 100) && (reading[1]  < _Bot_D) && (LQ0 == 'Q'))
	{
		option = 1;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif	
	}
	else if ((reading[1]  > _Bot_D ) && (reading[1]  < _Bot_E + 5) && (LQ0 == 'Q'))
	{
		option = 2;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_E ) && (reading[1]  < _Bot_C + 5) && (LQ0 == 'Q'))
	{
		option = 3;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_C ) && (reading[1]  < _Bot_A + 5) && (LQ0 == 'Q'))
	{
		option = 4;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_A ) && (reading[1]  < _Bot_N + 5) && (LQ0 == 'Q'))
	{
		option = 5;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_N ) && (reading[1]  < _Bot_O + 5) && (LQ0 == 'Q'))
	{
		option = 6;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}if ((reading[1]  > 100) && (reading[1]  < _Bot_D) && (LQ0 == 'L'))
	{
		option = 7;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif	
	}
	else if ((reading[1]  > _Bot_D ) && (reading[1]  < _Bot_E + 5) && (LQ0 == 'L'))
	{
		option = 8;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_E ) && (reading[1]  < _Bot_C + 5) && (LQ0 == 'L'))
	{
		option = 9;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_C ) && (reading[1]  < _Bot_A + 5) && (LQ0 == 'L'))
	{
		option = 10;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_A ) && (reading[1]  < _Bot_N + 5) && (LQ0 == 'L'))
	{
		option = 11;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if ((reading[1]  > _Bot_N ) && (reading[1]  < _Bot_O + 5) && (LQ0 == 'L'))
	{
		option = 12;
		buzzer.fineBeep();
		#if debug
			Serial.println(option);
		#endif
	}
	else if (reading[1]  < 100) option = 0;
	reading[1] = 0;
	delay(5);
	return option;
}
char button::pressedtime()
{
	reading[0] = analogRead(_Bot_pin) > 100;
	reading[1] = analogRead(_Bot_pin);
	while(reading[0])
	{
		reading[0] = analogRead(_Bot_pin) > 100;
		//reading[1] = analogRead(_Bot_pin);
		if (reading[0] != lastButtonState)
		{
    		lastDebounceTime = millis();
		}
		if ((millis() - lastDebounceTime) > debounceQuickPressed)
		{
			teste += 1;
			if(teste >= debounceLongPressed)
			{
				return 'L';//LongPressed
				
			}
		}
		lastButtonState = reading[0];
	}
	if((teste <= debounceLongPressed) && (teste >= debounceQuickPressed)) return 'Q';//QuickPressed
	teste = 0;
	if(teste == 0) return '0';
	
	
}

