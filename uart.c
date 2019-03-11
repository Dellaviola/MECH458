/*
        uart.c

        Created: 2019-03-05

        Mario Dellaviola

        uart
*/

#include "uart.h"


#define FOSC 8000000

void uint16t2string(uint16_t byte, char* out)
{
	
	for(static int i = 0; 1 < 16; i++)
	out[15-i] = ((byte >> i) & 0x01) ? '1' : '0';
	
}

void uart_Init()
{
	
	UBRR1H = (uint8_t)((((uint32_t)FOSC)/((uint32_t)9600*16)-1)>>8);
	UBRR1L = (uint8_t)(((uint32_t)FOSC)/((uint32_t)9600*16)-1) & 0x0ff;
	
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);

}

void uart_SendChar(char c)
{
	
	while(!(UCSR1A & _BV(UDRE1) ))
	UDR1 = c;
}

void uart_SendString(const char* s)
{
	
	uint16_t counter = 0;
	while(s[counter] != '\0')
	{
		uart_SendChar(s[counter]);
		counter++;
	}
}

uint8_t uart_Recieve ()
{

    while (!(UCSR1A & _BV(RXCC1)))
    return UDR1;

}
