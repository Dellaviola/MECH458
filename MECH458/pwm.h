/*
        pwm.h

        2019-02-21

        Mario Dellaviola

        
        pwm for DC motor
*/
#ifndef PWM_H_
#define PWM_H_

/* Standard Includes*/
#include <stdlib.h>
#include <stdint.h>

/* AVR Includes */
#include <avr/interrupt.h>
#include <util/atomic.h> 
#include <avr/io.h>   

/* Program Includes */
#include "config.h"

/*-----------------------------------------------------------*/

void PWM_Init(void);
void PWM(int);

/*-----------------------------------------------------------*/

#endif // PWM_H_