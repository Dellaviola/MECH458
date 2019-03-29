/*
        gpio.c

        2019-02-21

        Mario Dellaviola

        gpio
*/

#include "gpio.h"
#include "timer.h"
#include "sys.h"


int GPIO_Init(void){

    DDRA = 0xFF;  // Sets all pins on Port A to output
    DDRB = 0xFF;  // Sets all pins on Port B to output for PWM [7]
    DDRC = 0xFF;  // Sets all pins on port C to output for LEDS
    DDRD = 0xF0;  // Sets all pins on port D to input for buttons
	DDRE = 0x00;
    DDRF = 0x00;  // Sets all pins on Port F to input for ADC

// 	EICRB |= ((1 << ISC71) | (1 << ISC70)
// 			| (1 << ISC60)
// 			| (1 << ISC51));
// 			
// 	EIMSK |= ((1 << INT7) | (1 << INT6) | (1 << 5));

    return 0;
}

// ISR(INT7_vect)
// {
// 	// STAGE 1
// 	if((PINE & 0x80) == 0) _timer[4].state = READY;
// 	EIFR |= (1 << INTF7);
// 	
// 	//SYS_Pause("ISR7");
// }
// ISR(INT6_vect)
// {
// 	// STAGE 2
// 	// UNBLOCK ADC
// 	if ((PINE & 0x60) == 0)
// 	{
// 		ADCSRA |= (1 << ADEN);
// 		EIFR |= (1 << INTF6);
// 	} 
// 	else if ((PINE & 0x60) == 0x60)
// 	{
// 		ADCSRA &= (0 << ADEN);
// 		EIFR |= (1 << INTF6);
// 	} 
// 	//SYS_Pause("ISR6");
// }
// ISR(INT5_vect)
// {
// 	// STAGE 3
// 	
// 	// Check position flags
// 	// Disable DC motor
// 	
// 	_timer[3].state = READY;
// 	EIFR |= (1 << INTF7);
// 	//SYS_Pause("ISR5");
// 	
// }
// 

