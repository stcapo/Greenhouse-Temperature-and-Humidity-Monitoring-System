#include "buzzer.h"
sbit Buzzer = P2^2; // p2.5 for pz

void Delay500us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 247;
	while (--i);
}

void buzz_ms(u16 ms){
	while(ms--){
		Buzzer = !Buzzer;
		Delay500us();
	}
}