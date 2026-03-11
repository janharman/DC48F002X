/*
 * dc4_UART_Crmx.c
 *
 *  Created on: 18. 12. 2019
 *      Author: harma
 */

#include "stm32h5xx.h"
#include "dc48_Globals.h"
#include "dc48_UART_Ethernet.h"

/* Local variables and MACROS */
#define DSX_BREAK_IS_AWAITING				0
#define DSX_READY_TO_REC_START_C			1
#define DSX_RECEIVING_DATA_FRAME			2

int 		DmxETH_RecStatus;
int			DmxETH_ReceivedStartCode;
int			DmxETH_ReceivedChrCnt;
int			DmxETH_LastReceivedChans;

void Reset_UART_ETHERNET_For_ReceivingDMX(void)
{
	DmxETH_RecStatus = DSX_BREAK_IS_AWAITING;
	USART6->CR1 = 	0;						// 8 Data Bits
	USART6->CR2 = 	USART_CR2_STOP_1;		// 2 Stop Bits
	// Module clock for UART6 is System clock = 200MHz.
	// Baud Rate = Module clock / (16 x SBR + BRFD) = 80000000 / (16 x 20 + 0) = 250000
	USART6->PRESC = 2;					// 200 MHz / 4 = 50 MHz
	USART6->BRR = 200;					// 50 MHz / 200 = 250 kHz
	USART6->CR1 = (	USART_CR1_UE |		// USART ENABLE
					USART_CR1_RE | 		// RECEIVER ENABLE
					USART_CR1_RXNEIE	// RECEIVER INTERRUPT ENABLE
				  );
}

void USART6_IRQHandler(void)
{
	unsigned int d = USART6->RDR;
	if (USART6->ISR & (USART_ISR_ORE | USART_ISR_FE))// | USART_ISR_NE))
	{
		USART6->ICR = USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_NECF;
		DmxETH_RecStatus = DSX_BREAK_IS_AWAITING;
	}
	switch (DmxETH_RecStatus)
	{
	case DSX_READY_TO_REC_START_C:
			DmxETH_ReceivedStartCode = d;
			if (d) break;
			DmxETH_RecStatus = DSX_RECEIVING_DATA_FRAME;
			// no break
	case DSX_RECEIVING_DATA_FRAME:
			DMX_ETH_RecBuffer[DmxETH_ReceivedChrCnt++] = d;
			if (DmxETH_ReceivedChrCnt >= NUMBER_OF_REC_DMX_FRAMES)
				DmxETH_RecStatus = DSX_BREAK_IS_AWAITING;
			break;
	default:
			if (!d)
			{
				DmxETH_RecStatus = DSX_READY_TO_REC_START_C;
				if (DmxETH_LastReceivedChans == DmxETH_ReceivedChrCnt)
					if (DmxETH_ReceivedChrCnt >= NUMBER_OF_REC_DMX_FRAMES)
					{
						DMX_ETH_Data_Process = 1;
					}
				DmxETH_LastReceivedChans = DmxETH_ReceivedChrCnt;
				DmxETH_ReceivedChrCnt = 0;
			}
	}
}

int Uart_ETHERNET_Enabled(void)
{
	return USART6->CR1 & USART_CR1_UE;
}

void Disable_Uart_ETHERNET(void)
{
	USART6->CR1 = 0;
}

void Initialization_Uart_ETHETNET(void)
{
	RCC->APB1LENR |= RCC_APB1LENR_USART6EN;
	Reset_UART_ETHERNET_For_ReceivingDMX();
	NVIC->ISER[2] |= (1<<(USART6_IRQn-64));				// Enable INTERRUPT in NVIC
	DmxETH_RecStatus = DSX_BREAK_IS_AWAITING;
}


