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

typedef struct stepper_param
{
	uint8_t _stepNum;
	uint8_t direction;
	uint8_t target;
	uint8_t current;
	uint8_t _targetStep;
	uint8_t _currentStep;
	uint8_t next;
	uint8_t _willContinue;
	uint8_t _isInitiated;
	uint8_t _accellStep;
} stepperParam;

stepperParam stepper;

void STEPPER_Init(void);
uint16_t STEPPER_NumSteps(uint8_t, uint8_t);
void STEPPER_Rotate(void);
void STEPPER_SetRotation(uint8_t, uint8_t);

#endif // STEPPER_H_
