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
#include "delay.h"
#include "stepper.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"
#include "string.h"
#include "sys.h"

#define DATAMODE 0
#define LISTUNITTEST 0
#define TIMERUNITTEST 0


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
	
	SYS_Init();
	
	while(!gSysCalibrated)
	{
		if((PIND & 0x03) == 0x00) // Both Buttons
		{
			UART_SendString("Starting System!\r\n");
			gSysCalibrated = 1;
		}
	}
	
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		TIMER_Create(4000, 1, D_Blinky, NULL);		// Placeholder -- Calibration
		_timer[0].state = READY;
	
		TIMER_Create(27, 1, ADC_Task, NULL);		// ADC Handler
		_timer[1].state = BLOCKED;
	
		TIMER_Create(200, 1, MAG_Task, NULL);		// Magnetic Sensor Polling
		_timer[2].state = BLOCKED;
	
		TIMER_Create(8000, 1, EXIT_Task, NULL);		// Item Exit Handling
		_timer[3].state = BLOCKED;
	
		TIMER_Create(8000, 1, ADD_Task, NULL);		// Item Enter Handling
		_timer[4].state = BLOCKED;
	
		TIMER_Create(1000, 1, BTN_Task, NULL);		// Button Handling
		_timer[5].state = READY;
	
		TIMER_Create(27, 1, SERVER_Task, NULL);	// Event Handling
		_timer[6].state = READY;
	};
	// Put IDLE operations in infinite loop
	while (1){;}
		
	return 0;
}

ISR(BADISR_vect)
{
	while(1)
	{
		TIMER_Create(4000, 1, C_Blinky, NULL);
	}
}
