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


void d_blinky (void *);
void c_blinky (void *);


#endif