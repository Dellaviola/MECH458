// adc.h
//Implements ADC Functions



#ifndef ADC_H_
#define ADC_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"
//Sets up registers for ADC
void ADC_Init(void);

#endif