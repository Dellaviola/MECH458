// /*stepper.h*/
//Implements stepper motor functions

#ifndef STEPPER_H_
#define STEPPER_H_

/* Standard Includes */
#include <stdlib.h>        
#include <stdint.h>

/* AVR Includes */
#include <avr/io.h>      
#include <util/atomic.h>    
#include <avr/interrupt.h> 

/* Program Includes */
#include "config.h"
#include "pwm.h"

typedef struct stepper_param
{
	volatile uint8_t _stepNum;
	volatile uint8_t direction;
	volatile int target;
	volatile uint8_t current;
	volatile int _targetStep;
	volatile uint8_t _currentStep;
	volatile int next;
	volatile uint8_t same;
	volatile uint8_t _willContinue;
	volatile uint8_t _isInitiated;
	volatile uint8_t _accellStep;
	volatile uint8_t early;
	volatile uint8_t earlytarget;
	volatile uint8_t earlynext;
	volatile uint8_t _itemCount;
} stepperParam;

stepperParam stepper;

void STEPPER_Init(void);
int STEPPER_NumSteps(uint8_t, uint8_t);
void STEPPER_Rotate(void);
void STEPPER_SetRotation(uint8_t, uint8_t);

#endif // STEPPER_H_
