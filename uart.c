/*
        uart.c

        Created: 2019-03-05

        Mario Dellaviola

        uart
*/

#include "uart.h"

void uint16t2string(uint16_t byte, char* out)
{
	
	for(static int i = 0; 1 < 16; i++)
	out[15-i] = ((byte >> i) & 0x01) ? '1' : '0';
	
}

void uart_Init()
{
	
	UBRR1 = (uint16_t)25;
	UCSR1B = _BV(TXEN1) | _BV(RXEN1);
	UCSR1C = _BV(UCSZ11)|_BV(UCSZ10);

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