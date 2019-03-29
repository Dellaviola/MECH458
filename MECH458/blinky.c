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
#include "sys.h"
#include "linkedlist.h"


void D_Blinky(void *arg)
{
	//PORTC = 0x40;
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
	/* Averages last 10 ADC values every 444 - 450 us for low pass filtering
	 * Checks if current items reflectivity is lower than new value
	 * Replaces item if condition is met and restarts ADC	
	 */
		
	//PORTC = 0x02;
	size_t i;
	uint32_t total = 0;
	
	for(i = 0; i < 10; i++)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			total += g_ADCResult[i];
		}
	}
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		total = total/10;
	}
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		if((total > 0) && (total < 2000) && (total < LL_GetRefl(STAGE2)) && ((PINE & 0x40) == 0x40))
		{
			if (STAGE2) LL_UpdateRefl(STAGE2, total);
		}
	}
	
	g_ADCCount = 0;
	_timer[1].state = BLOCKED;
	ADCSRA |= (1 << ADSC);
}
void MAG_Task(void* arg)
{
	//
	/* Checks magnetic sensor
	 * runs for [pwm ticks] or until an item is found to be magnetic 
	 *		
	 */
	
	//PORTC = 0x04;
	static uint8_t count = 0;
	static uint16_t tick = 0;
	if (gMotorOn) tick++;
	if((PINE & 0x10) == 0)
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
	else if (tick > 5)
	{
		LL_UpdateMag(STAGE1, 0);
		STAGE1 = LL_Next(STAGE1);
		_timer[2].state = BLOCKED;
		tick = 0;
	}
	
	//SYS_Pause(__FUNCTION__);
}
void EXIT_Task(void* arg)
{
	//
	/* Stops motor until tray is in position
	 * Processes item status
	 */
	//PORTC = 0x08;
	if(stepper.current == LL_GetClass(HEAD))
	{
		if(!gMotorOn) PWM(0x80);
		LL_UpdateStatus(HEAD, EXPIRED);
		HEAD = LL_Next(HEAD);
		STEPPER_SetRotation(LL_GetClass(HEAD), LL_GetClass(HEAD->next));
		_timer[3].state = BLOCKED;
	}
	else
	{
		PWM(0);
	}
	if(LL_GetClass(HEAD) == END_OF_LIST); // rampdown
	
	//SYS_Pause(__FUNCTION__);
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
	/* Debounces button inputs
	 *  
	 *	
	 */
	//PORTC = 0x20;
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
				UART_SendString("Button1 Pressed!\r\r\r\rPausing System...");
				SYS_Pause("Pause Requested");
				g_IdleStartTime = 0;
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
	/* When an item enters the first optical sensor
	 * Unblocks magnetic sensor task
	 *	
	 */
	//PORTC = 0x10;
	
	if (STAGE1 == NULL) STAGE1 = HEAD;
	LL_UpdateStatus(STAGE1, INITIALIZED);

	_timer[4].state = BLOCKED;
	_timer[2].state = READY;


	
	//SYS_Pause(__FUNCTION__);	
}
void STEPPER_Task(void* arg)
{
	//
}
void SERVER_Task(void* arg)
{
	//
	/* Optical Handler
	 * Tracks optical input states and unblocks tasks accordingly
	 * samples inputs at 2.25khz
	 */
	//PORTC = 0x01;
	static uint8_t pin7state = 1;
	static uint8_t pin6state = 0;
	static uint8_t pin5state = 1;
	
	if((PINE & 0x80) == 0) // E7
	{
		if(pin7state)
		{
			// Transition Detected O1 High -> Low : Item Enters
			_timer[4].state = READY;
		} 
		pin7state = 0;
	}
	
	if((PINE & 0x40) == 0) // E6
	{
		if(pin6state)
		{
			// Transition Detected O2 High -> Low : Stop ADC
			ADCSRA &= (0 << ADEN);
		}
		pin6state = 0;
	}
	
	if((PINE & 0x20) == 0) // E5
	{
		if(pin5state)
		{
			// Transition Detected O3 High -> Low : Item At End
			_timer[3].state = READY;
		}
		pin5state = 0;
	}
	
	if((PINE & 0x80) == 0x80) // E7
	{
		if(!pin7state)
		{
			// Transition Detected O2 Low -> High : Item Exits O1

		}
		pin7state = 1;
	}
		
	if((PINE & 0x40) == 0x40) // E6
	{
		if(!pin6state)
		{
			// Transition Detected O1 Low -> High : Item enters ADC
			STAGE2 = LL_Next(STAGE2);
			//ADCSRA |= (1 << ADEN);
			ADCSRA |= (1 << ADSC);
		}
		pin6state = 1;			
	}
		
	if((PINE & 0x20) == 0x20) // E5
	{
		if(!pin5state)
		{
			// Transition Detected O3 Low -> High : Item Exits System
		}
		pin5state = 1;			
	}
}

//ISR(INT7_vect)
//{
	
//}

//ISR(INT6_vect)
//{
	
//}
//ISR(INT5_vect)
//{
	
//}