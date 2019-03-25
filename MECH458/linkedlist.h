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

typedef struct itemContainer_s{
	
	uint8_t reflect;
	uint8_t magnet;
	ItemClass_e class;
		
}itemContainer;

typedef struct timerContainer_s{
	
	uint16_t expiry;
	uint8_t periodic;
	void (*callback)(void *);
	void* arg;
	
}timerContainer;

typedef struct listNode_s{

	struct container;
	struct listNode_s * next;
	uint8_t sorted;
	
}listNode;

typedef struct list_s{
	listNode * head;
	listNode * tail;
	listNode * stage;
	uint8_t size;
}list;
	
list LL_CTor(void);
listNode * LL_Container(struct container);
list LL_AddSorted(const list, const listNode);
listNode * LL_Remove(listNode );
list LL_Delete(list);

#endif /* LINKEDLIST_H_ */