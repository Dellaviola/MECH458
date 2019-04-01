/*
        pwm.c

        2019-02-21

        Mario Dellaviola

        pwm
*/

/* Header */
#include "pwm.h"
#include "config.h"

/*-----------------------------------------------------------*/

void PWM_Init()
{
    // Set Waveform Generation Mode to 3 - Fast PWM with TOP = MAX, and OCRA = Compare value
    TCCR0A |= 0x83; // TCCR0A7:6 -> COM0A = 0b10	(inverted mode)
                    // TCCR0A1:0 -> WGM1:0 = 11		(Fast PWM)
    // Set Clock Source
    TCCR0B |= 0x03; // CS2:0 = 0b010 (prescaler = 8 for f_PWM = 488 Hz)
    // Set value we want timer to reset at (MAX)
    OCR0A = BELT_SPEED; // Sets PWM duty cycle = 50%
} // PWM_Init

void PWM(const int dutyCycle)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		OCR0A = BELT_SPEED;
		if (dutyCycle) 
		{
			g_MotorOn = 1;
			PORTB = ~0x0E;
		}
		else 
		{
			g_MotorOn = 0;
			PORTB = ~0x0F;
		}
	}
} // PWM
