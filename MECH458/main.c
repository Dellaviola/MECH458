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
#include "gpio.h"
#include "uart.h"
#include "DATAACQ.h"

#define DATAMODE 1


int main(void)
{	
	
#if DATAMODE == 1
	DATAACQ();
	return 0;
#endif
	cli();
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	UART_Init();
	GPIO_Init();
	stepper_handle = -1;
	timer_handle = -1;
	delay_flag = -1;
	if (Timer_Init() != 0) UART_SendString("TIMER DID NOT INITIALIZE"); //red leds error
	//Stepper_Setup();
	PWM_Init();
	ADC_Init();
	
	sei();        // Enable global interrupts	
	uint16_t data[1500];
	char datao[10];
	int memory = 2;
	size_t i = 0;
	while(memory)
	{
		
	 	if ((PINE & 0x40) == 0x40) PORTC = 0xF0;
		if ((PINE & 0x40) == 0x00) PORTC = 0x0F;
		if((PIND & 0x01) == 0){
			PORTB = ~0x0F;
			//PORTC = ~0x0F;
			} else {
			PORTB = ~0x0E;
			//PORTC = ~0x07;
		}
		if (g_ADCFlag && ((PINE & 0x40) == 0x40))
		{	
			memory = 1;
			//PORTC = (g_ADCResultl);     // Print ADC result to PORT C
			g_ADCFlag = 0x00; //Clear ADC flag
			//Delay_Create(200);
			    // Restart ADC on rising edge
			data[i++] = ((g_ADCResulth<<8) | g_ADCResultl);

			ADCSRA |= _BV(ADSC);
			
		}
		if ((memory == 1) && ((PINE & 0x40) == 0) && ((PIND & 0x01) == 0x01)){
			PORTB = ~0x0f;
			for (i = 0; i < sizeof(data); i++){
				if (data[i] < 1000) {
					sprintf(datao,"%d\r\n",data[i]);
					UART_SendString(datao);
				}
			}
			i = 0;
			memory = 0;
		}

// 		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
// 			temp = getADC();
// 		}
		//g_ADCResult = 0;
// 		if (g_ADCFlag) {
// 			startADC(NULL);
// 			g_ADCFlag = 0;
// 		}
// 		sprintf(data,"%d\n",g_ADCResult);
// 		uartSendString(data);
		


	}
	Delay_Create(8000);
	UART_SendString("end\r\n");
	return 0;
}

ISR(BADISR_vect)
{
	while(1)
	{
		Timer_Create(4000, 1, C_Blinky, NULL);
	}
}
