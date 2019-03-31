/*
 * CALIBRATE.c
 *
 * Created: 2019-03-31 11:56:08 AM
 *  Author: Mario
 */ 


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

#include "CALIBRATE.h"

extern list* HEAD;
extern list* STAGE1;
extern list* STAGE2;
extern list* TAIL;
extern list* FRONT;


//volatile uint16_t gTimerTick = 0;

void CALIBRATE()
{
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
		_timer[6].state = READY;				//_timer[6]

		UART_SendString("System Ready...\r\n");
		PWM(0x80);
	};
	// Put IDLE operations in infinite loop
	while (1)
	{
		if(g_PauseRequest) SYS_Calibrate();
	}
	
	return;
}