/*
 * TIMERUNITTEST.c
 *
 * Created: 2019-03-25 5:40:30 PM
 *  Author: Mario
 */ 

/*
 * LISTTEST.c
 *
 * Created: 2019-03-25 1:30:50 PM
 *  Author: Mario
 */ 

#include "TIMERTEST.h"

void TIMERTEST()
{
	
	cli();
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	UART_Init();
	GPIO_Init();
	stepper_handle = -1;
	timer_handle = -1;
	delay_flag = -1;
	if (Timer_Init() != 0) UART_SendString("TIMER DID NOT INITIALIZE"); //red leds error
	//Stepper_Setup();
	PWM_Init();
	ADC_Init();	
	sei();
	
	UART_SendString("Testing Timer\r\n");

	//list* gScheduler = NULL;
	
	Timer_Create(8000,1,C_Blinky,NULL);
	
	//TIMER_Dispatch(gScheduler);
	
	while(1)
	{
	
	}
// 	char send[32];
// //	UART_SendString("Expecting Size 3, Class Black (1)\r\n");
// 	
// 	sprintf(send,"Size: %d\r\nClass: %d\r\nReflect: %d \r\n",LL_Size(head),LL_GetClass(head), LL_GetRefl(head));
// 	UART_SendString(send);
	
	
	return;
}