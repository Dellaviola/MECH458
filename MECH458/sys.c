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
	g_PauseRequest = 0;
	g_WDTimeout = 0;
	g_Timer = 0;
	g_Lock = 0;
	g_ItemInRange = 0;

	HEAD = NULL;
	TAIL = NULL;
	STAGE1 = NULL;
	STAGE2 = NULL;
	FRONT = NULL;
	BUFFER = NULL;

	// Initialize Item List
	itemNode* initNode = NULL;
	initNode = LL_ItemInit(65001,255, UNCLASSIFIED, UNINITIALIZED);
	
	// First Node
	HEAD = LL_ItemListInit(initNode);
	FRONT = HEAD;
	BUFFER = HEAD;

	// 48 total nodes for trial run
	for(int i = 0; i < 47; i++)
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

void SYS_Pause(const char str[20])
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
	
	sprintf(buffer, "Motor Tick: %u, HEADTick: %u\r\n",g_Timer, LL_GetTick(HEAD));
	UART_SendString(buffer);

	// Print List Information
	while (LL_GetClass(temp) != END_OF_LIST)
	{
		char listbuff[100];
		c++;
		sprintf(listbuff, "Item: %d, Refl: %u, Mag: %u, Class %u, Status: %u, Tick: %u\r\n",
					 c, LL_GetRefl(temp), LL_GetMag(temp), LL_GetClass(temp), LL_GetStatus(temp), LL_GetTick(temp));
		UART_SendString(listbuff);
		temp = LL_Next(temp);	
	}
	
	// Print Task Information
	for(int i = 0; i < 7; i++)
	{
		char statebuff[10];
		sprintf(statebuff, "\r\nTimer %d State: %u\r\n",i, _timer[i].state);
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

void SYS_Calibrate(char str[20])
{
	//
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
	//while (temp->prev) temp = LL_Prev(temp);
	
	UART_SendString(str);
	sprintf(buffer, "Motor Tick: %u\r\n",g_Timer);
	UART_SendString(buffer);
	
	while (LL_GetStatus(temp) != UNINITIALIZED)
	{
		char listbuff[100];
		c++;
		sprintf(listbuff, "Item: %d, Refl: %u, Mag: %u\r\n",
				c, LL_GetRefl(temp), LL_GetMag(temp));
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

void SYS_Rampdown()
{
	//
	uint8_t sortedStats[6] = {0,0,0,0,0,0};
	uint8_t total = 0;
	
	char str[50];
	
	list* temp = FRONT;
	
	cli();
	PORTD = 0;
	PORTC = 0;
	PWM(0);
	
	
	UART_SendString("\r\n\r\n\r\n\r\n\r\n\r\nSystem Ramping Down...\r\n");
	
	while(LL_GetStatus(temp) == EXPIRED)
	{	
		total++;
		sortedStats[LL_GetClass(temp)] += 1; 
		temp = LL_Next(temp);
	}
	
	sprintf(str,"%u Items Sorted!\r\n\r\nBlack: %u/12\tWhite: %u/12\tSteel: %u/12\tAluminum: %u/12\tUnknown Items: %u\r\n",
				total, sortedStats[1], sortedStats[0], sortedStats[3], sortedStats[2], sortedStats[4]);
	UART_SendString(str);
	//SYS_Calibrate("Get Calibration Stats:");
}
