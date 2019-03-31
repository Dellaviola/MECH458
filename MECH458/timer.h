/*
        timer.h
        2019-02-21
        Mario Dellaviola
        timer prototypes and definitions.
*/

#ifndef TIMER_H_
#define TIMER_H_

/* Standard Includes */
#include <stdlib.h>        
#include <stdint.h>

/* AVR Includesd */
#include <avr/io.h>      
#include <avr/interrupt.h> 

/* Program Includes */
#include "config.h"
#include "string.h"

/*-----------------------------------------------------------*/

typedef enum eTaskState 
{
	BLOCKED,
	RUNNING,
	READY,
	SUSPENDED
}eTaskState;

/*-----------------------------------------------------------*/

typedef struct timer
{
    uint16_t expiry;
    uint16_t periodic;
    void (*callback)(void *);
    void* arg;
	eTaskState state;
}timer;

/*-----------------------------------------------------------*/

void TIMER_Init(void);
int TIMER_Create(uint16_t, int, void (*callback)(void *) , void *);
int TIMER_Delete(int);
void Delay_Create(uint16_t);

#endif // TIMER_H_