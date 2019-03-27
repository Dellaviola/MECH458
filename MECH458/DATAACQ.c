/*
 * CFile1.c
 *
 * Created: 2019-03-19 1:32:12 PM
 *  Author: Mario
 */ 

#include "DATAACQ.h"

void DATAACQ()
{
	
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
		} else {
		PORTB = ~0x0E;
		}
		if (g_ADCFlag && ((PINE & 0x40) == 0x40))
		{
			memory = 1;
			g_ADCFlag = 0x00; //Clear ADC flag
			data[i++] = g_ADCResult;
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
	}
	Delay_Create(8000);
	UART_SendString("end\r\n");

}