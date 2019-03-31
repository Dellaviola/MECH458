/*
        adc.h

        2019-02-21

        Mario Dellaviola

        
        callback functions for timers
*/

#ifndef ADC_H_
#define ADC_H_

/* Standard Includes*/
#include <stdlib.h>
#include <stdint.h>

/* AVR Includes */
#include <avr/interrupt.h>
#include <util/atomic.h> 

/* Program Includes */
#include "config.h"
#include "timer.h"
#include "config.h"

/*-----------------------------------------------------------*/

void ADC_Init(void);

/*-----------------------------------------------------------*/

#endif //ADC_H_