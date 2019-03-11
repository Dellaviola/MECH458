// /*stepper.h*/
//Implements stepper motor functions

#ifndef STEPPER_H_
#define STEPPER_H_

#include <stdlib.h> // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>		   // the header of i/o port
#include <util/atomic.h>   // atomic blocks to handle blocking tasks
#include <avr/wdt.h>	   // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"

struct stepper_param
{
	uint8_t _stepNum;
	uint8_t direction;
	uint8_t _delay;
	uint8_t target;
	uint8_t current;
	uint8_t next;
	uint8_t _willContinue;
	uint8_t _currentStep;
	uint8_t _targetStep;
};

struct stepper_param stepper;

void Stepper_Setup(void);

//For testing
void Stepper(int, int, int);

void SetRotation(int, int);

void Rotate(void);

#endif
