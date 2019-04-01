/*
 * sys.c
 *
 * Created: 2019-03-27 12:50:48 PM
 *  Author: Mario
 */ 

#include "sys.h"

void SYS_Init()
{
	// Initialize system
	
		
	cli();
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	UART_Init();
	GPIO_Init();
	ADC_Init();
	STEPPER_Init();	
	TIMER_Init();
	PWM_Init();
	g_ADCCount = 0;
	memset(g_ADCResult, 0, sizeof(g_ADCResult));
	g_ADCFlag = 0;
	g_PauseRequest = 0;
	
// 	extern list* HEAD;
// 	extern list* STAGE1;
// 	extern list* STAGE2;
// 	extern list* TAIL;
//	extern list* FRONT;	
	itemNode* initNode = NULL;
	
	HEAD = NULL;
	TAIL = NULL;
	STAGE1 = NULL;
	STAGE2 = NULL;
	FRONT = NULL;

	
	initNode = LL_ItemInit(65001,255, UNCLASSIFIED, UNINITIALIZED);
	
	HEAD = LL_ItemListInit(initNode);
	FRONT = HEAD;
	for(int i = 0; i < 48; i++)
	{
		initNode = LL_ItemInit(65000 - i,250 - i, UNCLASSIFIED, UNINITIALIZED);
		TAIL = LL_AddBack(HEAD, initNode);
	}
	for(int j = 0; j < 5; j++)
	{
		initNode = LL_ItemInit(65000,250, END_OF_LIST, UNINITIALIZED);
		LL_AddBack(HEAD,initNode);
	}
	
	char temp[50];
	sprintf(temp,"%u\r\nHEAD: %x, TAIL: %x, FRONT: %x, END: %x\r\n", LL_Size(HEAD), HEAD, TAIL, FRONT, TAIL->next);
	UART_SendString(temp);
}

void SYS_Pause(char str[20])
{
	cli();
	PWM(0);
	char buffer[100];
	extern list* HEAD;
	extern list* STAGE1;
	extern list* STAGE2;
	extern list* TAIL;
	extern list* FRONT;
	list* temp = FRONT;
	int c = 0;
	
	sprintf(buffer,"System Pause Message: %s\r\n", str);
	UART_SendString(buffer);

	
	while (LL_GetClass(temp) != END_OF_LIST)
	{
		char listbuff[50];
		c++;
		sprintf(listbuff, "Item: %d, Refl: %u, Mag: %u, Class %u, Status: %u\r\n", c, LL_GetRefl(temp), LL_GetMag(temp), LL_GetClass(temp), LL_GetStatus(temp));
		UART_SendString(listbuff);
		temp = LL_Next(temp);	
	}
	
	for(int i = 0; i < 7; i++)
	{
		char statebuff[10];
		sprintf(statebuff, "Timer %d State: %u\r\n", i, _timer[i].state);
		UART_SendString(statebuff);
	}
	char anotherbuff[50];
	sprintf(anotherbuff,"%u\r\nHEAD: %x, TAIL: %x, FRONT: %x, END: %x, STAGE1: %x, Mag: %u STAGE2: %x, Refl: %u\r\n", LL_Size(HEAD), HEAD, TAIL, FRONT, TAIL->next, STAGE1, LL_GetMag(STAGE1), STAGE2, LL_GetRefl(STAGE2));
	UART_SendString(anotherbuff);
	while(1)
	{
		if((PIND & 0x03) == 0x00) // Both Buttons
		{
			UART_SendString("Starting System!\r\n");
			PWM(0x80);
			g_PauseRequest = 0;
			sei();
			break;
		}
	}
	return;
}

void SYS_Calibrate()
{
	//
	cli();
	PWM(0);
	//char buffer[100];
	extern list* HEAD;
	extern list* STAGE1;
	extern list* STAGE2;
	extern list* TAIL;
	extern list* FRONT;
	list* temp = FRONT;
	int c = 0;
	//while (temp->prev) temp = LL_Prev(temp);
	
	while (LL_GetStatus(temp) != UNINITIALIZED)
	{
		char listbuff[50];
		c++;
		sprintf(listbuff, "Item: %d, Refl: %u, Mag: %u, Class %u, Status: %u\r\n", c, LL_GetRefl(temp), LL_GetMag(temp), LL_GetClass(temp), LL_GetStatus(temp));
		UART_SendString(listbuff);
		temp = LL_Next(temp);
	}
	while(1)
	{
		if((PIND & 0x03) == 0x00) // Both Buttons
		{
			UART_SendString("Starting System!\r\n");
			PWM(0x80);
			g_PauseRequest = 0;
			sei();
			break;
		}
	}
	return;
}