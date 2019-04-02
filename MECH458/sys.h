/*
 * sys.h
 *
 * Created: 2019-03-27 12:53:32 PM
 *  Author: Mario
 */ 


#ifndef SYS_H_
#define SYS_H_

/* Standard Includes */
#include <stdlib.h>   
#include <stdint.h>
#include "string.h"

/* AVR Includes */
#include <avr/io.h>   
#include <avr/interrupt.h> 

/* Program Includes */
#include "timer.h"
#include "config.h"
#include "blinky.h"
#include "adc.h"
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"


/*-----------------------------------------------------------*/

void SYS_Init(void);
void SYS_Pause(char[20]);
void SYS_Calibrate(char [20]);

#endif /* SYS_H_ */