/*
timer.c
2019-02-21
Mario Dellaviola
timer function implementations.
*/


#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "timer.h"
#include "string.h"
#include "config.h"
#include "blinky.h"
#include "uart.h"


static volatile uint16_t _timer_tick = 0;

ISR (TIMER1_COMPA_vect)    // Timer1 ISR
{
	PORTC ^= 0xFE;
	// Scheduler timer
		// Overhead: 44.4 us
		// min frame size 444us = 3552 ticks = 0x0DE0;
		
	// uint16_t start = TCNT1;
	// uint16_t stop = 0;
	_timer_tick++;
	//g_SchedulerStartTime = TCNT1;
	
	size_t i;
	
	for (i = 0; i < MAX_TIMERS; i++) {
		/* If the timer is enabled and expired, invoke the callback */
		//if (_timer[i].callback != NULL) PORTD = 0xF0;

		if ((_timer[i].callback != NULL) && (_timer[i].expiry <= _timer_tick)) {
			if (_timer[i].state == READY) _timer[i].callback(_timer[i].arg);
			
			if (_timer[i].periodic > 0) {
				/* Timer is periodic, calculate next expiration */
				_timer[i].expiry += _timer[i].periodic;
				} else {
				/* If timer is not periodic, clear the callback to disable */
				//_timer[i].callback = NULL;
			}
		}
	}
	/*
	 * stop = TCNT1;
	 * uint16_t total = stop - start;
	 * char buf[sizeof(uint16_t)+1];
	 * sprintf(buf,"%u\r\n", total);
	 * UART_SendString(buf);
	*/
	
	// Utilization diagnostic
	PORTC ^= 0xFE;
}


int TIMER_Init(void)
{
    //Configure the PORTD4 as output
    TCNT1 = 0x0000;
    OCR1A = 0x0DE0;   // 12C0 == 600us, 960 = 300 us 0x0DE0 444us
    TCCR1A = 0x00;
    TCCR1B = (1<<CS10) | (1<<WGM12);  // Timer mode with no prescaling and CTC mode (reset counter on compare mode)
    TIMSK1 = (1 << OCIE1A) ;   // Enable timer1 output compare interrupt
	
	memset(_timer, 0, sizeof(_timer));

	return 0;
}
/*
	Timer_Create:
					timeout_ms: number of ms to count too, max 65535 for non periodic,32767 for periodic 
					periodic: 0 or 1 for not repeating, repeating.
					
*/
int TIMER_Create(uint16_t timeout_ms, int periodic, void (*callback)(void *), void *arg)
{
	int handle = -1;
	size_t i;
	
	/*
		Find an available timer by incrementing the index (i) until a NULL callback
	*/
	
	for(i = 0; i < MAX_TIMERS; i++)
	{
		if (_timer[i].callback == NULL) break;
	}
	
	if (i < MAX_TIMERS)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			
			if (periodic != 0) 
			{
				_timer[i].periodic = timeout_ms;
			} 
			else 
			{
				_timer[i].periodic = 0;
			}
			
			_timer[i].callback = callback;
			_timer[i].arg = arg;
			_timer[i].expiry = timeout_ms + _timer_tick;
			_timer[i].state = READY;
			
		}
		
	handle = i;	
	return handle;
	}
	
	
    return 0;
}

void Delay_Create(uint16_t timeout_ms)
{
	delay_flag = TIMER_Create(timeout_ms,0,Do_Nothing,NULL);
	
	while(delay_flag != -1);
}

//TODO: Timer delete, 100 ms timer