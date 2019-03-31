/*
        gpio.c

        2019-02-21

        Mario Dellaviola

        gpio
        commented code implements interrupts for optical sensors
*/

/* Header */
#include "gpio.h"


//#include "timer.h"
//#include "sys.h"

/*-----------------------------------------------------------*/

int GPIO_Init(void)
{

    DDRA = 0xFF;  // Sets all pins on Port A to output
    DDRB = 0xFF;  // Sets all pins on Port B to output for PWM [7]
    DDRC = 0xFF;  // Sets all pins on port C to output for LEDS
    DDRD = 0xF0;  // Sets all pins on port D to input for buttons
	DDRE = 0x00;
    DDRF = 0x00;  // Sets all pins on Port F to input for ADC

// 	EICRB |= ((1 << ISC70)					// O1 Both
// 			| (1 << ISC61) | (1 << ISC60)	// 02 Rising			
// 			| (1 << ISC51));				// 03 Falling
// 			
// 	EIMSK |= ((1 << INT7) | (1 << INT6) | (1 << INT5));

    return 0;
} // GPIO_Init

// ISR(INT7_vect)
// {
// 	// STAGE 1
// 	if((PINE & 0x80) == 0) // Falling edge
// 	{
// 		// _timer[4].state = READY;
// 		if(STAGE1 == NULL) STAGE1 = HEAD;
// 	}
// 	else if((PINE & 0x80) == 0x80) // Rising Edge
// 	{
// 		_timer[2].state = READY;
// 	}
// 	//EIFR |= (1 << INTF7);
// 	
// 	//SYS_Pause("ISR7");
// }
// ISR(INT6_vect)
// {
// 	// STAGE 2
// 	// UNBLOCK ADC
// 	if((PINE & 0x40) == 0)
// 	{
// 		if(STAGE2 == NULL)
// 		{
// 			STAGE2 = HEAD;
// 		}
// 		else
// 		{	
// 			STAGE2 = LL_Next(STAGE2);
// 		}
// 		//ADCSRA |= (1 << ADSC);
// 	}
// 	
// 	//SYS_Pause("ISR6");
// }
// ISR(INT5_vect)
// {
// 	// STAGE 3
// 	
// 	// Check position flags
// 	// Disable DC motor
// 	if((PINE & 0x20) == 0)
// 	{	
// 		_timer[3].state = READY;
// 		SYS_Pause("ISR5");
// 	}
// 	//EIFR |= (1 << INTF5);
// 	//SYS_Pause("ISR5");
// 	
// }


