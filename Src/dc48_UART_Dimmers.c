/*
 * dc48_PowerCtrl.c
 *
 *  Created on: Jan 30, 2026
 *      Author: harma
 */

#include "stm32h523xx.h"
#include "dc48_Globals.h"
#include "dc48_DimmersCom.h"

/* Local Variables and MACROS */
#define	TX_DATA_LENGTH				28
#define	TX_DATA_LENGTH_SH			(TX_DATA_LENGTH>>1)
#define	RX_DATA_LENGTH				14
#define	RX_DATA_LENGTH_SH			(RX_DATA_LENGTH>>1)
#define LPUART_TRANSMIT_COMPLETE	BITBAND_REG(LPUART0->STAT, LPUART_STAT_TC_SHIFT)

#define DIMM_CMND_GET_RUNTIME_DT		0x00
#define DIMM_CMND_GET_VERSIONS			0x30

#define SERIAL_DIMM_SETUP__TRIAC_STYLE			0x0001
#define SERIAL_DIMM_SETUP__RELAY_USING			0x0002
#define SERIAL_DIMM_SETUP__TRIAC_STYLE_OPT		0x0100
#define SERIAL_DIMM_SETUP__RELAY_USING_OPT		0x0200
#define MAX_VALUE_FOR_100_PROC_OUT				36000

#define	NUM_OF_1COM_CHANNELS		12
#define DIMMCOMM_TX_BUFFER_SIZE		16
#define DIMMCOMM_RX_BUFFER_SIZE		16
#define NUM_OF_COM_CHANNELS			4
#define RX_RESPONSE_LENGTH			14

#define DIMMERS_GROUP_0112			0
#define DIMMERS_GROUP_1324			1
#define DIMMERS_GROUP_2536			2
#define DIMMERS_GROUP_3748			3

#define UART_START_TX	(USART_CR1_RE + \
						USART_CR1_RXNEIE + \
						USART_CR1_RTOIE + \
						USART_CR1_TE + \
						USART_CR1_UE + \
						USART_CR1_TXEIE)
#define UART_START_RX	(USART_CR1_RE + \
						USART_CR1_RXNEIE + \
						USART_CR1_RTOIE + \
						USART_CR1_TE + \
						USART_CR1_UE)
#define LPUART_START_TX	(USART_CR1_RE + \
						USART_CR1_RXNEIE + \
						USART_CR1_TE + \
						USART_CR1_UE + \
						USART_CR1_TXEIE)
#define LPUART_START_RX	(USART_CR1_RE + \
						USART_CR1_RXNEIE + \
						USART_CR1_TE + \
						USART_CR1_UE)
#define UART_CONFIGURATION	USART_CR2_TXINV | USART_CR2_RTOEN// | USART_CR2_STOP_1

typedef struct
{
	unsigned int		Responder;
	unsigned char *		pTx;
	unsigned int		TxCnt;
	unsigned short		TxData[DIMMCOMM_TX_BUFFER_SIZE];
	char				RxData[DIMMCOMM_RX_BUFFER_SIZE];
	unsigned int		RxCnt;
	unsigned int		RxFrameReceived;
	unsigned int		Dimmer9600Start;
	unsigned int		Dimmer9600State;
	unsigned int		Dimmer9600End;
}	tComDim;

tComDim				ComDim[NUM_OF_COM_CHANNELS];
int					ReceivedDimmerData;
int					DimmerCommGrp;

unsigned short frame_crc(unsigned short * ds, int len)
{
	unsigned short crc = 0;
	while (len--)
		crc ^= *ds++;
	*ds = crc;
	return crc;
}

void _measure_13fr_time(tComDim * comdim)
{
	if (comdim->Dimmer9600State == 0)
	{
		comdim->Dimmer9600Start = TIM5->CNT;
	}
	else
	if (comdim->Dimmer9600State == 13)
	{
		comdim->Dimmer9600End = TIM5->CNT;
	}
	comdim->Dimmer9600State++;
}

