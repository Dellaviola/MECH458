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

/* Header */
#include "stepper.h"

#define TURN_180 100
#define CW 0x04
#define CCW 0x08

//volatile uint8_t accell[6] = {0x94, 0x7D, 0x66, 0x56, 0x43, 0x40};
volatile uint8_t accell[23] = {0x6E, 0x66, 0x5F, 0x59, 0x54, 0x4F, 0x49, 0x45, 0x40, 0x3C, 0x3A, 0x37, 0x34, 0x32, 0x30, 0x2E, 0x2C, 0x2A, 0x28, 0x26, 0x25, 0x24, 0x23};
volatile uint8_t decell[6]  = {0, 4, 8, 10, 12, 14};
static volatile uint8_t position[6] = {100, 0, 50, 150, 100, 100};
volatile uint8_t MAX_ACCELL = 1;

extern list* STEPLIST;
extern list* HEAD;
//TODO; Write spin down

void STEPPER_Init()
{
	cli();
	// Initialize Stepper Parameters
	stepper._stepNum = 0;
	stepper.direction = 1;
	stepper.target = 0;
	stepper.current = 0;
	stepper.same = 0;
	stepper._currentStep = 0;
	stepper.next = 0;
	stepper._isInitiated = 0;
	stepper._accellStep = 0;
	stepper._willContinue = 0;

	// For calibration
	stepper._targetStep = 200;

	PORTA = 0x30;

	//Initial delay of 20ms
	OCR2A = 0x94;
	// Set to CTC Mode
	TCCR2A |= (1 << WGM21);
	//Set interrupt on compare match
	TIMSK2 |= (1 << OCIE2A);
	// set prescaler to 1024 and starts PWM
	TCCR2B |= ((1 << CS22) | (1 << CS21) | (1 << CS20));
	// set prescaler to 256 and starts PWM
	//TCCR2B |= ((1 << CS22) | (1 << CS21));

	// Enable Interrupts to calibrate
	sei();
} // STEPPER_Init

int STEPPER_NumSteps(uint8_t target, uint8_t current)
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
		steps = (steps < (-1 * TURN_180)) ? (-1) * (steps + TURN_180) : (steps);
	}
	return steps;
} // STEPPER_NumSteps

void STEPPER_Rotate()
{
	//Steps and direction to position
	stepper._targetStep = STEPPER_NumSteps(stepper.target, stepper.current);
	//Steps and direction to next position
	int nextSteps = STEPPER_NumSteps(stepper.next, stepper.target);
	//Determine rotation direction
	stepper.direction = (stepper._targetStep >= 0) ? CW : CCW;
	//Determine if the Next target will require a stop or not
	stepper._willContinue = (stepper._targetStep * nextSteps > 0) ? 1 : 0;

	//stepper can not take -ve numbers of steps
	stepper._targetStep = abs(stepper._targetStep);
	stepper._currentStep = 0;
} // STEPPER_Rotate

void STEPPER_SetRotation(uint8_t target, uint8_t next)
{
	//Use this function to set the target
	if(stepper.current != stepper.target)
	{
		stepper.early = 1;
		stepper.earlytarget = target;
		stepper.earlynext = next;
	}
	else
	{
		stepper.target = target;
		stepper.next = next;
		if(target == stepper.current) stepper.same = STEPPER_SAME;
		STEPPER_Rotate();
	}
}

ISR(TIMER2_COMPA_vect)
{
	
	volatile uint8_t step[4] = {0x36, 0x2E, 0x2D, 0x35};
	
	if(stepper.same) stepper.same--;
	
	if (stepper._currentStep == stepper._targetStep)
	{

		//if you are at the target, don't rotate any farther and adjust the current position
		stepper.current = stepper.target;
		if(stepper.early)
		{
			stepper.early = 0;
			STEPPER_SetRotation(stepper.earlytarget, stepper.earlynext);
		}
		//if the direction is changing reset the delay
		stepper._accellStep = (stepper._willContinue) ? stepper._accellStep : 0;
		OCR2A = accell[stepper._accellStep];
		stepper._itemCount++;
		
		//SET MAX ACCELL
		if(((stepper._itemCount%8)>=0)&&(MAX_ACCELL<8) ){
			MAX_ACCELL++;
		}
		
	}
	if ((stepper._currentStep < stepper._targetStep) && (stepper.same == 0))
	{
		//if your not at the target fire the motor
		PORTA = (stepper.direction == CW) ? (step[stepper._stepNum]) : (step[3 - stepper._stepNum]);
		stepper._stepNum = (stepper._stepNum == 3) ? 0 : (stepper._stepNum + 1);

		stepper._currentStep++;
		//Simple acceleration / deceleration block uses curve defined in accel
		if (((stepper._willContinue == 0) && (stepper._targetStep - stepper._currentStep) <= 6) && (accell[stepper._accellStep] < 0x6E))
		{
			stepper._accellStep = decell[stepper._targetStep - stepper._currentStep - 1];
		}
		else if ((stepper._currentStep > 10) && (accell[stepper._accellStep] > accell[23-MAX_ACCELL]))
		{
			stepper._accellStep++;
		}
		OCR2A = accell[stepper._accellStep];
	}
	if (stepper._isInitiated != 1)
	{
		if ((PINE & 0x08) == 0)
		{
			stepper._isInitiated = STEPPER_OFFSET;
		}
		if (stepper._isInitiated > 1) stepper._isInitiated--;
		if (stepper._isInitiated == 1)
		{
			stepper._isInitiated = 1;
			stepper._stepNum = 0;
			stepper.direction = 1;
			stepper.target = 0;
			stepper.current = 0;
			stepper._targetStep = 0;
			stepper._currentStep = 0;
			stepper.next = 0;
			stepper._willContinue = 0;
			stepper._accellStep = 0;
			stepper.early = 0;
			stepper.earlynext = 0;
			stepper.earlytarget = 0;
			stepper._itemCount = 0;
		}
	}
} // STEPPER_ISR