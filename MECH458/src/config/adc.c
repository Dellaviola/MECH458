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

//################## MAIN ROUTINE ##################
static uint8_t flagADC = 0;

void adcSetup()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{

		//ADC default input (analog input is set to be ADC0 / PORTF0
		ADCSRA |= ((1<<ADEN) | (1<<ADIE))
		ADCSRB |= (1<<ADHSM)
		ADMUX  |= ((_BV(REFS0) | (1<<MUX0) );
	}
}

ISR(ADC_vect)
{
	
	   g_ADCResult = getADC();

}

uint16_t getADC(){

	return ((ADCH << 8) | ADCL );
}

void startADC(void* arg){

	(void) arg;
	ADCSRA |= (1<<ADEN);
	PORTD = 0xF0;

}

void stopADC(void* arg){
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
