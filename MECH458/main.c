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

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "timer.h"
#include "config.h"
#include "blinky.h"
#include "adc.h"
#include "delay.h"
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"
#include "string.h"
#include "sys.h"


// Test Modes
#define DATAMODE 0
#define LISTUNITTEST 0
#define TIMERUNITTEST 0
#define EXECMODE 0

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
		
		TIMER_Create(180, 1, ADD_Task, NULL);		// Item Enter Handling
		_timer[4].state = BLOCKED;
		
		TIMER_Create(50, 1, BTN_Task, NULL);		// Button Handling
		_timer[5].state = READY;
		
		TIMER_Create(1000, 1, D_Blinky, NULL);		// Blinky Leds
		_timer[6].state = READY;				//_timer[6]

		UART_SendString("System Ready...\r\n");
		PWM(0x80);
	};

	// Put IDLE operations in infinite loop
	while (1)
	{		
// 		if (g_IdleStartTime == 0)
// 		{
// 			g_IdleStartTime = TCNT1;
// 			char str[20];
// 			sprintf(str, "Processor Use = %u\r\n", (g_IdleStartTime - g_SchedulerStartTime));
// 			UART_SendString(str);
// 		}
		
// 		list* temp = HEAD;
// 		while(temp){
// 			if(temp && (LL_GetClass(temp) == UNCLASSIFIED) && (LL_GetStatus(temp) == INITIALIZED) && (LL_GetClass(temp) != END_OF_LIST))
// 			{
// 				//classify temp
// 				uint16_t reflVal = LL_GetRefl(temp);
// 				uint8_t magVal = LL_GetMag(temp);
// 				
// 				if(magVal)
// 				{
// 					if(reflVal < 150)
// 					{
// 						LL_UpdateClass(temp, ALUMINUM);
// 					}
// 					else
// 					{
// 						LL_UpdateClass(temp, STEEL);
// 					}
// 				}
// 				else if(reflVal < 800)
// 				{
// 					LL_UpdateClass(temp, BLACK);
// 				}
// 				else
// 				{
// 					LL_UpdateClass(temp, WHITE);
// 				}
// 			}
// 			temp = LL_Next(temp);
// 		}
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
