/*
        blinky.c

        2019-02-21

        Mario Dellaviola

        call back functions used in timers
*/

/* Header */
#include "blinky.h"

/*-----------------------------------------------------------*/

// Make sure this file is accessing the correct list
extern list* HEAD;
extern list* STAGE1;
extern list* STAGE2;
extern list* TAIL;
extern list* FRONT;
extern list* N_1;

static volatile uint8_t position[6] = {100, 0, 50, 150, 100, 100};
extern stepperParam stepper;
static volatile uint16_t lastItemTick = 0;

/*-----------------------------------------------------------*/
/* 					Scheduler Functions 					 */

void SERVER_Task(void* arg)
{
	//
	/*! 
	* \brief 	Track optical sensor state changes
	*			Handles optical sensor events
	* \param	Unused
	*/	

	// State variables
	static uint8_t pin7state = 1;
	static uint8_t pin6state = 1;
	static uint8_t pin5state = 1;
	static uint8_t memory = 0;
// 	
// 	if(BUFFER != HEAD)
// 	{
// 		if(position[LL_GetClass(BUFFER)] == stepper.current)
// 		{
// 			BUFFER = LL_Next(BUFFER);
// 			if (LL_GetClass(BUFFER->prev) == LL_GetClass(BUFFER)) LL_Next(BUFFER);
// 			STEPPER_SetRotation(position[LL_GetClass(BUFFER)], position[LL_GetClass(BUFFER->next)]);
// 		}	
// 	}

	// E7 : O1 (Enter)
	if((PINE & 0x80) == 0) 
	{
		// Transition Detected O1 High -> Low : Item Enters
		if(pin7state)
		{
			// Signal the start of the system by placing the first node into stage1
			if(STAGE1 == NULL) STAGE1 = HEAD;
			if(LL_GetClass(HEAD) == END_OF_LIST) LL_UpdateStatus(HEAD,UNCLASSIFIED);
			g_WDTimeout = 0;

		}
		pin7state = 0;
	}
	
	// E6 : O2 (Reflective)
	if((PINE & 0x40) == 0) 
	{
		// Transition Detected O2 High -> Low : Leave Reflect
		if(pin6state)
		{
			// Item is sortable
			LL_UpdateStatus(STAGE2, SORTABLE);
			g_WDTimeout = 0;
		}
		pin6state = 0;
	}
	
	// E5 : O3 (Exit)
	if((PINE & 0x20) == 0) 
	{
		// Transition Detected O3 High -> Low : Item At End
		if(pin5state)
		{
			// Unblock EXIT_Task
			//g_PauseRequest = 1;
			_timer[3].state = READY;
			g_WDTimeout = 0;
		}
		pin5state = 0;
	}
	
	// E7 : O1 (Enter)
	if((PINE & 0x80) == 0x80) 
	{
		// Transition Detected O2 Low -> High : Item Exits O1
		if(!pin7state)
		{
			// Unblock MAG_Task and Watchdog Timer
			LL_UpdateTick(STAGE1, g_Timer);
			_timer[2].state = READY;
			_timer[4].state = READY;
			_timer[7].state = READY;	
		}
		pin7state = 1;
	}

	// E6 : O2 (Reflective)	
	if((PINE & 0x40) == 0x40) 
	{
		// Transition Detected O1 Low -> High : Item enters ADCs
		if(!pin6state)
		{
			if(STAGE2 == NULL)
			{
				// First Item enters stage 2
				STAGE2 = HEAD;
				if((g_Timer - LL_GetTick(STAGE2)) > STAGE1_STAGE2_TIME) g_MissingRequest = 1; 
				LL_UpdateTick(STAGE2, g_Timer);
				lastItemTick = STAGE2_EXIT_TIME + 50;
			}
			else
			{
				// Increment stage 2
				STAGE2 = LL_Next(STAGE2); 
				if((g_Timer - LL_GetTick(STAGE2)) > STAGE1_STAGE2_TIME) g_MissingRequest = 1; 
				LL_UpdateTick(STAGE2, g_Timer);
			}
			ADCSRA |= (1 << ADSC);
		}
		pin6state = 1;			
	}
		
	if((PINE & 0x20) == 0x20) // E5
	{
		// Transition Detected O3 Low -> High : Item Exits System
		if(!pin5state)
		{
						
		}
		pin5state = 1;			
	}
	if(g_Timer == 2*EXIT_DELAY)
	{
		if(HEAD == FRONT) g_MissingRequest = 1;
	}
	if((LL_GetClass(HEAD) == UNCLASSIFIED) && (stepper.early == 0) && ((g_Timer - LL_GetTick(HEAD)) > STAGE2_EXIT_TIME))
	{
		memory = 0;
	}
	if ((memory == 0) 
		&& (LL_GetClass(HEAD) != UNCLASSIFIED) 
		&& ((LL_GetClass(HEAD->next) != UNCLASSIFIED) || ((g_Timer - LL_GetTick(HEAD)) > STAGE2_EXIT_TIME))
		&& (stepper.current == stepper.target))
	{
		memory = 1;
		STEPPER_SetRotation(position[LL_GetClass(HEAD)], position[LL_GetClass(HEAD->next)]);
	}
} // SERVER_Task

