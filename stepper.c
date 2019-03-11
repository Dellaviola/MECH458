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

#include <stdlib.h> // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>		   // the header of i/o port
#include <util/atomic.h>   // atomic blocks to handle blocking tasks
#include <avr/wdt.h>	   // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "stepper.h"

#define TURN_180 100
#define CW 0x04
#define CCW 0x08

//TODO; Write spin down

void Stepper_Setup()
{
	stepper._stepNum = 0;
	stepper.direction = 0;
	stepper.target = 0;
	stepper.current = 0;
	stepper.next = 0;
	PORTA = 0x30;

	cli();
	//Initial delay of 20ms
	OCR2B = 0x9C;
	// Set to CTC Mode
	TCCR2A |= (1 << WGM21);
	//Set interrupt on compare match
	TIMSK2 |= (1 << OCIE2A);
	// set prescaler to 1024 and starts PWM
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

	sei();
	// enable interrupts
}

int NumSteps(int target, int current)
{
	int steps = (target - current);
	if (steps >= 0)
	{
		//if there are a positive number of steps > 180; subtract 180 and rotate the other way
		steps = (steps > TURN_180) ? (-1) * (steps - TURN_180) : (steps);
	}
	else
	{
		//if there are a negative number of steps < 180; add 180 and rotate the other way
		steps = (steps < (-1 * TURN_180)) ? (steps) : (-1) * (steps + TURN_180);
	}
	return steps;
}

void SetRotation(int target, int next)
{
	//Use this to set the target positions
	stepper.target = target;
	stepper.next = next;
	Rotate();
	OCR2B = 0x07 * stepper._delay;
}

void Rotate()
{
	//Steps and dirrection to position
	stepper._targetStep = NumSteps(stepper.target, stepper.current);
	//Steps and dirrection to next pos
	int nextSteps = NumSteps(stepper.next, stepper.target);
	stepper.direction = (stepper._targetStep >= 0) ? CW : CCW;
	stepper._willContinue = (stepper._targetStep * nextSteps >= 0) ? 1 : 0;

	//stepper can not take -ve numbers of steps
	stepper._targetStep = abs(stepper._targetStep);
}

ISR(TIMER2_COMPA_vect)
{
	volatile uint8_t step[4] = {0x36, 0x2E, 0x2D, 0x35};

	if (stepper._currentStep < stepper._targetStep)
	{
		PORTC = (stepper.direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
		PORTA = (stepper.direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
		stepper._stepNum = (stepper._stepNum == 3) ? (stepper._stepNum = 0) : (stepper._stepNum + 1);
		PORTC = 0x00;
		stepper._currentStep++;

		//Simple accell / decel blcok
		if ((stepper._currentStep > 5) && (stepper._delay > 8))
			stepper._delay -= 1;
		if (((stepper._targetStep - stepper._currentStep) <= 5) && (stepper._delay < 20))
			stepper._delay += 1;
	}
	else if (stepper._currentStep == stepper._targetStep)
	{
		stepper._currentStep = 0;
		stepper._targetStep = 0;
		stepper.current = stepper.target;
		stepper._delay = (stepper._willContinue) ? stepper._delay : 20;
	}

	OCR2B = 0x07 * stepper._delay;
}