// Dimmers 1-12 - LPUART1 -------------------------------------------------

void LPUART1_IRQHandler(void)
{
	tComDim * comdim = &ComDim[DIMMERS_GROUP_0112];
	if ((LPUART1->CR1 & USART_CR1_TXEIE) && (LPUART1->ISR & USART_ISR_TXE))
	{
		LPUART1->TDR = *comdim->pTx++;// | 0x100;		// Send first byte
		if (!--comdim->TxCnt)
		{
			LPUART1->CR1 = LPUART_START_RX;
			comdim->RxCnt = 0;
			comdim->RxFrameReceived = 0;
		}
	}
	if (LPUART1->ISR & USART_ISR_RXNE)
	{
		_measure_13fr_time(comdim);
		char d = LPUART1->RDR;
		if (comdim->RxCnt < DIMMCOMM_RX_BUFFER_SIZE)
		{
			comdim->RxData[comdim->RxCnt++] = d;
			if (comdim->RxCnt == RX_RESPONSE_LENGTH)
			{
				LED_DEBUG_BLUE_TGL;
				comdim->RxFrameReceived = comdim->RxCnt;
				ReceivedDimmerData++;
			}
		}
	}
	LPUART1->RDR;
	if ((LPUART1->ISR & USART_ISR_ORE) || (LPUART1->ISR & USART_ISR_FE))
	{
		comdim->RxCnt = 0;
	}
	LPUART1->ICR = USART_ICR_TCCF | USART_ICR_IDLECF | USART_ICR_ORECF | USART_ICR_FECF;
}

// Dimmers 13-24 - UART5  -------------------------------------------------

void UART5_IRQHandler(void)
{
	tComDim * comdim = &ComDim[DIMMERS_GROUP_1324];
	if ((UART5->CR1 & USART_CR1_TXEIE) && (UART5->ISR & USART_ISR_TXE))
	{
		UART5->TDR = *comdim->pTx++;// | 0x100;		// Send first byte
		if (!--comdim->TxCnt)
		{
			UART5->CR1 = UART_START_RX;
			comdim->RxCnt = 0;
			comdim->RxFrameReceived = 0;
		}
	}
	if (UART5->ISR & USART_ISR_RXNE)
	{
		_measure_13fr_time(comdim);
		char d = UART5->RDR;
		if (comdim->RxCnt < DIMMCOMM_RX_BUFFER_SIZE)
			comdim->RxData[comdim->RxCnt++] = d;
	}
	UART5->RDR;
	if (UART5->ISR & USART_ISR_RTOF)
	{
		LED_DEBUG_RED_TGL;
		if (comdim->RxCnt == RX_RESPONSE_LENGTH)
		{
			comdim->RxFrameReceived = comdim->RxCnt;
			ReceivedDimmerData++;
		}
	}
	if ((UART5->ISR & USART_ISR_ORE) || (UART5->ISR & USART_ISR_FE))
	{
		comdim->RxCnt = 0;
	}
	UART5->ICR = USART_ICR_RTOCF | USART_ICR_TCCF | USART_ICR_IDLECF | USART_ICR_ORECF | USART_ICR_FECF;
}

// Dimmers 25-36 - USART3  -------------------------------------------------

