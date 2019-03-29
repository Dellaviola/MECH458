/*
 * linkedlist.h
 *
 * Created: 2019-03-18 11:15:14 AM
 *  Author: Mario
 */ 
#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <stdint.h>
#include <avr/io.h>        // the header of i/o port
#include <util/atomic.h>    // atomic blocks to handle blocking tasks
#include <avr/interrupt.h> // Delay functions for AT90USBKey
#include "config.h"

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef enum classification_e
{
	WHITE,
	BLACK,
	ALUMINUM,
	STEEL,
	UNCLASSIFIED,
	END_OF_LIST
}eclassification;

typedef enum status_e
{
	UNINITIALIZED,
	INITIALIZED,
	EXPIRED	
}estatus;

typedef struct itemNode_s
{
	uint16_t reflect;
	uint8_t magnet;
	eclassification class;
	estatus status;
		
}itemNode;

typedef struct timerNode_s
{
	uint8_t priority;
	uint16_t expiry;
	uint16_t periodic;
	void (*callback)(void *);
	void* arg;
}timerNode;

typedef union listNode_u
{
	struct timerNode_s timerNode;
	struct itemNode_s itemNode;
}listNode;

typedef struct list_s
{
	void* node;
	struct list_s* next;
	struct list_s* prev;
}list;

list* gScheduler;
list* HEAD;
list* TAIL;
list* STAGE1;
list* STAGE2;

itemNode* LL_ItemInit(uint16_t, uint8_t, eclassification, estatus);
timerNode* LL_TimerInit(uint16_t, int, void (*callback)(void *), void *, uint8_t);
list* LL_ItemListInit(void *);
list* LL_TimerListInit(void*);
void LL_Delete(list*);
list* LL_Next(list*);
list* LL_Prev(list*);
list* LL_PopFront(list*);
list* LL_AddBack(list*, void *);
list* LL_AddSorted(list*, void *);
list* LL_Insert(list*, void *);
list* LL_Remove(list*);
list* LL_Head(list*);
uint8_t LL_Size(list*);

eclassification LL_GetClass(list*);
estatus LL_GetStatus(list*);
uint16_t LL_GetRefl(list*);
uint8_t LL_GetMag(list*);

uint8_t LL_GetPriority(list*);
uint8_t LL_IsPeriodic(list*);
uint16_t LL_GetExpiry(list*);
void LL_CallCallback(list*);
void* LL_GetArg(list*);

void LL_UpdateExpiry(list*, uint16_t);
void LL_UpdatePriority(list*, uint8_t);
void LL_UpdatePeriodic(list*, uint8_t);

void LL_UpdateRefl(list*, uint16_t);
void LL_UpdateMag(list*, uint8_t);
void LL_UpdateClass(list*,eclassification);
void LL_UpdateStatus(list*, estatus);


#endif /* LINKEDLIST_H_ */