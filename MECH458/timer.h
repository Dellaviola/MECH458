/*
        timer.h

        2019-02-21

        Mario Dellaviola

        timer prototypes and definitions.

        Timer Structure:
                            expiry:     timing length
                            periodic:   no repeat (0) repeat (1)
                            callback pointer: function called on timer interrupt
                            void*: available to pass a parameter to callback
        
        Timer_Init()

            sets up timer/counter1

        Timer_Create()

            makes a new timer stored in the timer struct.
            param1: number of milliseconds (up to 65000)
            param2: binary flag for periodicness
            param3: pointer to a function
            param4: NULL or paramater that can be passed to function
*/


#ifndef TIMER_H_
#define TIMER_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"
#include "linkedlist.h"

int Timer_Init(void);
int Timer_Create(uint16_t, int, void (*callback)(void *) , void *, uint8_t);
int Timer_Delete(int);
void Delay_Create(uint16_t);

#endif