void ADC_Task(void* arg)
{	
	//
	/*!
	 * \brief 	Averages last 10 ADC values every 444 - 450 us for low pass filtering
	 * 			Checks if current items reflectivity is lower than new value
	 * 			Replaces item if condition is met and restarts ADC	
	 *  \param  Unused
	 */

	size_t i;
	uint32_t total = 0;
	
	uint16_t max = g_ADCResult[0];
	uint16_t min = g_ADCResult[0];
	static uint8_t ticks = 0;
	ticks++;
	
	// Averaging
	// Use atomic blocks to prevent interrupts while writing to multi-byte data
	for(i = 0; i < 6; i++)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			total += g_ADCResult[i];
			if(g_ADCResult[i] < min) min = g_ADCResult[i];
			if(g_ADCResult[i] > max) max = g_ADCResult[i];
		}
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		total = (total - max - min)/4;
	}
	
	// Min Reflectivity Condition
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if((total > 10) && (total < 1100) && (total < LL_GetRefl(STAGE2)))
		{
			if (STAGE2) LL_UpdateRefl(STAGE2, total);
		}
	}
	
	g_ADCCount = 0;

	// Block ADC_Task
	_timer[1].state = BLOCKED;

	// Restart ADC
	if((PINE & 0x40) == 0x40) ADCSRA |= (1 << ADSC);
	else
	{
		ticks = 0;
	}

} // ADC_Task

void MAG_Task(void* arg)
{
	//
	/*! 
	* \brief 	Checks ferro magnetic sensor
	*			If belt is moving a magnetic item will be detected within 30 cycles
	* \param	Unused
	*/
	volatile static uint8_t tick = 0;	
	if (g_MotorOn) tick++;
		
	// If the item is magnetic
	if((PINE & 0x10) == 0)
	{
		LL_UpdateStatus(STAGE1, INITIALIZED);
		LL_UpdateMag(STAGE1, 1);
		STAGE1 = LL_Next(STAGE1);
		
		tick = 0;
		_timer[2].state = BLOCKED;

	}
	// If the item is not magnetic
	else if(tick > 50)
	{	
		LL_UpdateStatus(STAGE1, INITIALIZED);
		LL_UpdateMag(STAGE1, 0);
		STAGE1 = LL_Next(STAGE1);
		tick = 0;
		_timer[2].state = BLOCKED;
	}
} // MAG_Task

