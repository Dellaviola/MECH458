//########################################################################
//# MILESTONE : 2
//# PROGRAM : 2
//# PROJECT : Lab2
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : This program does Nightrider using the internal oscillators
//# DATA
//# REVISED 28-01-2019
//########################################################################

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "timer.h"
#include "config.h"
#include "blinky.h"
#include "adc.h"
#include "delay.h"
#include "stepper.h"
#include "pwm.h"




int main()
{	
	cli();
	stepper_handle = -1;
	timer_handle = -1;
	delay_flag = -1;
	int t = 1;
    //START setup functions
    DDRA = 0xFF;  // Sets all pins on Port A to output
    DDRB = 0xFF;  // Sets all pins on Port B to output for PWM [7]
    DDRC = 0xFF;  // Sets all pins on port C to output for LEDS
    DDRD = 0x00;  // Sets all pins on port D to inpput for buttons
    DDRF = 0x00;  // Sets all pins on Port F to input for ADC
    //PORTB = 0x00; // clear Port B
    PORTC = 0x00; // clear Port C
    //PORTD = 0x00; // clear Port D
	if (Timer_Init() != 0) PORTC |= TIMER_ERROR; //red leds error
	Stepper_Setup();
	pwmSetup();
	adcSetup();
	PORTB = ~(0x0F);	//brake right away
	
	sei();        // Enable global interrupts
	
	//10ms brakes dc
	
	while(1)
	{
		
		//PORTB = 0xFF;
		if (ADC_result_flag)
		{
			PORTC = ADC_result;     // Print ADC result to PORT C
			ADC_result_flag = 0x00; //Clear ADC flag
			pwm(ADC_result, 1);
			//Delay_Create(200);
			ADCSRA |= _BV(ADSC);    // Restart ADC on rising edge
			
		}
		if ((PIND & 0x03) == 0x03) // ON CCW
		{
			
			if ((PORTB & 0x0F) == ~0x0D)
			{
				PORTB = ~0x0F;
				Delay_Create(10);
			} // brake to vcc
			//PORTC = ~0x07;
			PORTB = ~0x07; // turn on, go to ccw
		} 
		else if ((PIND & 0x03) == 0x02) // ON CW
		{
			
			if ((PORTB & 0x0F) == ~0x07)
			{
				PORTB = ~0x0F;
				Delay_Create(10);
			} // brake to vcc
			//PORTC = ~0x0D;// 00001101
			PORTB = ~0x0D; // turn on, go to cw
		}
		else if ((PIND & 0x03) == 0x0) // BRAKE TIME
		{
			
			PORTB = ~0x0F; // brake to vcc
			//PORTC = ~0x0F;

		}
		else // BRAKE ANYWAY
		{
			PORTB = ~(0x0F); // brake to vcc
			//PORTC = ~0x0F;
		}
		
			
// 					
// 		 if (timer_handle < 0) 
// 		 {
// 			timer_handle = Timer_Create(1000,1,d_blinky,NULL);
// 		 }
		
		while(t)
		{
// 			Stepper(TURN_30, CW);
// 			Delay_Create(1000);
			Stepper(TURN_30, CW);
			Delay_Create(1000);
			Stepper(TURN_60, CW);
			Delay_Create(1000);	
			Stepper(TURN_90, CW);
			Delay_Create(1000);
			Stepper(TURN_180, CW);
			Delay_Create(1000);
			Stepper(TURN_30, CCW);
			Delay_Create(1000);
			Stepper(TURN_60, CCW);
			Delay_Create(1000);
			Stepper(TURN_90, CCW);
			Delay_Create(1000);
			Stepper(TURN_180, CCW);
			Delay_Create(1000);
// 			Stepper(BRAKE, CCW);
// 			Delay_Create(1000);
// 			Stepper(OFF, CCW);
// 			Delay_Create(1000);
			t = 0;
		}
	}
	return 0;
}

