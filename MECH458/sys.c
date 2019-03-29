/*
 * sys.c
 *
 * Created: 2019-03-27 12:50:48 PM
 *  Author: Mario
 */ 

#include "sys.h"
#include "timer.h"

void SYS_Init()
{
	// Initialize system
	
	cli();
	
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	
	UART_Init();
	GPIO_Init();
	TIMER_Init();
	PWM_Init();
	ADC_Init();
	STEPPER_Init();
	
	stepper_handle = -1;
	timer_handle = -1;
	delay_flag = -1;
	
	g_ADCCount = 0;
	memset(g_ADCResult, 0, sizeof(g_ADCResult));
	g_ADCFlag = 0;
	
	HEAD = NULL;
	TAIL = NULL;
	STAGE1 = NULL;
	STAGE2 = NULL;
	
	listNode* initNode = LL_ItemInit(65000,250,UNCLASSIFIED,UNINITIALIZED);
	
	HEAD = LL_ItemListInit(initNode);
	
	for(int i = 47; i > 0; i--)
	{
		TAIL = LL_AddBack(HEAD, initNode);
	}
	
	initNode = LL_ItemInit(65000,250, END_OF_LIST, UNINITIALIZED);
	LL_AddBack(HEAD,initNode);
	
	//PORTC = LL_Size(HEAD);
	char temp[5];
	sprintf(temp,"%u\r\n", LL_Size(HEAD));
	UART_SendString(temp);
	//sei();

}

void SYS_Pause(char str[20])
{
	cli();
	PWM(0);
	char buffer[100];
	if(HEAD) sprintf(buffer, "%s: Item Refl: %u, Item Mag: %u, Item Class: %u\r\n", str, LL_GetRefl(HEAD), LL_GetMag(HEAD), LL_GetClass(HEAD));
	if(HEAD) UART_SendString(buffer);
	if(STAGE1) sprintf(buffer, "%s: Item Refl: %u, Item Mag: %u, Item Class: %u\r\n", str, LL_GetRefl(STAGE1), LL_GetMag(STAGE1), LL_GetClass(STAGE1));
	if(STAGE1) UART_SendString(buffer);
	if(STAGE2) sprintf(buffer, "%s: Item Refl: %u, Item Mag: %u, Item Class: %u\r\n", str, LL_GetRefl(STAGE2), LL_GetMag(STAGE2), LL_GetClass(STAGE2));
	if(STAGE2) UART_SendString(buffer);
	if(TAIL) sprintf(buffer, "%s: Item Refl: %u, Item Mag: %u, Item Class: %u\r\n", str, LL_GetRefl(TAIL), LL_GetMag(TAIL), LL_GetClass(TAIL));
	if(TAIL) UART_SendString(buffer);
	
	for(int i = 0; i < 7; i++)
	{
		char statebuff[10];
		sprintf(statebuff, "Timer %d State: %u\r\n", i, _timer[i].state);
		UART_SendString(statebuff);
	}
	while(1)
	{
		if((PIND & 0x03) == 0x00) // Both Buttons
		{
			UART_SendString("Starting System!\r\n");
			PWM(0x80);
			sei();
			break;
		}
	}
	return;
}