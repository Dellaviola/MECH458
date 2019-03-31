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
/* Header */
/*-----------------------------------------------------------*/
//TODO; Write spin down


void STEPPER_Init()
{
	stepper._stepNum = 0;
	stepper.direction = 1;
	stepper.target = 0;
	stepper.current = 0;
	stepper._targetStep = 200;
	stepper._currentStep = 0;
	stepper.next = 0;
	stepper._delay = 0x14;
	PORTA = 0x30;
	cli();
	//Initial delay of 20ms
	OCR2A = 0x90;
	// Set to CTC Mode
	TCCR2A |= (1 << WGM21);
	//Set interrupt on compare match
	TIMSK2 |= (1 << OCIE2A);
	// set prescaler to 1024 and starts PWM
	TCCR2B |= ((1 << CS22) | (1 << CS21) | (1 << CS20));
	
	sei();
	// enable interrupts
}

uint16_t STEPPER_NumSteps(uint8_t target, uint8_t current)
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

void STEPPER_Rotate()
{
	//Steps and direction to position
	stepper._targetStep = STEPPER_NumSteps(stepper.target, stepper.current);
	//Steps and direction to next pos
	int nextSteps = STEPPER_NumSteps(stepper.next, stepper.target);
	stepper.direction = (stepper._targetStep >= 0) ? CW : CCW;
	stepper._willContinue = (stepper._targetStep * nextSteps >= 0) ? 1 : 0;

	//stepper can not take -ve numbers of steps
	stepper._targetStep = abs(stepper._targetStep);
	stepper._currentStep = 0;
}

void STEPPER_SetRotation(uint8_t target, uint8_t next)
{
	cli();
	//Use this to set the target positions
	stepper.target = target;
	stepper.next = next;
	STEPPER_Rotate();
	OCR2A = 0x07 * stepper._delay;
	sei();
}

//TODO: double up steps and decrease timer scaling for higher accuracy

ISR(TIMER2_COMPA_vect)
{
// 	PORTC ^= 0xFE;
// 	PORTC |= 0x01;  
	volatile uint8_t step[4] = {0x36, 0x2E, 0x2D, 0x35};
	if (stepper._currentStep < stepper._targetStep)
	{
		//if your not at the target fire the motor
		PORTA = (stepper.direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
		stepper._stepNum = (stepper._stepNum == 3) ? 0 : (stepper._stepNum + 1);

		stepper._currentStep++;

		//Simple accel / decel block
		if ((stepper._currentStep > 5) && (stepper._delay > 6)){
			stepper._delay--;
			OCR2A = 0x07 * stepper._delay;
		}
		if (((stepper._targetStep - stepper._currentStep) <= 5) && (stepper._delay < 0x13)){
			stepper._delay++;
			OCR2A = 0x07 * stepper._delay;
		}
		
	}
	else if (stepper._currentStep == stepper._targetStep)
	{
		//if you are at the target, don't rotate any farther and adjust the current position
		stepper.current = stepper.target;
		//if the direction is changing reset the delay
		stepper._delay = (stepper._willContinue) ? stepper._delay : 20;
		OCR2A = 0x07 * stepper._delay;
		PORTA = (stepper._willContinue) ? PORTA : 0x00;
	}
	//PORTC &= 0xFE;
	//PORTC ^= 0xFF;
}