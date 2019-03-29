/*
 * sys.h
 *
 * Created: 2019-03-27 12:53:32 PM
 *  Author: Mario
 */ 


#ifndef SYS_H_
#define SYS_H_

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
#include "delay.h"
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"
#include "string.h"

void SYS_Init(void);

void SYS_Pause(char[20]);

#endif /* SYS_H_ */