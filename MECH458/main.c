//########################################################################
//# MECH 458 Project Code
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : 	This program sorts items using reflectance and inductance readings
//			Implemented with a clock driven scheduler operating off timer 1 interrupts
//			Stepper driven by timer 2 interrupts
//			No external interrupts are used
//
//# DATA	Interesting run time statistics
//					
//
//# REVISED 28-01-2019
//########################################################################


/* Standard Includes */
#include <stdlib.h>      
#include <stdint.h>
#include "string.h"

/* AVR Includes */
#include <avr/io.h>       
#include <avr/interrupt.h> 

/* Program Includes */
#include "timer.h"
#include "config.h"
#include "blinky.h"
#include "adc.h"
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"
#include "sys.h"


// Test Modes
#define DATAMODE 0
#define LISTUNITTEST 0
#define TIMERUNITTEST 0
#define EXECMODE 0
#define CALIBMODE 0

// Configure boundaries
const uint16_t BLACK_BOUNDARY_HIGH = 931;
const uint16_t BLACK_BOUNDARY_LOW = 886;

const uint16_t WHITE_BOUNDARY_HIGH = 874;
const uint16_t WHITE_BOUNDARY_LOW = 827;

const uint16_t STEEL_BOUNDARY_HIGH = 650;
const uint16_t STEEL_BOUNDARY_LOW = 299;

const uint16_t ALUMINUM_BOUNDARY_HIGH = 100;
const uint16_t ALUMINUM_BOUNDARY_LOW = 20;

const uint8_t BELT_SPEED = 200;

// Make sure to use the correct lists
extern list* HEAD;
extern list* STAGE1;
extern list* STAGE2;
extern list* TAIL;
extern list* FRONT;

int main(void)
{	
#if DATAMODE == 1
	#include "DATAACQ.h"
	DATAACQ();
	return 0;
#endif
#if LISTUNITTEST == 1
	#include "LISTTEST.h"
	LISTTEST();
	return 0;
#endif
#if TIMERUNITTEST == 1
	#include "TIMERTEST.h"
	TIMERTEST();
	return 0;
#endif
#if EXECMODE == 1
	#include "EXECACQ.h"
	EXECACQ();
	return 0;
#endif
#if CALIBMODE == 1
	#include "CALIBRATE.h"
	CALIBRATE();
	return 0;
#endif
	
	// Initialize the system
	SYS_Init();
	
	// Wait for start signal
	while(1)
	{
		if((PIND & 0x03) == 0x00) // Both Buttons
		{
			UART_SendString("Starting System!\r\n");
			break;
		}
	}
	
	// Start tasks and enable interrupts
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		TIMER_Create(1, 1, SERVER_Task, NULL);		// Optical Handling
		_timer[0].state = READY;
		
		TIMER_Create(1, 1, ADC_Task, NULL);			// ADC Handler
		_timer[1].state = READY;
		
		TIMER_Create(1, 1, MAG_Task, NULL);			// Magnetic Sensor Handler
		_timer[2].state = READY;
		
		TIMER_Create(1, 1, EXIT_Task, NULL);		// Item Exit Handling
		_timer[3].state = READY;
		
		TIMER_Create(1, 1, ADD_Task, NULL);		// Item Enter Handling
		_timer[4].state = READY;
		
		TIMER_Create(100, 1, BTN_Task, NULL);		// Button Handling
		_timer[5].state = READY;
		
		TIMER_Create(2000, 1, D_Blinky, NULL);		// Blinky Leds
		_timer[6].state = READY;				//_timer[6]

		TIMER_Create(9000, 1, WATCHDOG_Task, NULL);
		_timer[7].state = READY;

		UART_SendString("System Ready...\r\n");
		PWM(0x80);
		PORTC = 0xFF;
	};

	// For initial stepper positioning
	int memory = 0;
	static volatile uint8_t position[6] = {100, 0, 50, 150, 100, 100};

	// Put IDLE operations in infinite loop
	while (1)
	{
		/*
			Optimized ADC
			
			Using 6/2 FIR average.
			
			take 6 readings, remove min and max, average.
			
			ADC e : 40us
			ADC ISR 6e : 86ua
			
			TOTAL scheduler usage 220 us
			
			Design for 100% utilization
			
			Will leave some slack for stepper ~ 12us
			
			measured utilization with stepper = 243us
			leave 7 us for isr issues
			leave 15 us for idle time
			
			give 265 us
			
			MIN FRAME SIZE: 265us = 2120 ticks = 0x0848
			
			TESTING: 265us Frame
					 240us Execution
					 15us  Slack
					 
					 TOTAL U = 91% + Premptable idle time classifier
					 
					 Classifier can classify up to 3 objects per frame at max U;
			
		*/
		
		list* temp = HEAD;
		uint16_t reflVal = LL_GetRefl(temp);
		uint8_t magVal = LL_GetMag(temp);
		
		PORTC |= 0x08;
		
		if(magVal) //
		{
			if((reflVal >= ALUMINUM_BOUNDARY_LOW) && (reflVal <= ALUMINUM_BOUNDARY_HIGH))
			{
				LL_UpdateClass(temp, ALUMINUM);
			}
			else if ((reflVal >= STEEL_BOUNDARY_LOW) && (reflVal <= STEEL_BOUNDARY_HIGH))
			{
				LL_UpdateClass(temp, STEEL);
			}
			else
			{
				// Unknown Magnetic Object
			}
		}
		else if((reflVal >= BLACK_BOUNDARY_LOW) && (reflVal <= BLACK_BOUNDARY_HIGH))
		{
			LL_UpdateClass(temp, BLACK);
		}
		else if((reflVal >= WHITE_BOUNDARY_LOW) && (reflVal <= WHITE_BOUNDARY_HIGH))
		{
			LL_UpdateClass(temp, WHITE);
		}
		else
		{
			// Unknown non-magnetic object
		}
		if(memory == 0)
		{
			STEPPER_SetRotation(position[LL_GetClass(HEAD)],position[LL_GetClass(HEAD)]);
			memory = 1;
		}
		PORTC &= 0b11110111;
		
  	}
	return 0;
}

// Catch bad isrs
ISR(BADISR_vect)
{
	while(1)
	{
		PORTC = 0xAA;
	}
}
