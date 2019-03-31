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

extern list* HEAD;
extern list* STAGE1;
extern list* STAGE2;
extern list* TAIL;
extern list* FRONT;

void D_Blinky(void *arg)
{
	//PORTC = 0x40;
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
void ADC_Task(void* arg)
{	
	//
	/* Averages last 10 ADC values every 444 - 450 us for low pass filtering
	 * Checks if current items reflectivity is lower than new value
	 * Replaces item if condition is met and restarts ADC	
	 */
		
	//PORTC = 0x02;
	//PORTC ^= 0xFF;
	size_t i;
	uint32_t total = 0;
	//char buff[50];
	static int j = 0;
	j++;
	
	for(i = 0; i < 10; i++)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			total += g_ADCResult[i];
// 			sprintf(buff, "Total: %u, i: %u, ADC: %u\r\n",total,i,g_ADCResult[i]);
// 			UART_SendString(buff);
		}
	}
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		total = total/10;
	}
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		if((total > 0) && (total < 2000) && (total < LL_GetRefl(STAGE2)))
		{
			if (STAGE2) LL_UpdateRefl(STAGE2, total);
		}
	}
	
	g_ADCCount = 0;
	_timer[1].state = BLOCKED;
	if((PINE & 0x40) == 0x40) ADCSRA |= (1 << ADSC);
	else
	{
// 		char bf[5];
// 		sprintf(bf, "Runs: %d/r/n", j);
// 		UART_SendString(bf);
	}
}
void MAG_Task(void* arg)
{
	//
	/* Checks magnetic sensor
	 * runs for [pwm ticks] or until an item is found to be magnetic 
	 *		
	 */
	
	//PORTC = 0x04;
	//static uint8_t count = 0;
	static uint16_t tick = 0;
	if (gMotorOn) tick++;
	//char buff[2];
	if((PINE & 0x10) == 0)
	{
		LL_UpdateStatus(STAGE1, INITIALIZED);
		LL_UpdateMag(STAGE1, 1);
		STAGE1 = LL_Next(STAGE1);
		_timer[2].state = BLOCKED;
// 		sprintf(buff,"%u" ,tick);
// 		SYS_Pause(buff);
		//count = 0;
		tick = 0;
	}
	else if(tick > 30)
	{	
		LL_UpdateStatus(STAGE1, INITIALIZED);
		LL_UpdateMag(STAGE1, 0);
		STAGE1 = LL_Next(STAGE1);
		_timer[2].state = BLOCKED;
		//SYS_Pause(__FUNCTION__);
//  		sprintf(buff,"%u" ,temp);
//  		SYS_Pause(buff);
		tick = 0;
		//count = 0;
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
		//if(!gMotorOn) PWM(0x80);
		LL_UpdateStatus(HEAD, EXPIRED);
		HEAD = LL_Next(HEAD);
		STEPPER_SetRotation(LL_GetClass(HEAD), LL_GetClass(HEAD->next));
		_timer[3].state = BLOCKED;
	}
	else
	{
		//PWM(0);
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
// 	if ((PINE & 0x08) == 0)
// 	{
// 		if (STAGE1 != NULL) STAGE1 = LL_Next(STAGE1);
// 		if (STAGE1 == NULL) STAGE1 = HEAD;
// 		LL_UpdateStatus(STAGE1, INITIALIZED);
// 		_timer[4].state = BLOCKED;
// 	}

	//_timer[2].state = READY;

	
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
	static uint8_t pin7state = 0;
	static uint8_t pin6state = 1;
	static uint8_t pin5state = 1;
//	static uint8_t temp = 0;
	
	if((PINE & 0x80) == 0) // E7
	{
		if(pin7state)
		{
			// Transition Detected O1 High -> Low : Item Enters		
			// Just signal the start of the system by placing the first node into stage 1
			// And enable the watchdog timer
			
			if(STAGE1 == NULL) STAGE1 = HEAD;
			_timer[7].state = READY;
			g_WDTimeout = 0;
		}
		pin7state = 0;
	}
	
	if((PINE & 0x40) == 0) // E6
	{
		if(pin6state)
		{
			// Transition Detected O2 High -> Low : Stop ADC
				// The ADC is started on the Low -> High edge
				// Once the ADC finishes ten conversions it enables the ADC handling task
				// The ADC handling task restarts conversions as long as this pin is high
				// If the pin goes High -> Low, the ADC task will finish and wont restart the ADC
				// So nothing happens here.
				g_WDTimeout = 0;
		}
		pin6state = 0;
	}
	
	if((PINE & 0x20) == 0) // E5
	{
		if(pin5state)
		{
			// Transition Detected O3 High -> Low : Item At End
			_timer[3].state = READY;
			//g_PauseRequest = 1;
			g_WDTimeout = 0;
		}
		pin5state = 0;
	}
	
	if((PINE & 0x80) == 0x80) // E7
	{
		if(!pin7state)
		{
			// Transition Detected O2 Low -> High : Item Exits O1
			// Unblock the magnetic sensor when the item leaves O1
			// The magnetic sensor blocks once the magnetism of the piece is inferred
			_timer[2].state = READY;	
			g_WDTimeout = 0;
		}
		pin7state = 1;
	}
		
	if((PINE & 0x40) == 0x40) // E6
	{
		if(!pin6state)
		{
			// Transition Detected O1 Low -> High : Item enters ADC
			if(STAGE2 == NULL)
			{
				STAGE2 = HEAD; // First Item enters stage 2
			}
			else
			{
				LL_UpdateStatus(STAGE2, SORTABLE);
				STAGE2 = LL_Next(STAGE2); // Increment stage 2
			}
			g_WDTimeout = 0;
			ADCSRA |= (1 << ADSC);
		}
		pin6state = 1;			
	}
		
	if((PINE & 0x20) == 0x20) // E5
	{
		if(!pin5state)
		{
			// Transition Detected O3 Low -> High : Item Exits System
			g_WDTimeout = 0;
		}
		pin5state = 1;			
	}
}

void WATCHDOG_Task(void* arg)
{
	// If this function runs twice then then no item has triggered an optical sensor for 4 seconds.
	if(g_WDTimeout) SYS_Pause(__FUNCTION__); 
	g_WDTimeout++;
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