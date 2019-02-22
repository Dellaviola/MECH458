//########################################################################
//# MILESTONE : 2
//# PROGRAM : 2
//# PROJECT : Lab2
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : This program does Nightrider using the internal oscillators
//# DATA
//# REVISED 28-01-2019
//########################################################################

#include<avr/io.h>
#include<avr/interrupt.h>
#include "timer.h"
#include "config.h"
#include "blinky.h"




int main()
{	
	uint16_t timer_handle = 0;
	DDRD = (0xFF);              
	DDRC = (0xFF);
	PORTD = (0xA0);
	PORTC = (0xF0);
	if (Timer_Init() != 0) PORTC |= TIMER_ERROR; //red leds error
	sei();        // Enable global interrupts
	
	while(1)
	{
		 if (timer_handle == 0) 
		 {
			 
			
			timer_handle = Timer_Create(1000, 1, c_blinky, NULL); 
			timer_handle = Timer_Create(500, 1, d_blinky, NULL);  
		 }
	}
	return 0;
}

