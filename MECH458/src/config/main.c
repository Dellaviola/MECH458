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

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "timer.h"
#include "config.h"
#include "blinky.h"
#include "adc.h"
#include "delay.h"
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"




int main()
{	
	cli();
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	uartInit();
	gpioInit();
	stepper_handle = -1;
	timer_handle = -1;
	delay_flag = -1;
	if (Timer_Init() != 0) uPrint("TIMER DID NOT INITIALIZE"); //red leds error
	Stepper_Setup();
	pwmSetup();
	adcSetup();
	
	sei();        // Enable global interrupts
	
	char data[50];
	uint16_t temp;

	
	while(1)
	{
		if (optHandler_1){
			startADC();
			Timer_Create(60000, 0, stopADC(), NULL );
		}

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			temp = getADC();
		}

		sprintf(data,"%d",temp);

		uartSendChar(data);

		if((BUTTON_1_PIN & PIN0) != 0){
			PORTB = ~0x0F;
			PORTC = ~0x0F;
		} else {
			PORTB = ~0x07;
			PORTC = ~0x07;
		}

	}
	return 0;
}

