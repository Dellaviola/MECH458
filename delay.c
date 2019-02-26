//########################################################################
//# MILESTONE : 2
//# PROGRAM : 2
//# PROJECT : Lab2
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : This is the timer functions from lab 2 migrated to a seperate library
//# DATA
//# REVISED 28-01-2019
//########################################################################

//ms timer function using the internal oscillators, Count is the desired # ms

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <avr/io.h>        // the header of i/o port
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "delay.h"

void mTimer(int count)
{
    int i = 0;                    //loop counter
	TCCR1B |= _BV(CS10);
    TCCR1B |= _BV(WGM12);         // Set to Waveform Generator Mode 12 (Clear on Timer Compare)
    OCR1B = 0x03E8;               // Set output compare register for 1000 cycles(1ms)
    TCNT1 = 0x0000;               // Set initalal counter timer time to 0
   // TIMSK1 = TIMSK1 | 0b00000010; // Enable output compare interrupt enable
    TIFR1 |= _BV(OCF1A);          // Clear interrupt flag and start timer

    //TODO: switch this to a real interrupt
    while (i < count)
    {
        //When the interrups flag is triggered
        if ((TIFR1 & 0x02) == 0x02)
        {
            TIFR1 |= _BV(OCF1A); //Clear the flag
            i++;                 //increment the loop
        }
    }
    return;
}