void EXIT_Task(void* arg)
{
	//
	/*! 
	* \brief 	Handles an item at the end of the conveyor belt
	*
	*/
	
	// Error Checks
	if(LL_GetStatus(HEAD) < SORTABLE) {_timer[3].state = BLOCKED; return;}
	if(g_Timer < EXIT_DELAY) {_timer[3].state = BLOCKED; return;}
	if(((g_Timer - LL_GetTick(HEAD)) < STAGE2_EXIT_TIME)) {_timer[3].state = BLOCKED; return;}
	if(LL_GetClass(HEAD) == UNCLASSIFIED)
	{
		g_UnclassifiedRequest = 1;
		LL_UpdateStatus(HEAD,EXPIRED);
		lastItemTick = g_Timer;
		HEAD = LL_Next(HEAD);
		STEPPER_SetRotation(position[LL_GetClass(HEAD)],position[LL_GetClass(HEAD->next)]);
		_timer[3].state = BLOCKED;
	} // Unclassified item handler
	
	//if(lastItemTick == 0) lastItemTick = EXIT_DELAY;
	if((LL_GetStatus(HEAD) == SORTABLE) && ((g_Timer - lastItemTick) >= (LL_GetTick(HEAD) - LL_GetTick(N_1) + MISSING_DELAY)))
	{
		g_MissingRequest = 1;
		_timer[3].state = BLOCKED;
		return;
	} // ITEM MISSINGITEM MISSING
	
	volatile uint8_t query = stepper._targetStep - stepper._currentStep;
	
	if((query < STEPPER_RANGE) && (stepper.early == 0))
	{
		if((query < 5) && (stepper.same == 0) && ((LL_GetTick(HEAD) - LL_GetTick(HEAD->prev)) > 300)) 
		{
			stepper.same = STEPPER_SET;
			stepper._accellStep = 0;
		}
		LL_UpdateStatus(HEAD,EXPIRED);
		lastItemTick =  g_Timer;
		g_TimerLast = g_Timer;
		N_1 = HEAD;
		HEAD = LL_Next(HEAD);
		PWM(1);
		STEPPER_SetRotation(position[LL_GetClass(HEAD)],position[LL_GetClass(HEAD->next)]);
		_timer[3].state = BLOCKED;
	}
	else
	{
		PWM(0);
	}
} // EXIT_Task

void BTN_Task(void* arg)
{
	//
	/*! 
	* \brief 	Debounces button presses
	*			Handles button events
	* \param	Unused
	*/
	
	static uint8_t debounce = 0;
	
	if (PIND & 0x03)
	{
		debounce++;
		if(debounce > 10)
		{
			// Both Buttons : No Function
			if((PIND & 0x03) == 0x00) 
			{
				//
			}
			// Button 1 : Pause System
			else if ((PIND & 0x03) == 0x01) 
			{
				g_PauseRequest = 1;
			}
			// Button 2 : Force Ramp Down 
			else if ((PIND & 0x03) == 0x02) 
			{
				_timer[4].state = READY;
			}
			// Spurious
			else
			{
				//
			}
		}
	}	
	else debounce = 0;
} // BTN_Task

void WATCHDOG_Task(void* arg)
{
	// If this function runs twice then then no item has triggered an optical sensor for 4 seconds.
	if(g_WDTimeout > 1) SYS_Pause(__FUNCTION__);//SYS_Rampdown(); 
	g_WDTimeout++;
}

/*-----------------------------------------------------------*/
/* 					Debug and LED functions					 */

void D_Blinky(void *arg)
{
	//flashing green and red onboard leds
	(void) arg;
	
	PORTD ^= 0xA0;
}
void C_Blinky(void *arg)
{
	// flashing the led bank
	(void) arg;
	PORTC ^= 0xFF;
}
void C_Shifty(void *arg)
{
	// shifting the led bank left
	(void) arg;
	PORTC = (PORTC == 0xFF ? 0x01 : (++PORTC));
}
void C_Picky(void *arg)
{
	// flashing the input leds
	PORTC = (uint8_t) arg;
}
void Do_Nothing(void *arg)
{
	(void) arg;
	//delay_flag = -1;
}
void Say_Hello(void *arg)
{
	(void) arg;
	UART_SendString("Hello!\r\n");
}

/*-----------------------------------------------------------*/
/* 					Unused Functions						 */


void ADD_Task(void* arg)
{
	//
	/*! 
	* \brief 	Initialize a new item to the list
	*			Functionality moved to compile time
	* \param	Unused
	*/	
	if(g_MotorOn) g_Timer++;
	return;
} // ADD_Task

void STEPPER_Task(void* arg)
{
	//
	/*! 
	* \brief 	Send required positon to stepper
	*			Functionality moved to TIMER2
	* \param	Unused
	*/	
} // STEPPER_Task
void IDLE_Task(void* arg)
{
	//
	/*! 
	* \brief 	Placeholder
	*			Would handle idle time, functionality moved to main
	* \param	Unused
	*/
	while(1) {;}
} // IDLE_Task
