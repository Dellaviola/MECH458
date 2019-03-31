/*
        gpio.h

        2019-02-21

        Mario Dellaviola

        gpio setup, pin debouncing.
*/

#ifndef GPIO_H_
#define GPIO_H_

/* AVR Includes */
#include <avr/io.h>       
#include <avr/interrupt.h> 

int GPIO_Init(void);

#endif //GPIO_H_