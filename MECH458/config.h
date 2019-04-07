/*
        config.h

        2019-02-21

        Mario Dellaviola

        Universal definitions and literals.
*/

#ifndef CONFIG_H_
#define CONFIG_H_

/* Standard includes */
#include <stdint.h>
#include <stdbool.h>

/* AVR Includes */
#include <avr/io.h>
#include <util/atomic.h>

/* Program Includes */
#include "linkedlist.h"

// Setting Declarations

volatile uint8_t BELT_SPEED;

const uint16_t STAGE2_EXIT_TIME;
const uint16_t EXIT_DELAY;
const uint16_t MISSING_DELAY;

const uint8_t STEPPER_OFFSET;
const uint8_t STEPPER_REVERSE;
const uint8_t STEPPER_SAME;
const uint8_t STEPPER_RANGE;
const uint8_t STEPPER_SET;

// Globals
volatile uint16_t g_ADCResult[10];
volatile uint16_t g_ADCCount;
volatile uint8_t g_MotorOn; 
volatile uint8_t g_PauseRequest;
volatile uint8_t g_WDTimeout;
volatile uint16_t g_Timer;
volatile uint8_t g_UnclassifiedRequest;
volatile uint8_t g_MissingRequest;
/*
        TIMER DEFINES
*/

#define MAX_TIMERS (8)

/*
		List
*/


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