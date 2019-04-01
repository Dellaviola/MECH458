/*
        blinky.h

        2019-02-21

        Mario Dellaviola

        
        callback functions for timers
*/


#ifndef BLINKY_H_
#define BLINKY_H_

/* Standard Includes */
#include <stdlib.h>        
#include <stdint.h>

/* AVR Includes */
#include <avr/io.h>      
#include <util/atomic.h>    
#include <avr/interrupt.h> 

/* Program Includes */
#include "config.h"
#include "uart.h"
#include "timer.h"
#include "stepper.h"
#include "pwm.h"
#include "sys.h"
#include "linkedlist.h"

/*-----------------------------------------------------------*/
/* 		        Scheduler Functions                  */

void SERVER_Task(void*);
void ADC_Task(void *);
void MAG_Task(void *);
void EXIT_Task(void *);
void SERVER_Task(void*);
void BTN_Task(void*);
void WATCHDOG_Task(void*);

/*-----------------------------------------------------------*/
/* 		        LED/DEBUG Functions                  */

void D_Blinky (void *);
void C_Blinky (void *);
void C_Shifty (void *);
void C_Picky (void *);
void Do_Nothing (void *);
void Say_Hello(void *);

/*-----------------------------------------------------------*/
/* 		        Unused Functions                     */

void IDLE_Task(void*);
void ADD_Task(void*);
void STEPPER_Task(void*);

#endif //BLINKY_H_