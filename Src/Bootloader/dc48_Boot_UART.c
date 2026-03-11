/*
 * dc4_Boot_UART.c
 *
 *  Created on: Mar 21, 2024
 *      Author: harma
 */


#include <dc48_Boot_Glob.h>
#include "stm32h5xx.h"
#include "string.h"
#include "dc48_Globals.h"

#define BOOT_UART_MODE_RECEIVER						0
#define BOOT_UART_MODE_TRANSMITTER					1
#define BOOT_UART_REC_STS__BREAK_AWAITING			0
#define BOOT_UART_REC_STS__RECEIVING_DATA			1
#define BOOT_UART_TRANS_STS__ONE_BEFORE_TRANS		0
#define BOOT_UART_TRANS_STS__OUTGOING_BREAK			1
#define BOOT_UART_TRANS_STS__OUTGOING_DATA			2

unsigned int						boot_UsartMode;
unsigned int						boot_BuffPtr;
unsigned int						boot_RecSts;
unsigned int						boot_TransmitterSts;
unsigned int						boot_TransBytesCnt;
char *								boot_TransBytePtr;
unsigned int						boot_TransmitGuard;

#define BOOT_TRANSMITTER_GUARD_TIME					200

#define USART_STOP_9BIT				(USART_CR1_RE + USART_CR1_M0)
#define USART_START_9BIT			(USART_CR1_RE + USART_CR1_UE + USART_CR1_M0)

void bl_UartSetForReceiver(void)
{
	boot_BuffPtr = 0;
	boot_UsartMode = BOOT_UART_MODE_RECEIVER;
	boot_RecSts = BOOT_UART_REC_STS__BREAK_AWAITING;
	DMX_XLR_SET_DIR_TO_RECEIVE;
	USART1->CR1 = 0;
	USART1->BRR = 256;	// 250 kb
	USART1->RTOR = 50;	// Receiver Timeout
	USART1->CR2 = USART_CR2_RTOEN;
	USART1->CR1 = USART_STOP_9BIT;	// 9-bit mode, because of proper break detection
	USART1->CR1 = USART_START_9BIT;
}

void bl_UartSetForTransmitter(void)
{
	boot_BuffPtr = 0;
	boot_UsartMode = BOOT_UART_MODE_TRANSMITTER;
	boot_TransmitterSts = BOOT_UART_TRANS_STS__ONE_BEFORE_TRANS;
	DMX_XLR_SET_DIR_TO_TRANSMIT;
	USART1->CR1 = 0;
	// GPIO = HIGH STATE
	DMX_XLR_SET_TRANSMIT_TX_LINE_1;
	// GPIO FOR OUTPUT MODE
	GPIOA->MODER &= ~GPIO_MODER_MODE9;
	GPIOA->MODER |= GPIO_MODER_MODE9_0;
	TIM6->ARR = 50;		// 50 us
	TIM6->CNT = 0;
	TIM6->SR = 0;
	TIM6->CR1 = TIM_CR1_OPM + TIM_CR1_CEN;
	boot_TransmitGuard = BOOT_TRANSMITTER_GUARD_TIME;
}

void bl_UartThread(void)
{
	unsigned int isr = USART1->ISR;
	if (boot_UsartMode == BOOT_UART_MODE_RECEIVER)
	{
		if (!(isr & (USART_ISR_FE | USART_ISR_RXNE | USART_ISR_RTOF))) return;
		USART1->ICR = USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_NECF | USART_ICR_IDLECF | USART_ICR_RTOCF;	// clear flags

		unsigned int ds = USART1->RDR;
		unsigned int d = ds & 0xff;
		unsigned int frame_error = isr & USART_ISR_FE;
		if (frame_error || !(ds & 0x100))	// 9th bit is zero --> it is break
			boot_RecSts = BOOT_UART_REC_STS__BREAK_AWAITING;

		if (boot_RecSts == BOOT_UART_REC_STS__BREAK_AWAITING)
		{
			if (frame_error && !ds)	// Break Received
			{
				boot_RecSts = BOOT_UART_REC_STS__RECEIVING_DATA;
				boot_BuffPtr = 0;
			}
		}
		else
		{
			if (isr & USART_ISR_RTOF)
			{
				blRxProcessFrame = boot_BuffPtr;	// number of received chars
				boot_RecSts = BOOT_UART_REC_STS__BREAK_AWAITING;
				return;
			}

			if (boot_BuffPtr < BOOT_RX_BUFFER_SIZE)
			{
				blRxBuffer[boot_BuffPtr++] = d;
			}
			else
			{
				boot_RecSts = BOOT_UART_REC_STS__BREAK_AWAITING;
			}
		}
	}
	else

	//--------------- TRANSMITTER --------------------------

	if (boot_UsartMode == BOOT_UART_MODE_TRANSMITTER)
	{
		switch (boot_TransmitterSts)
		{
		case BOOT_UART_TRANS_STS__ONE_BEFORE_TRANS:
				if (!(TIM6->SR & TIM_SR_UIF)) return;
				DMX_XLR_SET_TRANSMIT_TX_LINE_0;
				boot_TransmitterSts = BOOT_UART_TRANS_STS__OUTGOING_BREAK;
				TIM6->ARR = 200;
				TIM6->CNT = 0;
				TIM6->SR = 0;
				TIM6->CR1 = TIM_CR1_OPM + TIM_CR1_CEN;
				return;
		case BOOT_UART_TRANS_STS__OUTGOING_BREAK:
				if (!(TIM6->SR & TIM_SR_UIF)) return;
				DMX_XLR_SET_TRANSMIT_TX_LINE_1;
				boot_TransmitterSts = BOOT_UART_TRANS_STS__OUTGOING_DATA;
				GPIOA->MODER &= ~GPIO_MODER_MODE9;
				GPIOA->MODER |= GPIO_MODER_MODE9_1;
				USART1->CR1 = USART_CR1_TE + USART_CR1_UE;
				break;
				// This continue to fill transmit buffer
				// Before start transmit, idle line is send for the one character time length
				// Idle line transmitted is taken as MAB (Mark After Break)
		}
		if (!(isr & (USART_ISR_TXE | USART_ISR_TC))) return;
		if (boot_TransBytesCnt--)
		{
			USART1->TDR = *boot_TransBytePtr++;
		}
		else
		{
			boot_TransBytesCnt = 0;
			if (isr & USART_ISR_TC)
			{
				USART1->ICR = USART_ICR_TCCF;
				bl_UartSetForReceiver();
			}
		}
	}
}

void bl_SendRdmTxFrame(char * b, int cnt)
{
	bl_UartSetForTransmitter();
	boot_TransBytePtr = b;
	boot_TransBytesCnt = cnt;
}

// UART1 Initialization
// Ports PTA10 = RX
//		 PTA9 = TX
//
void bl_initializationUartXLR(void)
{
	RCC->CCIPR1 = 3 << RCC_CCIPR1_USART1SEL_Pos;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	blRxProcessFrame = 0;
	// init TIM3 for UART BREAK character timing
	// 64MHz is input clock
	RCC->APB1LENR |= RCC_APB1LENR_TIM6EN;
	TIM6->PSC = 64;	// -> resulting clock is 1MHz -> 1us period
	TIM6->ARR = 180;
	TIM6->CR1 = TIM_CR1_OPM + TIM_CR1_CEN;
	boot_TransmitGuard = BOOT_TRANSMITTER_GUARD_TIME;

	// Start UART receiver
	bl_UartSetForReceiver();
}



