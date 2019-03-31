/*
        uart.h

        Created: 2019-03-05

        Mario Dellaviola

        uart
*/

#ifndef UART_H_
#define UART_H_

/* Standard Includes */
#include <stdlib.h>        
#include <stdint.h>
#include <stdio.h>

/* AVR Includes */
#include <avr/io.h>      
#include <avr/interrupt.h> 

/* Program Includes */
#include "config.h"


void UART_Init(void);
void UART_SendChar(const char);
void UART_SendString(const char*);
uint8_t UART_Recieve(void);

#endif // UART_H_