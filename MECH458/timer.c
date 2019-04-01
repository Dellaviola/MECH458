/*
timer.c
2019-02-21
Mario Dellaviola
timer function implementations.
*/

/* Header */
#include "timer.h"

/*-----------------------------------------------------------*/

static volatile uint16_t _timer_tick = 0;

ISR (TIMER1_COMPA_vect)    // Timer1 ISR
{
	//
	/*! 
	* \brief 	This is the system scheduler
	*			Clock Driven scheduling was used to implement this project
	*			The clock operates on 444 us frame size
	*			Worst case utilization is around 350 / 444 us
	*/	
	_timer_tick++;
	size_t i;
	PORTC ^= 0xFE;
	
	for (i = 0; i < MAX_TIMERS; i++) {
		
		// If the timer is enabled and expired
		if ((_timer[i].callback != NULL) && (_timer[i].expiry == _timer_tick)) {

			// If the timer is not blocked invoke the callback
			if (_timer[i].state == READY) _timer[i].callback(_timer[i].arg);
			
			if (_timer[i].periodic > 0) {
				
				// Recalculate expiry
				_timer[i].expiry += _timer[i].periodic;
				} else {
				// Disable Timer
				_timer[i].callback = NULL;
			}
		}
	}
	PORTC ^= 0xFE;
}


void TIMER_Init(void)
{
	//
	/*! 
	* \brief 	Initializes Timer1 (Scheduler)
	*
	* \return 	NULL
	*/	
    TCNT1 = 0x0000;

	// 12C0 == 600us, 960 = 300 us, 0x0DE0 = 444us, 0x06F0 = 222us, 0x0A68 = 333us, 0x0898 = 275 us, 0x848 = 265us
	
	// At 6/2 adc
    OCR1A = 0x0848;

	// Timer mode with no prescaling and CTC mode (reset counter on compare mode)   
    TCCR1A = 0x00;
    TCCR1B = (1<<CS10) | (1<<WGM12);

	// Enable timer1 output compare interrupt  
    TIMSK1 = (1 << OCIE1A) ;   
	
	// Allocate scheduler array
	memset(_timer, 0, sizeof(_timer));

	return;
} // TIMER_Init

int TIMER_Create(uint16_t timeout, int periodic, void (*callback)(void *), void *arg)
{
	//
	/*! 
	* \brief 	Creates a timer structure (Task Controller)
	*			
	* \param	Timer period in frame ticks (444us)
	*
	* \param	(0) for one-shot (1)
	*
	* \param	Timer callback function (the task)
	*
	* \param	Callback function parameter (NULL for nothing)
	*
	* \return	0 on fail, _timer handle on success
	*/	
	int handle = -1;
	size_t i;
	
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
				_timer[i].periodic = timeout;
			} 
			else 
			{
				_timer[i].periodic = 0;
			}
			
			_timer[i].callback = callback;
			_timer[i].arg = arg;
			_timer[i].expiry = timeout + _timer_tick;
			_timer[i].state = READY;
			
		}
		
	handle = i;	
	return handle;
	}
    return 0;
} // TIMER_Create

void Delay_Create(uint16_t timeout_ms)
{
	// A defunct debugging function 

	//delay_flag = TIMER_Create(timeout_ms,0,Do_Nothing,NULL);
	
	//while(delay_flag != -1);
} // Delay_Create
