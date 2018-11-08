#include "Arduino.h"
#include "SoundCod.h"

sound::sound(int* pin)
{
    pinMode(*pin, OUTPUT);
    _buzzer = *pin;
	
}
void sound::soundHome()
{
	for(short int i=400;i<1000;i++)
	{
    	tone(_buzzer, i, 3);
    	delay(3);
	}
	noTone(_buzzer);
	delay(50);
	tone(_buzzer, 1000, 50);
	delay(50);
	noTone(_buzzer);
	delay(50);
	tone(_buzzer, 1000, 50);
	delay(50);
	noTone(_buzzer);
}
void sound::error()
{
	tone(_buzzer, 391, 800);
	delay(150);
	noTone(_buzzer);
	delay(30);
	tone(_buzzer, 261, 1500);
	delay(400);
	noTone(_buzzer);
}
void sound::Beep()
{
	tone(_buzzer, 391, 100);
	delay(100);
	noTone(_buzzer);
}
void sound::fineBeep()
{
	tone(_buzzer, 1047, 30);
	delay(30);
	noTone(_buzzer);
}
void sound::Beeplong()
{
	tone(_buzzer, 1047, 500);
	delay(100);
	noTone(_buzzer);
}
void sound::soundOk()
{
	tone(_buzzer, 440, 200);
	delay(200);
	noTone(_buzzer);
	delay(100);
	tone(_buzzer, 494, 200);
	delay(200);
	noTone(_buzzer);
	delay(100);
	tone(_buzzer, 523, 400);
	delay(400);
	noTone(_buzzer);
}
void sound::soundEnd()
{
	tone(_buzzer, 440, 50);
	delay(50);
	tone(_buzzer, 494, 50);
	delay(50);
	tone(_buzzer, 523, 400);
	delay(400);
	tone(_buzzer, 494, 50);
	delay(50);
	tone(_buzzer, 440, 50);
	delay(50);
	tone(_buzzer, 391, 400);
	delay(400);
	noTone(_buzzer);
}
void sound::soundRecording()
{
	tone(_buzzer, 1047, 30);
	delay(30);
	noTone(_buzzer);
	delay(100);
	tone(_buzzer, 1047, 30);
	delay(30);
	noTone(_buzzer);
	delay(100);
	tone(_buzzer, 1047, 30);
	delay(30);
	noTone(_buzzer);
}
int sound::expotpin()
{
	return *_buzzer;
}