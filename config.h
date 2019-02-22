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

/*
        TIMER DEFINES
*/
#define MAX_TIMERS (3)
#define us
#define ms 0x03E8
#define ss

/*
		LED error indicators
*/
#define TIMER_ERROR 0x01; //LED 0

#endif