/*
 * CALIBRATE.h
 *
 * Created: 2019-03-31 11:56:30 AM
 *  Author: Mario
 */ 


#ifndef CALIBRATE_H_
#define CALIBRATE_H_


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
#include "string.h"
#include "sys.h"

void CALIBRATE(void);




#endif /* CALIBRATE_H_ */