/*
 * CFile1.c
 *
 * Created: 2019-03-18 11:14:30 AM
 *  Author: Mario
 */ 
#include "linkedlist.h"
#include "sys.h"

itemNode* LL_ItemInit(uint16_t reflVal, uint8_t magVal, classification class)
{
	//
	itemNode* newItem = malloc(sizeof(itemNode));
	newItem->reflect = reflVal;
	newItem->magnet = magVal;
	newItem->class = class;
	return newItem;
}
timerNode* LL_TimerInit(uint16_t timeout_ms, int periodic, void (*callback)(void *), void *arg, uint8_t priority)
{
	//
	timerNode* newItem = malloc(sizeof(timerNode));
	extern volatile uint16_t gTimerTick;
	
	newItem->arg = arg;
	newItem->callback = callback;
	newItem->expiry = timeout_ms + gTimerTick;
	newItem->periodic = (periodic) ? timeout_ms : 0;
	newItem->priority = priority;
	return newItem;
}
list* LL_ItemListInit(void* newNode)
{
	//
	list* newList = (list*)malloc(sizeof(list));
	
	newList->node = (itemNode*)newNode;
	newList->prev = NULL;
	newList->next = NULL;
	return newList;
}
list* LL_TimerListInit(void* newNode)
{
	//
	list* newList = (list*)malloc(sizeof(list));
		
	newList->node = (timerNode*)newNode;
	newList->prev = NULL;
	newList->next = NULL;
	return newList;
}
void LL_Delete(list* ref)
{
	//
	while((ref = LL_Remove(ref)) != NULL);
}
list* LL_Next(list* ref)
{
	//
	return ref->next;
}
list* LL_Prev(list* ref)
{
	//
	return ref->prev;
}
list* LL_AddBack(list* ref, void* newNode)
{
	//
	list* tail = ref;
	while(tail->next) tail = LL_Next(tail);
	
	list* newList = LL_ItemListInit(newNode);
	tail->next = newList;
	
	//SYS_Pause(__FUNCTION__);
	
	return newList;
}
list* LL_AddSorted(list* ref, void* newNode)
{
	//
		list* head = ref;
		while((head->prev) && (((timerNode*)head->node)->priority > ((timerNode*)newNode)->priority)) head = LL_Prev(head);
		
		list* newList = LL_TimerListInit(newNode);
		head->next = newList;
		
		return newList;
}

list* LL_Remove(list* ref)
{
	//
	list* tempnext = LL_Next(ref);
	list* tempprev = LL_Prev(ref);
	
	
	free(ref->node);
	free(ref);
	if (tempnext) tempnext->prev = tempprev;
	if (tempprev) tempprev->next = tempnext;
	
	return tempnext;
}
list* LL_Head(list* ref)
{
	while(ref->prev) ref = LL_Prev(ref);
	return ref;
}
uint8_t LL_Size(list* ref)
{
	//
	uint8_t tempsize = 1;
	list* head = ref;
	while((head = LL_Next(head)) != NULL) tempsize++;
	return tempsize;
}

classification LL_GetClass(list* ref)
{
	//
	return ((itemNode*)ref->node)->class;	
}

uint16_t LL_GetRefl(list* ref)
{
	//
	return ((itemNode*)ref->node)->reflect;
}

uint8_t LL_GetMag(list* ref)
{
	//
	return ((itemNode*)ref->node)->magnet;
}

uint8_t LL_GetPriority(list* ref)
{
	//
	return ((timerNode*)ref->node)->priority;
}
uint8_t LL_IsPeriodic(list* ref)
{
	return ((timerNode*)ref->node)->periodic;
}
uint16_t LL_GetExpiry(list* ref)
{
	//
	return ((timerNode*)ref->node)->expiry;
}

void LL_CallCallback(list* ref)
{
	//
	void* arg = ((timerNode*)ref->node)->arg;
	
	((timerNode*)ref->node)->callback(arg);
	return;
}

void* LL_GetArg(list* ref)
{
	//
	return ((timerNode*)ref->node)->arg;
}

void LL_UpdateRefl(list* ref, uint16_t newRefl)
{
	//
	((itemNode*)ref->node)->reflect = newRefl;
	return;
}
void LL_UpdateMag(list* ref, uint8_t newMag)
{
	//
	((itemNode*)ref->node)->magnet = newMag;
	return;
}
void LL_UpdateClass(list* ref, classification newClass)
{
	//
	((itemNode*)ref->node)->class = newClass;
	return;
}
void LL_UpdateExpiry(list* ref, uint16_t newExpiry)
{
	//
	((timerNode*)ref->node)->expiry = newExpiry;
	return;
}
void LL_UpdatePriority(list* ref, uint8_t newPriority)
{
	//
	((timerNode*)ref->node)->priority = newPriority;
	return;
}
void LL_UpdatePeriodic(list* ref, uint8_t newPeriodic)
{
	//
	((timerNode*)ref->node)->periodic = newPeriodic;
	return;
}