/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include <dc48_UART_Dimmers.h>
#include "stm32h523xx.h"
#include "dc48_Globals.h"
#include "dc48_TaskMgr.h"
#include "dc48_Initialization.h"
#include "dc48_Buttons.h"
#include "dc48_Display.h"
#include "dc48_ButtonFnc.h"
#include "dc48_ProcessData.h"
#include "dc48_UART_XLR.h"
#include "dc48_Rdm.h"
#include "dc48_DimmersCtrl.h"
#include "dc48_Flash.h"
#include "dc48_DimmersCom.h"
#include "dc48_UART_Ethernet.h"
#include "dc48_Ethernet.h"
#include "dc48_Tools.h"
#include "dc48_Adc.h"

int Interval_SysTick_Elapsed;

void ButtonsHandle(void)
{
	Scan_Buttons();
	Evaluate_Buttons_Actions();
}

void SysTickIntervalHandle(void)
{
    Interval_SysTick_Elapsed = 0;
    Task_DecrementCounters();
}

/* 100 us interval */

void SysTick_Handler(void)
{
	Interval_SysTick_Elapsed = 1;
	CircularCounter100us++;
//	if (LED_BlueCtc)
//	{
//		LED_BlueCtc--;
//		LED_DEBUG_BLUE_ON;
//	} else LED_DEBUG_BLUE_OFF;
	GPIOB->ODR ^= 0xF000;
}


/* 2 ms interval */

void Cyclic500Hz(void)
{
	ADC_Job();
    Fan_CheckRotaion();
}

void I_Am_Alive(void)
{
//	LED_DEBUG_RED_TGL;
}

int main(void)
{
	iniSystem();				// Clock, FLASH, CPU...
	iniPorts();
	iniSysTick();
	Task_EmptyAllTasks();
	Display_Initialization();
	iniVariables();
	ProcessDataInit();
	initializationUartXLR();
	Initialization_RDM();
	DimmersCtrlInitialization();
	DimmersCommInitialization();
	Initialization_Uart_ETHETNET();
	Initialization_ETHERNET_SSI();
	Fan_Initialization();
	initAdc();

	/* TASK INITIALIZATION */
	Task_Create_New(TASK_HANDLE_FREQUENCY(1),	TSK_TYPE_CYCLIC_TASK,	I_Am_Alive);
	Task_Create_New(TASK_HANDLE_FREQUENCY(100), TSK_TYPE_CYCLIC_TASK,	ButtonsHandle);
	Task_Create_New(TASK_HANDLE_FREQUENCY(54),	TSK_TYPE_CYCLIC_TASK,	Refresh_Display_Content);
	Task_Create_New(TASK_HANDLE_FREQUENCY(25),	TSK_TYPE_CYCLIC_TASK,	Display_Other_Time_Actions);
	Task_Create_New(TASK_HANDLE_FREQUENCY(21),	TSK_TYPE_CYCLIC_TASK,	UART_to_XLR_Manager);
	Task_Create_New(TASK_HANDLE_FREQUENCY(8),	TSK_TYPE_CYCLIC_TASK,	TimedProcessData);
	Task_Create_New(TASK_HANDLE_FREQUENCY(113),	TSK_TYPE_CYCLIC_TASK,	DimmersCommThread);
	Task_Create_New(TASK_HANDLE_FREQUENCY(80),	TSK_TYPE_CYCLIC_TASK,	DimmersCtrlThread);
	Task_Create_New(TASK_HANDLE_FREQUENCY(25),	TSK_TYPE_CYCLIC_TASK,	SaveToFlash_Thread);
	Task_Create_New(TASK_HANDLE_FREQUENCY(20), 	TSK_TYPE_CYCLIC_TASK,	PluginModuleHandle_ETHERNET);
	Task_Create_New(TASK_HANDLE_FREQUENCY(10), 	TSK_TYPE_CYCLIC_TASK,	DimmersCommTimed);
	Task_Create_New(TASK_HANDLE_FREQUENCY(11),	TSK_TYPE_CYCLIC_TASK,	General_Timed_Function);
	Task_Create_New(TASK_HANDLE_FREQUENCY(500),	TSK_TYPE_CYCLIC_TASK,	Cyclic500Hz);

	while (1)
	{
		if (DMX_XLR_Data_Process) Process_DMX_XLR_Data_Fnc();
		if (DMX_ETH_Data_Process) Process_DMX_ETH_Data_Fnc();
		if (DmxRS485_RDM_DataToProcess) Process_DMX_RS485_RDM_Data_Packet();	// in Rdm
		if (DmxRS485_RDM_DiscoveryResponse) Process_RDM_RS485_Discovery_Response();	// in UART_XLR
		if (ReceivedDimmerData) CheckDimmersReceivedData();

		Task_CheckTasksToExe();
		if (Interval_SysTick_Elapsed) SysTickIntervalHandle();
	}
}

