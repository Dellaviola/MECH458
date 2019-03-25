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
	    g_ADCResultl = ADCL;
		g_ADCResulth = ADCH;
	    g_ADCFlag = 1;
	
}

volatile uint16_t ADC_Get(){

	return ((ADCH << 8) | ADCL );
	
}

inline void ADC_Start(void* arg){

	(void) arg;
	ADCSRA |= (1<<ADSC);
	PORTD = 0xF0;

}

void ADC_Stop(void* arg){
	(void) arg;
	ADCSRA &= (0<<ADEN);
	PORTD = 0x00;
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
