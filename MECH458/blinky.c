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
#include "blinky.h"
#include "uart.h"



void D_Blinky(void *arg)
{
	//flashing green and red onboard leds
	(void) arg;
	
	PORTD ^= 0xA0;
}

void C_Blinky(void *arg)
{
	//flashing the top 4 leds of the led bank
	(void) arg;
	PORTC ^= 0xFF;
}
void C_Shifty(void *arg)
{
	//flashing the top 4 leds of the led bank
	(void) arg;
	PORTC = (PORTC == 0xFF ? 0x01 : (++PORTC));
}
void C_Picky(void *arg)
{
	//flashing the top 4 leds of the led bank
	(uint8_t) arg;
	PORTC = arg;
}
void Do_Nothing(void *arg)
{
	(void) arg;
	delay_flag = -1;
}
void Say_Hello(void *arg)
{
	(void) arg;
	UART_SendString("Hello!\r\n");
}