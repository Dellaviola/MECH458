/*
        config.h

        2019-02-21

        Mario Dellaviola

        Universal definitions and literals.
*/

#ifndef CONFIG_H_
#define CONFIG_H_

/*

        Standard includes

*/

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <math.h>
#include <stdfix.h>


//globals

	volatile int stepper_handle;
	volatile int timer_handle;
	volatile int delay_flag ;
	volatile uint16_t g_ADCResult;

/*
        TIMER DEFINES
*/
#define MAX_TIMERS (8)
#define us
#define ms 0x03E8
#define ss

/*
		LED error indicators
*/
#define TIMER_ERROR 0x01; //LED 0



//Stop Defines for readability of stepper
#define BRAKE 0x3f
#define OFF 0x00
#define TURN_30 17
#define TURN_60 33
#define TURN_90 50
#define TURN_180 100
#define DELAY 20
#define CW 0x04
#define CCW 0x08

#endif