void USART3_IRQHandler(void)
{
	tComDim * comdim = &ComDim[DIMMERS_GROUP_2536];
	if ((USART3->CR1 & USART_CR1_TXEIE) && (USART3->ISR & USART_ISR_TXE))
	{
		USART3->TDR = *comdim->pTx++;// | 0x100;		// Send first byte
		if (!--comdim->TxCnt)
		{
			USART3->CR1 = UART_START_RX;
			comdim->RxCnt = 0;
			comdim->RxFrameReceived = 0;
		}
	}
	if (USART3->ISR & USART_ISR_RXNE)
	{
		_measure_13fr_time(comdim);
		char d = USART3->RDR;
		if (comdim->RxCnt < DIMMCOMM_RX_BUFFER_SIZE)
			comdim->RxData[comdim->RxCnt++] = d;
	}
	USART3->RDR;
	if (USART3->ISR & USART_ISR_RTOF)
	{
		LED_DEBUG_BLUE_TGL;
		if (comdim->RxCnt == RX_RESPONSE_LENGTH)
		{
			comdim->RxFrameReceived = comdim->RxCnt;
			ReceivedDimmerData++;
		}
	}
	if ((USART3->ISR & USART_ISR_ORE) || (USART3->ISR & USART_ISR_FE))
	{
		comdim->RxCnt = 0;
	}
	USART3->ICR = USART_ICR_RTOCF | USART_ICR_TCCF | USART_ICR_IDLECF | USART_ICR_ORECF | USART_ICR_FECF;
}

// Dimmers 37-48 - UART4  -------------------------------------------------

void UART4_IRQHandler(void)
{
	tComDim * comdim = &ComDim[DIMMERS_GROUP_3748];
	if ((UART4->CR1 & USART_CR1_TXEIE) && (UART4->ISR & USART_ISR_TXE))
	{
		UART4->TDR = *comdim->pTx++;// | 0x100;		// Send first byte
		if (!--comdim->TxCnt)
		{
			UART4->CR1 = UART_START_RX;
			comdim->RxCnt = 0;
			comdim->RxFrameReceived = 0;
		}
	}
	if (UART4->ISR & USART_ISR_RXNE)
	{
		_measure_13fr_time(comdim);
		char d = UART4->RDR;
		if (comdim->RxCnt < DIMMCOMM_RX_BUFFER_SIZE)
			comdim->RxData[comdim->RxCnt++] = d;
	}
	UART4->RDR;
	if (UART4->ISR & USART_ISR_RTOF)
	{
		LED_DEBUG_RED_TGL;
		if (comdim->RxCnt == RX_RESPONSE_LENGTH)
		{
			comdim->RxFrameReceived = comdim->RxCnt;
			ReceivedDimmerData++;
		}
	}
	if ((UART4->ISR & USART_ISR_ORE) || (UART4->ISR & USART_ISR_FE))
	{
		comdim->RxCnt = 0;
	}
	UART4->ICR = USART_ICR_RTOCF | USART_ICR_TCCF | USART_ICR_IDLECF | USART_ICR_ORECF | USART_ICR_FECF;
}

void _PrepareDataForTx(tComDim * comdim, const float * dimData, const TDimmerSetup * dimset)
{
	comdim->Responder++;
	if (comdim->Responder >= NUM_OF_1COM_CHANNELS) comdim->Responder = 0;

	comdim->TxCnt = 28;
	unsigned short * txd = comdim->TxData;
	comdim->pTx = (unsigned char *)txd;
	*txd++ = comdim->Responder + 0x40 + (dimset[comdim->Responder].Ts_Ru << 8);
	for (int i=0; i<NUM_OF_1COM_CHANNELS; i++)
		*txd++ = (*dimData++) * 36000.0f;
	frame_crc(comdim->TxData, TX_DATA_LENGTH_SH-1);
}

void CheckDimmersReceivedData(void)
{
	ReceivedDimmerData = 0;
	for (int i=0; i<NUM_OF_COM_CHANNELS; i++)
		if (ComDim[i].RxFrameReceived)
		{
			ComDim[i].RxFrameReceived = 0;
			ProcessDimmersRxData(i, ComDim[i].RxData);
			ProcessDimmerFrequency(ComDim[i].Dimmer9600Start, ComDim[i].Dimmer9600End);
			ComDim[i].Dimmer9600State = 0;
		}
}

// 8.85 ms period (113 Hz)
// each group is 35.4 ms = 28.25 Hz refresh rate

