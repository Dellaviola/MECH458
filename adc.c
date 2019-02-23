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

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "timer.h"
#include "adc.h"

//################## MAIN ROUTINE ##################

void adcSetup()
{
    cli();
    //configure external interrupts
    EIMSK |= (_BV(INT2));               //enable INT2
    EICRA |= (_BV(ISC21) | _BV(ISC20)); //rising edge interrupt

    //ADC default input (analog input is set to be ADC0 / PORTF0
    ADCSRA |= _BV(ADEN);                // enable ADC
    ADCSRA |= _BV(ADIE);                // enable interrupt of ADC
    ADMUX |= (_BV(ADLAR) | _BV(REFS0)); // left adjust ADC result, use AVcc
    sei();
    ADCSRA |= _BV(ADSC); //Start ADC converions
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
