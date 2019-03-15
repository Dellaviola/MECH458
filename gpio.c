/*
        gpio.c

        2019-02-21

        Mario Dellaviola

        gpio
*/

#include "gpio.h"


int gpioInit{

    DDRA = 0xFF;  // Sets all pins on Port A to output
    DDRB = 0xFF;  // Sets all pins on Port B to output for PWM [7]
    DDRC = 0xFF;  // Sets all pins on port C to output for LEDS
    DDRD = 0x00;  // Sets all pins on port D to input for buttons
    DDRF = 0x00;  // Sets all pins on Port F to input for ADC

    

    return 0;
}