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

#define TURN_180 100


//Stepper Globals ------- moved into header, replaced with struct
// volatile char step[4] = {0x30, 0x06, 0x28, 0x05};
// static int stepNum = 0;
// static int motorPosition; //TODO: Track position


void Stepper_Setup()
{
    stepper._motorPosition = 0;
    stepper._stepNum = 0;
	stepper.direction = 0;
	PORTA = 0x30;

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

void Rotate(int target, int current, int next)
{
	//Steps and dirrection to position
	steps = NumSteps(target, current);
	//Steps and dirrection to next pos
	nextSteps = NumSteps(next, target);
	stepper._dirrection = (steps >= 0) ? CW : CCW;
	bool cont = (steps * nextSteps >= 0) TRUE : FALSE;

	//setpper can not take -ve numbers of steps
	Stepper(abs(steps), stepper._dirrection, cont);
}

void Stepper(int rotation, int direction, bool cont)
{
	//PORTC = 0xAA;
	//(stepper_param) arg;
	size_t i;
	volatile uint8_t step[4] = {0x36, 0x2E, 0x2D, 0x35};

	for (i = 0; i < rotation; i++)
	{
		//PORTC = (direction == CW) ? (PORTA = step[stepper._stepNum]) : (PORTA = step[3 - stepper._stepNum]);
		PORTC = (direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
		PORTA = (direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
		stepper._stepNum = (stepper._stepNum == 3) ? (stepper._stepNum = 0) : (stepper._stepNum + 1);
		Delay_Create(stepper._delay);
		PORTC = 0x00;

		//Simple accell / decel blcok
		if ((i > 5) && (stepper._delay > 6))
			stepper._delay -= 2;
		if (((roation - i) <= 5) && (stepper._delay < 20))
			stepper._delay += 2;
	}
	//if not continuing in the same dirrections reset the delay
	stepper._delay = (cont) ? stepper._delay : 20;
}
