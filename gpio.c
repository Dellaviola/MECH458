/*
        gpio.c

        2019-02-21

        Mario Dellaviola

        gpio
*/

#include "gpio.h"


int gpioInit(void){

    DDRA = 0xFF;  // Sets all pins on Port A to output
    DDRB = 0xFF;  // Sets all pins on Port B to output for PWM [7]
    DDRC = 0xFF;  // Sets all pins on port C to output for LEDS
    DDRD = 0xF0;  // Sets all pins on port D to input for buttons
	DDRE = 0x00;
    DDRF = 0x00;  // Sets all pins on Port F to input for ADC



    return 0;
}

int optHandler_1(void)
{
    return (OPTICAL_1_PIN & PIN6) ? 0 : 1;
}