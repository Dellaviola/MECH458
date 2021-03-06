/*
 * LISTTEST.c
 *
 * Created: 2019-03-25 1:30:50 PM
 *  Author: Mario
 */ 

#include "LISTTEST.h"

void LISTTEST()
{
	
	cli();
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	UART_Init();
	GPIO_Init();
	TIMER_Init();
	PWM_Init();
	ADC_Init();
	
	
	sei();
	
	
	UART_SendString("Testing List\r\n");
	PORTC = 0xF0;
	itemNode* newItem = LL_ItemInit(100,100,WHITE,INITIALIZED);
	list* listIndex = LL_ItemListInit(newItem);
	list* timerList = NULL;
	list* head = listIndex;
	
	newItem = LL_ItemInit(100,100,BLACK,INITIALIZED);
	LL_AddBack(listIndex, newItem);
	newItem = LL_ItemInit(100,100,ALUMINUM,INITIALIZED);
	LL_AddBack(listIndex, newItem);
	newItem = LL_ItemInit(100,100,STEEL,INITIALIZED);
	LL_AddBack(listIndex, newItem);
	
	head = LL_Remove(LL_Head(listIndex));
	
	timerNode* newTimer = LL_TimerInit(100,100,Say_Hello, NULL, 0);
	timerList = LL_TimerListInit(newTimer);

	//((timerNode*)timerList->node)->callback(NULL);
	LL_CallCallback(timerList);
	LL_UpdateRefl(head,200);
	
	
	char send[32];
//	UART_SendString("Expecting Size 3, Class Black (1)\r\n");
	
	sprintf(send,"Size: %d\r\nClass: %d\r\nReflect: %d \r\n",LL_Size(head),LL_GetClass(head), LL_GetRefl(head));
	UART_SendString(send);
	
	
	return;
}