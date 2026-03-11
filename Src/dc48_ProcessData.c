/*
 * dc4_ProcessData.c
 *
 *  Created on: 14. 2. 2019
 *      Author: J�n Harman
 */

#include "dc48_Globals.h"
#include "dc48_UART_XLR.h"
#include "dc48_UART_Ethernet.h"
#include "dc48_ProcessData.h"
#include "dc48_Ethernet.h"
//#include "dc48_Flash.h"

/* LOCAL VARIABLES */
unsigned int		dmx_xlr_present_ctc;
unsigned int		dmx_eth_present_ctc;
unsigned int		dmx_src[2];
unsigned int 		last_ctrl = 0;
unsigned int		last_ctrl_ctc = 0;
#define CTRL_LAST_TIME	(5*8)

/* from main loop
 * 8x per second = 125 ms period */

void TimedProcessData(void)
{
	// XLR
	if (!--dmx_xlr_present_ctc)
	{
		DMX_Is_Present &= ~DMX_PR_XLR;
		for (int i=0; i<NUM_OF_CHANNELS; i++)
			DMX_XLR_Value[i] = 0;
	}

	// ETH
	if (!--dmx_eth_present_ctc)
	{
		DMX_Is_Present &= ~DMX_PR_ETH;
		for (int i=0; i<NUM_OF_CHANNELS; i++)
			DMX_ETH_Value[i] = 0;
	}
}

void Process_DMX_XLR_Data_Fnc(void)
{
	for (int i=0; i<NUM_OF_CHANNELS; i++)
		DMX_XLR_Value[i] = DMX_XLR_RecBuffer[Patch[i]];
	DMX_Is_Present |= (DMX_PR_XLR | DMX_PR_FROM_STRT);
	DMX_XLR_Data_Process = 0;
	dmx_xlr_present_ctc = 22;
}

void Process_DMX_ETH_Data_Fnc(void)
{
	for (int i=0; i<NUM_OF_CHANNELS; i++)
		DMX_ETH_Value[i] = DMX_ETH_RecBuffer[Patch[i]];
	DMX_Is_Present |= (DMX_PR_ETH | DMX_PR_FROM_STRT);
	DMX_ETH_Data_Process = 0;
	dmx_eth_present_ctc = 22;
}

void ProcessDataInit(void)
{
	dmx_xlr_present_ctc = 1;
	dmx_eth_present_ctc = 1;
}

