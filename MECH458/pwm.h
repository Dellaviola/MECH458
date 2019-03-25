//pwm.h
//implemnet PWM functions
#ifndef PWM_H_
#define PWM_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"

void PWM_Init(void);
void PWM(int);


#endif