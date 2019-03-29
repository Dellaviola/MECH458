//########################################################################
//# MILESTONE : 4
//# PROGRAM : 4
//# PROJECT : Lab4
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : Lab 4 ADC
//# DATA
//# REVISED 11-02-2019
//########################################################################

// #include <stdlib.h>        // the header of the general purpose standard library of C programming language
// #include <stdint.h>
// #include <avr/io.h>        // the header of i/o port
// #include <util/atomic.h>    // atomic blocks to handle blocking tasks
// #include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
// #include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"
#include "adc.h"
#include "uart.h"
#include "string.h"
#include "timer.h"

//################## MAIN ROUTINE ##################
void ADC_Init()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{

		//ADC default input (analog input is set to be ADC0 / PORTF0
		
 		ADCSRB |= (1<<ADHSM);
		ADCSRA |= _BV(ADEN);                // enable ADC
		ADCSRA |= _BV(ADIE);                // enable interrupt of ADC
		ADMUX |=  ((1<<REFS0) | (1<<MUX0)); // left adjust ADC result, use AVcc
		ADCSRA |= _BV(ADSC); //Start ADC converions
	}
}

ISR(ADC_vect)
{
		PORTC ^= 0xFE;
		if (g_ADCCount < 10)
		{
			 g_ADCResult[g_ADCCount++] = ADC;
			 ADCSRA |= (1 << ADSC);
			 
		}
		if (g_ADCCount == 10) _timer[1].state = READY;
		PORTC ^= 0xFE;
}

// Function for polling ADC
// void adcCheck
// {
//     if (ADC_result_flag)
//     {
//         PORTC = ADC_result;     // Print ADC resuult to PORT C
//         ADC_result_flag = 0x00; //Clear ADC flag
//         ADCSRA |= _BV(ADSC);    // Restart ADC on rising edge
//     }
// }
