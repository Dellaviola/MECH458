/*
        gpio.h

        2019-02-21

        Mario Dellaviola

        gpio setup, pin debouncing.
*/

#ifndef GPIO_H_
#define GPIO_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"

int gpioInit();


#endif GPIO_H_