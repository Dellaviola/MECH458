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
const uint16_t BLACK_BOUNDARY_HIGH = 950;
const uint16_t BLACK_BOUNDARY_LOW = 800;

const uint16_t WHITE_BOUNDARY_HIGH = 915;
const uint16_t WHITE_BOUNDARY_LOW = 905;

const uint16_t STEEL_BOUNDARY_HIGH = 650;
const uint16_t STEEL_BOUNDARY_LOW = 100;

const uint16_t ALUMINUM_BOUNDARY_HIGH = 100;
const uint16_t ALUMINUM_BOUNDARY_LOW = 20;

const uint16_t STAGE2_DELAY_COUNT = 400;
const uint16_t DROP_DELAY_COUNT = 2;
const uint16_t ITEM_MISSING_COUNT = 10000;

const uint8_t STEPPER_OFFSET = 5;
const uint8_t STEPPER_REVERSE = 5;

volatile uint8_t BELT_SPEED = 100;

// Make sure to use the correct lists
extern list* HEAD;
extern list* STAGE1;
extern list* STAGE2;
extern list* TAIL;
extern list* FRONT;
extern list* STEPLIST;

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
		_timer[1].state = BLOCKED;
		
		TIMER_Create(1, 1, MAG_Task, NULL);			// Magnetic Sensor Handler
		_timer[2].state = BLOCKED;
		
		TIMER_Create(1, 1, EXIT_Task, NULL);		// Item Exit Handling
		_timer[3].state = BLOCKED;
		
		TIMER_Create(2, 1, ADD_Task, NULL);		// Item Enter Handling
		_timer[4].state = BLOCKED;
		
		TIMER_Create(100, 1, BTN_Task, NULL);		// Button Handling
		_timer[5].state = READY;
		
		TIMER_Create(2000, 1, D_Blinky, NULL);		// Blinky Leds
		_timer[6].state = READY;					

		TIMER_Create(5000, 1, WATCHDOG_Task, NULL); // For Rampdown or system stalls
		_timer[7].state = BLOCKED;

		UART_SendString("System Ready...\r\n");
		PWM(0x80);
	};

	// For initial stepper positioning
	int memory = 0;
	static volatile uint8_t position[6] = {100, 0, 50, 150, 100, 100};
	// Put IDLE operations in infinite loop
	while (1)
	{	
		// Check for pause request	
		if(g_PauseRequest) SYS_Pause(__FUNCTION__);

// 		if(g_MotorOn)
// 		{
// 			if((g_Timer - LL_GetTick(HEAD)) < STAGE2_DELAY_COUNT)
// 			{
// 				BELT_SPEED = 150;
// 				PWM(1);
// 			}
// 			else 
// 			{   
// 				if(LL_GetClass(HEAD->prev) != LL_GetClass(HEAD)) BELT_SPEED = 100;
// 				PWM(1);
// 			}
// 		}
// 		if(STAGE2 && (g_Timer - LL_GetTick(HEAD) > ITEM_MISSING_COUNT))
// 		{
// 			// Item Missing
// 			SYS_Pause("!!!Item Missing!!!\r\n");
// 		}

		list* temp = HEAD;
		uint16_t reflVal; 
		
		while(temp)
		{
			// Classify any sortable nodes
			if(temp && (LL_GetClass(temp) == UNCLASSIFIED) && (LL_GetStatus(temp) == SORTABLE))
			{
	
				reflVal = LL_GetRefl(temp);				
				uint8_t magVal = LL_GetMag(temp);
				
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
				else if((reflVal >= WHITE_BOUNDARY_LOW) && (reflVal <= WHITE_BOUNDARY_HIGH))
				{
					LL_UpdateClass(temp, WHITE);
				}
				else if((reflVal >= BLACK_BOUNDARY_LOW) && (reflVal <= BLACK_BOUNDARY_HIGH))
				{
					LL_UpdateClass(temp, BLACK);
				}
				else
				{
					// Unknown non-magnetic object might be aluminum
// 					if((reflVal >= ALUMINUM_BOUNDARY_LOW) && (reflVal <= ALUMINUM_BOUNDARY_HIGH))
// 					{
// 						LL_UpdateClass(temp, ALUMINUM);
// 					}
				}
			}
			temp = LL_Next(temp);
		}
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
