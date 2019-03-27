/*
        blinky.h

        2019-02-21

        Mario Dellaviola

        
        callback functions for timers
*/


#ifndef BLINKY_H_
#define BLINKY_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"


void D_Blinky (void *);
void C_Blinky (void *);
void C_Shifty (void *);
void C_Picky (void *);
void Do_Nothing (void *);
void Say_Hello(void *);
void ADC_Task(void *);
void MAG_Task(void *);
void EXIT_Task(void *);
void IDLE_Task(void*);
void SERVER_Task(void*);
void STEPPER_Task(void*);
void ADD_Task(void*);
void BTN_Task(void*);

#endif