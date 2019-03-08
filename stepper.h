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

struct stepper_param
{
	uint8_t _stepNum;
	uint8_t _motorPosition;
	uint8_t direction;
	uint8_t rotation;
	uint8_t _delay = 20;
	//uint8_t step[4] = {0x30, 0x06, 0x28, 0x05};
};


struct stepper_param stepper;

void Stepper_Setup(void);

void Stepper(int, int);


#endif
