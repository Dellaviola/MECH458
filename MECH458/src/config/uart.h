/*
        uart.h

        Created: 2019-03-05

        Mario Dellaviola

        uart
*/

#ifndef UART_H_
#define UART_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"

#define uartSendString() uPrint()

void uartInit(void);
void uartSendChar(char);
void uartSendString(const char*);
uint8_t uartRecieve(void);

#endif