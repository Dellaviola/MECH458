// /*stepper.h*/
//Implements stepper motor functions

#ifndef STEPPER_H_
#define STEPPER_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
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
	uint8_t _delay;
	uint8_t _willContinue;
}stepperParam;

stepperParam stepper;

void STEPPER_Init(void);
uint16_t STEPPER_NumSteps(uint8_t, uint8_t);
void STEPPER_Rotate(void);
void STEPPER_SetRotation(uint8_t, uint8_t);


#endif
