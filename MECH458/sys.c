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
	Timer_Init();
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
	
	gSysCalibrated = 0;
	
	sei();

}