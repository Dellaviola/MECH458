//########################################################################
//# MILESTONE : 4
//# PROGRAM : 4~
//# PROJECT : Lab4
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : PWM motor controls
//# DATA
//# REVISED 11-02-2019
//########################################################################

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "PWM.h"

// Motor Dirrections
#define CW 0x04
#define CCW 0x08

//Uses PORTB[7]
void PWM_Init()
{
    // Set Waveform Generation Mode to 3 - Fast PWM with TOP = MAX, and OCRA = Compare value
    TCCR0A |= 0x83; // TCCR0A7:6 -> COM0A = 0b10	(inverted mode)
                    // TCCR0A1:0 -> WGM1:0 = 11		(Fast PWM)
    // Set Clock Source
    TCCR0B |= 0x03; // CS2:0 = 0b010 (prescaler = 8 for f_PWM = 488 Hz)
    // Set value we want timer to reset at (MAX)
    OCR0A = 0x80; // Sets PWM duty cycle = 50%
}

void PWM(const int dutyCycle)
{
    //OCR0A = 90;
	OCR0A = 0x80;
	if (dutyCycle) 
	{
		gMotorOn = 1;
		PORTB = ~0x0E;
	}
	else 
	{
		gMotorOn = 0;
		PORTB = ~0x0F;
	}
}

