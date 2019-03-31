//########################################################################
//# MILESTONE : 2
//# PROGRAM : 2
//# PROJECT : Lab2
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : This program does Nightrider using the internal oscillators
//# DATA
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
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"
#include "string.h"
#include "sys.h"

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

const uint16_t STEEL_BOUNDARY_HIGH = 524;
const uint16_t STEEL_BOUNDARY_LOW = 299;

const uint16_t ALUMINUM_BOUNDARY_HIGH = 76;
const uint16_t ALUMINUM_BOUNDARY_LOW = 35;

extern list* HEAD;
extern list* STAGE1;
extern list* STAGE2;
extern list* TAIL;
extern list* FRONT;


//volatile uint16_t gTimerTick = 0;

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
	
	SYS_Init();

	while(1)
	{
		if((PIND & 0x03) == 0x00) // Both Buttons
		{
			UART_SendString("Starting System!\r\n");
			break;
		}
	}
	
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		TIMER_Create(1, 1, SERVER_Task, NULL);		// Placeholder -- Calibration
		_timer[0].state = READY;
		
		TIMER_Create(1, 1, ADC_Task, NULL);		// ADC Handler
		_timer[1].state = BLOCKED;
		
		TIMER_Create(1, 1, MAG_Task, NULL);		// Magnetic Sensor Polling
		_timer[2].state = BLOCKED;
		
		TIMER_Create(1, 1, EXIT_Task, NULL);		// Item Exit Handling
		_timer[3].state = BLOCKED;
		
		TIMER_Create(180, 1, ADD_Task, NULL);		// Item Enter Handling
		_timer[4].state = BLOCKED;
		
		TIMER_Create(50, 1, BTN_Task, NULL);		// Button Handling
		_timer[5].state = READY;
		
		TIMER_Create(1000, 1, D_Blinky, NULL);	// Event Handling
		_timer[6].state = READY;				
		
		TIMER_Create(4505,1, WATCHDOG_Task, NULL); // Software watchdog (2 seconds)
		_timer[7].state = BLOCKED;

		UART_SendString("System Ready...\r\n");
		PWM(0x80);
	};
	// Put IDLE operations in infinite loop
	while (1)
	{		
		// Check for a pause request
		// Only pause during idle time to properly restart the scheduler on unpause
		if(g_PauseRequest) SYS_Pause(__FUNCTION__);

		list* temp = HEAD;
		
		LL_UpdateClass(temp, STEEL);
		temp = LL_Next(temp);
		LL_UpdateClass(temp, ALUMINUM);
		uint16_t reflVal; 

		while(temp)
		{
			if(temp && (LL_GetClass(temp) == UNCLASSIFIED) && (LL_GetStatus(temp) == SORTABLE))
			{
				//classify temp
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					reflVal = LL_GetRefl(temp);				
				}
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
			}
			temp = LL_Next(temp);
		}
  	}
	return 0;
}

ISR(BADISR_vect)
{
	while(1)
	{
		PORTC = 0xAA;
		//TIMER_Create(4000, 1, C_Blinky, NULL);
	}
}
