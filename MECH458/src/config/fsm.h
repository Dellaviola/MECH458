/*
        fsm.h

        Created: 2019-03-05

        Mario Dellaviola

        fsm
*/

#ifndef FSM_H_
#define FSM_H_

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/wdt.h>        // watchdog macros reset MCU on hangs.
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"

typedef enum
{
    State_1,
    State_2,
    State_3
} eStates;

typedef enum
{
    Event_1,
    Event_2,
    Event_3
} eEvents;

typedef eSystemEvent(*pfEventHandler)(void);

typedef struct
{
    eStates eStateMachine;
    eEvents eStateMachineEvent;
    pfEventHandler pfStateMachineEventHandler;
} sStateMachine;

eStates Event_1_Handler(void);
eStates Event_2_Handler(void);
eStates Event_3_Handler(void);

sStateMachine FSM_Init(void);
#endif UART_H_