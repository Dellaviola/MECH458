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

int GPIO_Init(void);
int OPT_1_Handler(void);
int OPT_2_Handler(void);
int OPT_3_Handler(void);

#endif //GPIO_H_