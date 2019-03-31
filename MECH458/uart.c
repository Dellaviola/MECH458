/*
        uart.c

        Created: 2019-03-05

        Mario Dellaviola

        uart
*/

/* Header */
#include "uart.h"

const uint32_t FOSC = 8000000;

/*-----------------------------------------------------------*/

void UART_Init()
{
	//
	/*! 
	* \brief 	Initializes UART
	*/	

	// BAUD 9600
	UBRR1H = (uint8_t)((((uint32_t)FOSC)/((uint32_t)9600*16)-1)>>8);
	UBRR1L = (uint8_t)(((uint32_t)FOSC)/((uint32_t)9600*16)-1) & 0x0ff;
	
	// Enable Transmit Receive
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);

}

void UART_SendChar(const char c)
{
	//
	/*! 
	* \brief 	Send a character
	*		
	* \param	a character
	*/	

	// Wait while UART is busy
	while ((UCSR1A & (1 << UDRE1)) == 0) {;}
	UDR1 = c;
	
}

void UART_SendString(const char* str)
{
	//
	/*! 
	* \brief 	Send a string
	*		
	* \param	a string
	*/

	// Send all char except \0
	uint16_t counter = 0;
	while(str[counter] != '\0')
	{
		UART_SendChar(s[counter]);
		counter++;
	}
}

uint8_t UART_Recieve ()
{	
	//
	/*! 
	* \brief 	Returns a byte received over uart 
	*		
	* \return	A byte
	*/	
	
	// Wait while UART is busy
    while ((UCSR1A & (1 << RXC1)) == 0) {;}
    return UDR1;

}