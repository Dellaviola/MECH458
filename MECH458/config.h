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
#include "linkedlist.h"

// Settings

#define OPTICAL_1_PIN           PINE7
#define OPTICAL_2_PIN           PINE6
#define OPTICAL_3_PIN           PINE5
#define REFLECT_PIN             PINF0
#define MAGNET_PIN              PINE4
#define BUTTON_1_PIN            PIND0
#define BUTTON_2_PIN            PIND1


// Defines
// #define PIN7                    0x80
// #define PIN6                    0x40
// #define PIN5                    0x20
// #define PIN4                    0x10
// #define PIN3                    0x08
// #define PIN2                    0x04
// #define PIN1                    0x02
// #define PIN0                    0x01
//globals

	volatile int stepper_handle;
	volatile int timer_handle;
	volatile int delay_flag ;
	volatile uint16_t g_ADCResult[10];
	volatile uint16_t g_ADCCount;
	volatile uint8_t g_ADCFlag;
/*
        TIMER DEFINES
*/
#define MAX_TIMERS (8)
#define us
#define ms 0x03E8
#define ss


volatile uint16_t gTimerTick;
list* gScheduler;
volatile uint8_t gMotorOn;
volatile uint8_t gSysCalibrated;
/*
		LED error indicators
*/
#define TIMER_ERROR 0x01; //LED 0

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