void DimmersCommThread(void)
{
	switch (DimmerCommGrp++)
	{
	case DIMMERS_GROUP_0112:
		// Dimmers 01-12 - LPUART1  -------------------------------------------------
		_PrepareDataForTx(&ComDim[0], &DimmerOutput[0*12], &DimmerSetup[0*12]);
		LPUART1->CR1 = LPUART_START_TX;
		break;
	case DIMMERS_GROUP_1324:
		// Dimmers 13-24 - UART5  -------------------------------------------------
		_PrepareDataForTx(&ComDim[1], &DimmerOutput[1*12], &DimmerSetup[1*12]);
		UART5->CR1 = UART_START_TX;
		break;
	case DIMMERS_GROUP_2536:
		// Dimmers 25-36 - USART3  -------------------------------------------------
		_PrepareDataForTx(&ComDim[2], &DimmerOutput[2*12], &DimmerSetup[2*12]);
		USART3->CR1 = UART_START_TX;
		break;
	case DIMMERS_GROUP_3748:
		// Dimmers 37-48 - UART4  -------------------------------------------------
		_PrepareDataForTx(&ComDim[3], &DimmerOutput[3*12], &DimmerSetup[3*12]);
		UART4->CR1 = UART_START_TX;
		DimmerCommGrp = 0;
		break;
	}
}

void DimmersCommInitialization(void)
{
	// Dimmers 1-12 - LPUART1 -------------------------------------------------
	RCC->APB3ENR |= RCC_APB3ENR_LPUART1EN;
	NVIC->ISER[1] |= (1<<(LPUART1_IRQn-32));				// Enable INTERRUPT in NVIC
	LPUART1->PRESC = 8;										// 25MHz / 32 = 781250 Hz
	LPUART1->BRR = 20833;									// (781250 Hz * 256) / 20833 = 9600 Hz
	LPUART1->CR2 = USART_CR2_TXINV;

	// Dimmers 13-24 - UART5  -------------------------------------------------
	RCC->APB1LENR |= RCC_APB1LENR_UART5EN;
	NVIC->ISER[1] |= (1<<(UART5_IRQn-32));					// Enable INTERRUPT in NVIC
	UART5->PRESC = 2;										// 200MHz / 4 = 50 MHz
	UART5->BRR = 5208;										// 50MHz / 5208 = 9600 Hz
	UART5->RTOR = 35;										// 25 Bits timeout
	UART5->CR2 = UART_CONFIGURATION;

	// Dimmers 25-36 - USART3  -------------------------------------------------
	RCC->APB1LENR |= RCC_APB1LENR_USART3EN;
	NVIC->ISER[1] |= (1<<(USART3_IRQn-32));					// Enable INTERRUPT in NVIC
	USART3->PRESC = 2;										// 200MHz / 4 = 50 MHz
	USART3->BRR = 5208;										// 50MHz / 5208 = 9600 Hz
	USART3->RTOR = 35;										// 25 Bits timeout
	USART3->CR2 = UART_CONFIGURATION;

	// Dimmers 37-48 - UART4  -------------------------------------------------
	RCC->APB1LENR |= RCC_APB1LENR_UART4EN;
	NVIC->ISER[1] |= (1<<(UART4_IRQn-32));					// Enable INTERRUPT in NVIC
	UART4->PRESC = 2;										// 200MHz / 4 = 50 MHz
	UART4->BRR = 5208;										// 50MHz / 5208 = 9600 Hz
	UART4->RTOR = 35;										// 25 Bits timeout
	UART4->CR2 = UART_CONFIGURATION;

	// Others Initializations
	ComDim[0].Responder = 0;
	ComDim[1].Responder = 0;
	ComDim[2].Responder = 0;
	ComDim[3].Responder = 0;
	ReceivedDimmerData = 0;

	// Timers for measure PSTR4 frequency
	RCC->APB1LENR |= RCC_APB1LENR_TIM5EN;
	(void)RCC->APB1LENR;
	TIM5->PSC = 0;
	TIM5->EGR = TIM_EGR_UG;
	TIM5->CR1 = TIM_CR1_CEN;	// Counts run at 200MHz
	DimmerCommGrp = 0;
}
