/*
        blinky.c

        2019-02-21

        Mario Dellaviola

        call back functions used in timers
*/


#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>         
#include <avr/io.h>        // the header of i/o port
#include "config.h" 



void d_blinky(void *arg)
{
	//flashing green and red onboard leds
	(void) arg;
	
	PORTD ^= 0xA0;
}

void c_blinky(void *arg)
{
	//flashing the top 4 leds of the led bank
	(void) arg;
	PORTC ^= 0xF0;
}