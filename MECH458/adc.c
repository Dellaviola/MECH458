/*
        adc.c

        2019-02-21

        Mario Dellaviola

        adc configuration
*/

/* Header */
#include "adc.h"

/*-----------------------------------------------------------*/

void ADC_Init()
{
	//
	// High Speed, Enable ADC & Interrupts
	ADCSRB |= (1 << ADHSM);			  
	ADCSRA |= (1 << ADEN);                
	ADCSRA |= (1 << ADIE);                

	// Input Pin F1
	ADMUX |=  ((1 << REFS0) | (1 << MUX0)); 

	// Startup conversion (throw away)
	ADCSRA |= _BV(ADSC);
}

/*-----------------------------------------------------------*/

ISR(ADC_vect)
{
	//
	// Take 10 samples	
	PORTC ^= 0x08;
	if (g_ADCCount < 6)
	{
			g_ADCResult[g_ADCCount++] = ADC;
			ADCSRA |= (1 << ADSC);	
			if (g_ADCCount == 6) _timer[1].state = READY;
	}

	// Unblock ADC task
	
	PORTC ^= 0x08;
}

