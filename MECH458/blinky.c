/*
        blinky.c

        2019-02-21

        Mario Dellaviola

        call back functions used in timers
*/

// TODO: Figure out why ADC only runs 8 times.


#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>         
#include <avr/io.h>        // the header of i/o port
#include "config.h" 
#include "blinky.h"
#include "uart.h"
#include "timer.h"
#include "stepper.h"
#include "pwm.h"


void D_Blinky(void *arg)
{
	//flashing green and red onboard leds
	(void) arg;
	
	PORTD ^= 0xA0;
}

void C_Blinky(void *arg)
{
	//flashing the top 4 leds of the led bank
	(void) arg;
	PORTC ^= 0xFF;
}
void C_Shifty(void *arg)
{
	//flashing the top 4 leds of the led bank
	(void) arg;
	PORTC = (PORTC == 0xFF ? 0x01 : (++PORTC));
}
void C_Picky(void *arg)
{
	//flashing the top 4 leds of the led bank
	(uint8_t) arg;
	PORTC = arg;
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
void ADC_Task(void* arg)
{
	//
	/* Writes adc value to the correct list node
	 * Restarts conversion	
	 * RUNTIME at i < 10, 140 cycles : ~18us
	 */
	
	size_t i;
	uint32_t total = 0;
	
	for(i = 0; i < 10; i++)
	{
		total += g_ADCResult[i];
	}
	total = total/10;
	
	if((total > 10) && (total < 900) && (total < LL_GetRefl(STAGE2)))
	{
		LL_UpdateRefl(STAGE2, total);
	}
	
	g_ADCCount = 0;
	_timer[1].state = BLOCKED;
	ADCSRA |= (1 << ADSC);
	
}
void MAG_Task(void* arg)
{
	//
	/* Checks magnetic sensors
	 * runs for [pwm ticks] or until an item is found to be magnetic 
	 *	
	 */
	static uint8_t count = 0;
	static uint16_t tick = 0;
	if (gMotorOn) tick++;
	if((PINE & 0x08) == 0)
	{
		count++;
		if (count > 2)
		{
			LL_UpdateMag(STAGE1, 1);
			STAGE1 = LL_Next(STAGE1);
			_timer[2].state = BLOCKED;
			tick = 0;
		}
	}
	else if (tick > 4000)
	{
		LL_UpdateMag(STAGE1, 0);
		STAGE1 = LL_Next(STAGE1);
		_timer[2].state = BLOCKED;
		tick = 0;
	}
}
void EXIT_Task(void* arg)
{
	//
	/* checks position
	 * 
	 */
	if(stepper.current == LL_GetClass(HEAD))
	{
		HEAD = LL_Remove(HEAD);
		if(!gMotorOn) PWM(0x80);
		STEPPER_SetRotation(LL_GetClass(HEAD), LL_GetClass(HEAD->next));
		_timer[3].state = BLOCKED;
	}
	else
	{
		PWM(0);
	}
}
void IDLE_Task(void* arg)
{
	//
	/* does nothing
	 * 	placeholder for now
	 */
	while(1) {;}
}
void BTN_Task(void* arg)
{
	//
	static uint8_t debounce = 0;
	
	if (PIND & 0x03)
	{
		debounce++;
		if(debounce > 2)
		{
			if((PIND & 0x03) == 0x00) // Both Buttons : No Function
			{
				UART_SendString("Both Buttons Pressed!\r\n");
				debounce = 0;
			}
			else if ((PIND & 0x03) == 0x01) // Button 1 : Pause System
			{
				UART_SendString("Button1 Pressed!\r\n");
				debounce = 0;
			} 
			else if ((PIND & 0x03) == 0x02) // Button 2 : Ramp Down
			{
				UART_SendString("Button2 Pressed!\r\n");
				debounce = 0;
			}
			else
			{
				//spurious
				debounce = 0;
			}
		}
	}
	
}
void ADD_Task(void* arg)
{
	//
	itemNode* newNode = LL_ItemInit(65535, 255, UNCLASSIFIED);
	list* newList = LL_AddBack(HEAD, newNode);
	
	if (STAGE1 == NULL) STAGE1 = newList;
	TAIL = newList;

	_timer[2].state = READY;
	_timer[4].state = BLOCKED;
}
void STEPPER_Task(void* arg)
{
	//
}
void SERVER_Task(void* arg)
{
	//
	/* Event handler
	 * 	
	 */
	static uint8_t pin7state = 0;
	static uint8_t pin6state = 0;
	static uint8_t pin5state = 0;
	
	if((PINE & 0x40) == 0) // E7
	{
		if(pin7state)
		{
			// Transition Detected O2 High -> Low : Reflect Exit
			ADCSRA &= (0 << ADEN);
		} 
		pin7state = 0;
	}
	
	if((PINE & 0x20) == 0) // E6
	{
		if(pin6state)
		{
			// Transition Detected O1 High -> Low : New Item Enters
		}
		pin6state = 0;
	}
	
	if((PINE & 0x10) == 0) // E5
	{
		if(pin5state)
		{
			// Transition Detected O3 High -> Low : Item At End
			_timer[3].state = READY;
		}
		pin5state = 0;
	}
	
	if((PINE & 0x40) == 0x40) // E7
	{
		if(!pin7state)
		{
			// Transition Detected O2 Low -> High : Item Entering Reflective
			ADCSRA |= (1 << ADEN);
		}
		pin7state = 1;
	}
		
	if((PINE & 0x20) == 0x20) // E6
	{
		if(!pin6state)
		{
			// Transition Detected O1 Low -> High : Item Enters System
			_timer[4].state = READY;
		}
		pin6state = 1;			
	}
		
	if((PINE & 0x10) == 0x10) // E5
	{
		if(!pin5state)
		{
			// Transition Detected O3 Low -> High
		}
		pin5state = 1;			
	}
}

