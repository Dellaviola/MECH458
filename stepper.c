//########################################################################
//# MILESTONE : 4
//# PROGRAM : 4
//# PROJECT : Lab4
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : Stepper motor controls
//# DATA
//# REVISED 11-02-2019
//########################################################################

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "timer.h"
#include "stepper.h"
#include "config.h"
#include "blinky.h"


//Stepper Globals ------- moved into header, replaced with struct
// volatile char step[4] = {0x30, 0x06, 0x28, 0x05};
// static int stepNum = 0;
// static int motorPosition; //TODO: Track position


void Stepper_Setup()
{
    stepper._motorPosition = 0;
    stepper._stepNum = 0;
	stepper.direction = 0;

}

void Stepper(int rotation, int direction)
{
	//PORTC = 0xAA;
	//(stepper_param) arg;
	size_t i;
	volatile uint8_t step[4] = {0x30, 0x06, 0x28, 0x05};

	for (i = 0; i < rotation; i++)
	{
		//PORTC = (direction == CW) ? (PORTA = step[stepper._stepNum]) : (PORTA = step[3 - stepper._stepNum]);
		PORTC = (direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
		PORTA = (direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
        stepper._stepNum = (stepper._stepNum == 3) ? (stepper._stepNum = 0) : (stepper._stepNum + 1) ;
        Delay_Create(20);
		PORTC = 0x00;
    }
}
