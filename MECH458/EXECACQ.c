/*
 * EXECACQ.c
 *
 * Created: 2019-03-29 9:26:50 AM
 *  Author: Mario
 */ 

#include "EXECACQ.h"

void EXECACQ()
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
	
	// 1 period = 444 us
	// item appears on optical for about 64 ms (140 periods)

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
	
		//UART_SendString("System Ready...\r\n");
		PWM(0x80);
 		PORTC = 0xFE;
// 		g_ADCCount = 0;
// 		ADCSRA |= (1 << ADEN);
//		ADCSRA |= (1 << ADSC);
// 		PORTC ^= 0xFF;
	};
	
	//for(int g = 0; g < 10; g++) g_ADCResult[g] = 100;
	//gMotorOn = 1;
	//itemNode* fakeItem = LL_ItemInit(65000,250,UNCLASSIFIED);
	//HEAD = LL_ItemListInit(fakeItem);
// 	ADCSRA = (1 << ADEN);
// 	ADCSRA = (1 << ADSC);
	// Put IDLE operations in infinite loop
	cli();
	while (1)
	{
		//
			// TIMER SERVICE 
				// NO TASKS RUNNING : 44.4 us
				
			// ADC_TASK : 144 us
			
			// MAG_TASK : 10 us
			
			// EXIT_TASK : 2200 us
				//EXIT_TASK new : 32 us
				
			// BTN_TASK : 5 us
			
			// SERVER_Task : 7 us
			
			// ADD_Task : 2000 us	
				//	ADD_Task new : 3 us
			
			// ADC SERVICE : 150us
			
			// ADC INTERRUPT TIME : 7 us
			
			// STEPPER_Numsteps : 3 us
			
			// STEPPER_Rotate : 10 us
			
			// STEPPER_SetRotation : 16 us
			
			// STEPPER Service : 9 us
			
		// ADC Service time will be rounded up to a total 250 us to take ten readings and do averaging and assignment
			// wcet timer = 44 + 250 + 3 + 7 + 5 + 32 + 10 + ISR time (7 + 3 + 10 + 16 + 9) = 396 us
			// frame size will be 444 us
			// max U = 89%
			// Idle time can be used for non system critical tasks.
			
			
		// ISR LOAD TEST
			// All tasks enabled running every cycle
		
			// SERVER : 16 us
			// ADC : 115 us
			// MAG : 16 us
			// EXIT : 170 us
			// ADD : 10 us
			// BTN : 16 us
			
			// ISR to IDLE : 350 us, one adc interrupt occured
				// Slack : 90 us
			// ISR to IDLE with 10 ADC interrupts and all tasks active : 354 us
			
			// Calling STEPPER set rotation in while loop uses 15 us of slack
			// Stepper ISR has min period of around 7 ms, e : 10us
			
			// About 374us to service entire system with timer, stepper, adc.
			
			// SUMMARY:
				// Frame Size : 444 us
				// Max Utilization: ~374 us
			//PORTC = 0xFF;
// 			if (g_ADCCount == 10)
// 			{
// 				PORTC ^= 0xFF;
// 				g_ADCCount = 0;
// 				ADCSRA |= (1 << ADEN);
// 				ADCSRA |= (1 << ADSC);
// 				
// 			}
// 			if((PINE & 0x10) != 0x10){
// 				PORTC = 0xFF;
// 				 SYS_Pause(__FUNCTION__);
// 			}
			//PORTC = 0xFF;
			
			
			//PORTC ^= 0xFE;
	}
	return ;
}