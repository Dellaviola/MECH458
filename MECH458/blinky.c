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
	
	// E7 : O1 (Enter)
	if((PINE & 0x80) == 0) 
	{
		// Transition Detected O1 High -> Low : Item Enters
		if(pin7state)
		{
			// Signal the start of the system by placing the first node into stage1
			if(STAGE1 == NULL) STAGE1 = HEAD;

		}
		pin7state = 0;
	}
	
	// E6 : O2 (Reflective)
	if((PINE & 0x40) == 0) 
	{
		// Transition Detected O2 High -> Low : Stop ADC
		if(pin6state)
		{
			// Nothing happens here
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
			_timer[3].state = READY;
			SYS_Pause(__FUNCTION__);
		}
		pin5state = 0;
	}
	
	// E7 : O1 (Enter)
	if((PINE & 0x80) == 0x80) 
	{
		// Transition Detected O2 Low -> High : Item Exits O1
		if(!pin7state)
		{
			// Unblock MAG_Task
			_timer[2].state = READY;	
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
			}
			else
			{
				// Increment stage 2
				STAGE2 = LL_Next(STAGE2); 
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
			// Nothing happens here
		}
		pin5state = 1;			
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
	char buff[50];
	static int j = 0;
	j++;
	
	// Averaging
	// Use atomic blocks to prevent interrupts while writing to multi-byte data
	for(i = 0; i < 10; i++)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			total += g_ADCResult[i];
		}
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		total = total/10;
	}
	
	// Min Reflectivity Condition
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if((total > 10) && (total < 900) && (total < LL_GetRefl(STAGE2)))
		{
			if (STAGE2) LL_UpdateRefl(STAGE2, total);
		}
	}
	
	g_ADCCount = 0;

	// Block ADC_Task
	_timer[1].state = BLOCKED;

	// Restart ADC
	if((PINE & 0x40) == 0x40) ADCSRA |= (1 << ADSC);

} // ADC_Task

void MAG_Task(void* arg)
{
	//
	/*! 
	* \brief 	Checks ferro magnetic sensor
	*			If belt is moving a magnetic item will be detected within 30 cycles
	* \param	Unused
	*/
	static uint8_t tick = 0;	
	if (g_MotorOn) tick++;
	char buff[2];

	// If the item is magnetic
	if((PINE & 0x10) == 0)
	{
		LL_UpdateStatus(STAGE1, INITIALIZED);
		LL_UpdateMag(STAGE1, 1);
		STAGE1 = LL_Next(STAGE1);
		_timer[2].state = BLOCKED;
		tick = 0;
	}
	// If the item is not magnetic
	else if(tick > 30)
	{	
		LL_UpdateStatus(STAGE1, INITIALIZED);
		LL_UpdateMag(STAGE1, 0);
		STAGE1 = LL_Next(STAGE1);
		_timer[2].state = BLOCKED;
		tick = 0;
	}
} // MAG_Task

void EXIT_Task(void* arg)
{
	//
	/*! 
	* \brief 	Handles an item at the end of the conveyor belt
	*
	* \param	Unused
	*/
	// If tray is in position
	if(stepper.current == LL_GetClass(HEAD))
	{
		if(!gMotorOn) PWM(0x80);
		LL_UpdateStatus(HEAD, EXPIRED);
		HEAD = LL_Next(HEAD);
		STEPPER_SetRotation(LL_GetClass(HEAD), LL_GetClass(HEAD->next));
		_timer[3].state = BLOCKED;
	}
	// If tray is not in position
	else
	{
		PWM(0);
	}

	// Rampdown
	if(LL_GetClass(HEAD) == END_OF_LIST);
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
		if(debounce > 2)
		{
			// Both Buttons : No Function
			if((PIND & 0x03) == 0x00) 
			{
				UART_SendString("Both Buttons Pressed!\r\n");
				debounce = 0;
			}
			// Button 1 : Pause System
			else if ((PIND & 0x03) == 0x01) 
			{
				UART_SendString("Button1 Pressed!\r\r\r\rPausing System...");
				SYS_Pause("Pause Requested");
				g_IdleStartTime = 0;
				debounce = 0;
			}
			// Button 2 : Force Ramp Down 
			else if ((PIND & 0x03) == 0x02) 
			{
				UART_SendString("Button2 Pressed!\r\n");
				debounce = 0;
			}
			// Spurious
			else
			{
				
				debounce = 0;
			}
		}
	}	
} // BTN_Task

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
	delay_flag = -1;
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
