/*
        uart.c

        Created: 2019-03-05

        Mario Dellaviola

        uart
*/

#include "uart.h"

#define FOSC 8000000

void UART_Init()
{
	
	UBRR1H = (uint8_t)((((uint32_t)FOSC)/((uint32_t)9600*16)-1)>>8);
	UBRR1L = (uint8_t)(((uint32_t)FOSC)/((uint32_t)9600*16)-1) & 0x0ff;
	
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);

}



void UART_SendChar(const char c)
{
	
	while ((UCSR1A & (1 << UDRE1)) == 0) {;}
	UDR1 = c;
	
}

void UART_SendString(const char* s)
{
	
	uint16_t counter = 0;
	while(s[counter] != '\0')
	{
		UART_SendChar(s[counter]);
		counter++;
	}
}

uint8_t UART_Recieve ()
{

    while ((UCSR1A & (1 << RXC1)) == 0) {;}
    return UDR1;

}