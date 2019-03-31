/*
 * DATAACQ.h
 *
 * Created: 2019-03-19 1:34:22 PM
 *  Author: Mario
 */ 


#ifndef DATAACQ_H_
#define DATAACQ_H_


#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "timer.h"
#include "config.h"
#include "blinky.h"
#include "adc.h"
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"

void DATAACQ(void);



#endif /* DATAACQ_H_ */