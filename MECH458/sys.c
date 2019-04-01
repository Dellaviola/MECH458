/*
 * sys.c
 *
 * Created: 2019-03-27 12:50:48 PM
 *  Author: Mario
 */ 



/* Header */
#include "sys.h"

/*-----------------------------------------------------------*/

void SYS_Init()
{
	// 
	/*! 
	* \brief 	Initializes the system
	*/
	
	cli();
	
	// Change system to 8Mhz
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	
	// Initialize Middleware
	UART_Init();
	GPIO_Init();
	TIMER_Init();
	PWM_Init();
	ADC_Init();
	STEPPER_Init();
	
	// Intiialize Globals
	g_ADCCount = 0;
	memset(g_ADCResult, 0, sizeof(g_ADCResult));
	g_ADCFlag = 0;

	HEAD = NULL;
	TAIL = NULL;
	STAGE1 = NULL;
	STAGE2 = NULL;
	FRONT = NULL;

	// Initialize Item List
	itemNode* initNode = NULL;
	initNode = LL_ItemInit(65001,255, UNCLASSIFIED, UNINITIALIZED);
	
	// First Node
	HEAD = LL_ItemListInit(initNode);
	FRONT = HEAD;

	// 48 total nodes for trial run
	for(int i = 0; i < 47; i--)
	{
		initNode = LL_ItemInit(65000 - i,250 - i, UNCLASSIFIED, UNINITIALIZED);
		TAIL = LL_AddBack(HEAD, initNode);
	}
	// 5 node buffer for robustness
	for(int j = 0; j < 5; j++)
	{
		initNode = LL_ItemInit(65000,250, END_OF_LIST, UNINITIALIZED);
		LL_AddBack(HEAD,initNode);
	}

	UART_SendString("System Initialized...");

} // SYS_Init

void SYS_Pause(char str[20])
{
	// 
	/*! 
	* \brief 	Pause the system and assert dump
	*/

	// Turn off interrupts
	cli();

	// Turn off motor
	PWM(0);

	// Prepare for assert dump
	char buffer[100];
	extern list* HEAD;
	extern list* STAGE1;
	extern list* STAGE2;
	extern list* TAIL;
	extern list* FRONT;
	list* temp = FRONT;
	int c = 0;

	// Print Pause message
	sprintf(buffer,"System Pause Message: %s\r\n", str);
	UART_SendString(buffer);

	// Print List Information
	while (LL_GetClass(temp) != END_OF_LIST)
	{
		char listbuff[50];
		c++;
		sprintf(listbuff, "Item: %d, Refl: %u, Mag: %u, Class %u, Status: %u\r\n", c, LL_GetRefl(temp), LL_GetMag(temp), LL_GetClass(temp), LL_GetStatus(temp));
		UART_SendString(listbuff);
		temp = LL_Next(temp);	
	}
	
	// Print Task Information
	for(int i = 0; i < 7; i++)
	{
		char statebuff[10];
		sprintf(statebuff, "FROM: %s\r\nTimer %d State: %u\r\n",str, i, _timer[i].state);
		UART_SendString(statebuff);
	}

	// Print Current List Pointers
	char anotherbuff[50];
	sprintf(anotherbuff,"%u\r\nHEAD: %x, TAIL: %x, FRONT: %x, END: %x, STAGE1: %x, Mag: %u STAGE2: %x, Refl: %u\r\n", LL_Size(HEAD), HEAD, TAIL, FRONT, TAIL->next, STAGE1, LL_GetMag(STAGE1), STAGE2, LL_GetRefl(STAGE2));
	UART_SendString(anotherbuff);

	// Wait
	while(1)
	{
		// Both Buttons
		if((PIND & 0x03) == 0x00) 
		{
			UART_SendString("Starting System!\r\n");
			PWM(0x80);
			sei();
			return;
		}
	}
} // SYS_Pause

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
} // SYS_Calibrate