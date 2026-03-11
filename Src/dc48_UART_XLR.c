/*
 * dc4_UART_XLR.c
 *
 *  Created on: 14. 2. 2019
 *      Author: J�n Harman
 */

#include <dc48_UART_XLR.h>
#include "stm32h5xx.h"
#include "dc48_Globals.h"
//#include "dc5_UART_Crmx.h"
//#include "dc5_Rdm.h"
#include "dc48_RdmDef.h"
#include "dc48_Ethernet.h"
#include "rdm.h"
#ifdef COMPILATION_FOR__DSL
#include "dc5_DisplayConst.h"
#endif

/* Local Variables and MACROS */

#define DS_BREAK_IS_AWAITING			0
#define DS_READY_TO_REC_START_C			1
#define DS_DMX_ADDRESS_MATCHING			2
#define DS_RECEIVING_DATA_FRAME			3
#define TRS_TRANSMITER_INACTIVE			0
#define TRS_IDLE_STATE					1
#define TRS_MARK_BEFORE_BREAK			2
#define TRS_BREAK_PROCESSING			3
#define TRS_MARK_AFTER_BREAK			4
#define TRS_LAST_SLOT_SENDING			5
#define TRS_RDM_DELAY_BEFORE_TX			6
#define TRS_RDM_BEAK_AFTER_SLOT			7
#define TRS_RDM_END_OF_TRANSMIT			8
#define TRS_CHECK_LINE_BEFORE_TX		9
#define TRS_CHECKOUT_LINE				10

int 						DmxXLR_ValidRecChrCnt;				// pocet platnych prijatych znaku do bufferu
int 						DmxXLR_ReceivedStartCode;			// Last received Start code
int 						DmxXLR_ReceivedChrCnt;				// Pocet prijatych znaku celkem
int 						DmxXLR_PrevRecStartCode;			// Start Code received in the previous frame
int 						DmxXLR_RecStatus;
unsigned int 				DmxXLR_TransmitSts;
int							DmxXLR_LastReceivedChans;
int 						DmxXLR_TransmitChrCnt;
const int *					DmxXLR_TransCharPtr;
int 						DmxXLR_OutputIsActive;				// enabled only if Wifi is enabled and DmxToXlr is enabled too
int 						RdmXLR_TransmittingInProcess;		// for avoid colision with regular DMX transfer
unsigned int 				DmxXLR_PresentDelayCnt;
unsigned int 				DmxETH_PresentDelayCnt;
unsigned int				DmxXLR_CheckingLine;
unsigned int				DmxXLR_LineIsFree;
int							DmxXLR_RDM_TransmitBuffer[520];
int							DmxXLR_RDM_Response;


#define UART1_TRANSMITTER_ENABLED		(USART1->CR1 & USART_CR1_TE)
#define UART1_RECEIVER_ENABLED			(USART1->CR1 & USART_CR1_RE)
// Module clock for UART1 is System clock / 2 = 32MHz.
// Baud Rate = 50000000/200 = 250000
#define USART1_BRR_FOR_250000BD			200
#define NUMBER_OF_REC_DMX_FRAMES		513

void __set_tim16_interval(int interval)
{
	TIM6->CR1 = 0;
	TIM6->SR = 0;
	TIM6->CNT = 0;
	TIM6->ARR = interval;
	TIM6->DIER = TIM_DIER_UIE;
	TIM6->CR1 = TIM_CR1_OPM + TIM_CR1_CEN;
}

void Reset_UART1_For_IdleState(void)
{
	DMX_XLR_SET_DIR_TO_TRANSMIT;
	USART1->CR1 = 0;
	DMX_XLR_SET_TRANSMIT_TX_LINE_1;
	// GPIO = HIGH STATE
	GPIOA->MODER &= ~GPIO_MODER_MODE9;
	// GPIO FOR OUTPUT MODE
	GPIOA->MODER |= GPIO_MODER_MODE9_0;
	DmxXLR_TransmitSts = TRS_IDLE_STATE;
	TIM6->CR1 = 0;
	TIM6->PSC = 249;						// PRESCALER = 64 --> 250MHz/250=1MHz --> 1us is ctc frequency
	TIM6->ARR = 100;
	NVIC->ISER[1] |= (1<<(TIM6_IRQn-32));	// Enable INTERRUPT in NVIC
}

void Reset_UART1_For_ReceivingDMX(void)
{
	DmxXLR_RecStatus = DS_BREAK_IS_AWAITING;
	DMX_XLR_SET_DIR_TO_RECEIVE;
	// GPIO FOR UART - TX line
	GPIOA->MODER &= ~GPIO_MODER_MODE9;
	GPIOA->MODER |= GPIO_MODER_MODE9_1;	// Alternate Function
	USART1->CR1 = 	0;
	USART1->CR1 = 	USART_CR1_M0;			// 9 Data Bits
	USART1->CR2 = 	USART_CR2_RTOEN;		// 1 Stop Bits, RECEIVER TIMEOUT ENABLED
	USART1->CR3 =	USART_CR3_EIE;
	USART1->RTOR = 30;						// Number of bits after received last char
	USART1->BRR = USART1_BRR_FOR_250000BD;
	USART1->CR1 = (	USART_CR1_M0 |		// 9 Data Bits
					USART_CR1_UE |		// USART ENABLE
					USART_CR1_RE | 		// RECEIVER ENABLE
					USART_CR1_RXNEIE |	// RECEIVER INTERRUPT ENABLE
					USART_CR1_RTOIE |		// RECEIVER TIMEOUT INTERRUPT ENABLE
				    0);
}

void Reset_UART1_For_TransmitDMX(void)
{
	DmxXLR_TransmitSts = TRS_MARK_BEFORE_BREAK;
	if (DmxXLR_RDM_Response)
		DmxXLR_TransCharPtr = DmxXLR_RDM_TransmitBuffer;
//	else
//		DmxXLR_TransCharPtr = DmxCRMX_Buffer;
	DmxXLR_RDM_Response = 0;
	DMX_XLR_SET_DIR_TO_TRANSMIT;
	// GPIO FOR UART
	GPIOA->MODER &= ~GPIO_MODER_MODE9;
	GPIOA->MODER |= GPIO_MODER_MODE9_1;	// Alternate Function
	USART1->CR1 = 	0;						// 8 Data Bits
	USART1->CR2 = 	USART_CR2_STOP_1;		// 2 Stop Bits
	USART1->CR3 =	0;
	USART1->BRR = USART1_BRR_FOR_250000BD;
	USART1->CR1 = (	USART_CR1_UE |		// USART ENABLE
					USART_CR1_TE | 		// TRANSMITTER ENABLE
					USART_CR1_TXEIE		// TRANSMIT DATA REGISTER EMPTY INTERRUPT ENABLE
				);
}

void TIM6_IRQHandler(void)
{
	TIM6->SR = 0;
	switch (DmxXLR_TransmitSts)
	{
	case TRS_MARK_BEFORE_BREAK:
		// Break is starting
		DMX_XLR_SET_TRANSMIT_TX_LINE_0;
		DmxXLR_TransmitSts = TRS_BREAK_PROCESSING;
		if (RdmXLR_TransmittingInProcess)
			__set_tim16_interval(200);
		else
			__set_tim16_interval(200);
		break;
	case TRS_BREAK_PROCESSING:
		DMX_XLR_SET_TRANSMIT_TX_LINE_1;
		DmxXLR_TransmitSts = TRS_MARK_AFTER_BREAK;
		__set_tim16_interval(8);
		break;
	case TRS_MARK_AFTER_BREAK:
		Reset_UART1_For_TransmitDMX();
		break;
	case TRS_RDM_DELAY_BEFORE_TX:
		Reset_UART1_For_TransmitDMX();
		break;
	case TRS_LAST_SLOT_SENDING:
		if (DmxXLR_OutputIsActive)
		{
			Reset_UART1_For_IdleState();	// DmxXLR_TransmitSts = TRS_IDLE_STATE;
			DmxXLR_TransmitChrCnt = NUMBER_OF_REC_DMX_FRAMES;
//			DmxXLR_TransmitSts = TRS_MARK_BEFORE_BREAK;
			DmxXLR_TransmitSts = TRS_CHECK_LINE_BEFORE_TX;
			__set_tim16_interval(200);
			break;
		}
		TIM6->CR1 = 0;	// pro jistotu vypnout timer
		DmxXLR_TransmitSts = TRS_TRANSMITER_INACTIVE;
		break;
	case TRS_RDM_BEAK_AFTER_SLOT:
		Reset_UART1_For_IdleState();	// DmxXLR_TransmitSts = TRS_IDLE_STATE;
		DMX_XLR_SET_TRANSMIT_TX_LINE_0;
		DmxXLR_TransmitSts = TRS_RDM_END_OF_TRANSMIT;
		__set_tim16_interval(200);
		break;
	case TRS_RDM_END_OF_TRANSMIT:
		DMX_XLR_SET_TRANSMIT_TX_LINE_1;
		DmxXLR_TransmitSts = TRS_TRANSMITER_INACTIVE;
		Reset_UART1_For_ReceivingDMX();
		break;
	case TRS_CHECK_LINE_BEFORE_TX:
		DmxXLR_CheckingLine = 0;
		Reset_UART1_For_ReceivingDMX();
		__set_tim16_interval(800);
		DmxXLR_TransmitSts = TRS_CHECKOUT_LINE;
		break;
	case TRS_CHECKOUT_LINE:
		if (DmxXLR_CheckingLine)
		{
			RdmXLR_TransmittingInProcess = 0;
			DMX_to_XLR_Active = 0;
			DmxXLR_TransmitSts = TRS_TRANSMITER_INACTIVE;
			DMX_XLR_SET_DIR_TO_RECEIVE;
			DmxXLR_LineIsFree = 0;
			break;
		}
		Reset_UART1_For_IdleState();	// DmxXLR_TransmitSts = TRS_IDLE_STATE;
		DmxXLR_TransmitChrCnt = NUMBER_OF_REC_DMX_FRAMES;
		DmxXLR_TransmitSts = TRS_MARK_BEFORE_BREAK;
		__set_tim16_interval(200);
		break;
	}
}

void USART1_IRQHandler(void)
{
	if (DmxXLR_TransmitChrCnt && (USART1->ISR & USART_ISR_TXE_TXFNF))
	{
		USART1->TDR = (unsigned short)*DmxXLR_TransCharPtr++;
		DmxXLR_TransmitChrCnt--;
		if (!DmxXLR_TransmitChrCnt)
		{
			USART1->CR1 = (USART_CR1_UE | USART_CR1_TE);	// no interrupt from now
			if (!RdmXLR_TransmittingInProcess)
			{
				DmxXLR_TransmitSts = TRS_LAST_SLOT_SENDING;
				__set_tim16_interval(120);
			}
			else
			{
				DmxXLR_TransmitSts = TRS_RDM_END_OF_TRANSMIT;
				__set_tim16_interval(220);
			}
		}
		return;
	}

	unsigned int ds = USART1->RDR;
	unsigned int d = ds & 0xff;
	unsigned int frameErr = USART1->ISR & USART_ISR_FE;
	unsigned int rtoi = USART1->ISR & USART_ISR_RTOF;
	USART1->ICR = USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_NECF | USART_ICR_RTOCF;

	if (rtoi)	// RECEIVER TIMEOUT INTERRUPT
	{
		if (!DmxXLR_ReceivedChrCnt) return;	// Interrupt after Break is supressed!
		if (*DMX_XLR_RecBuffer == E120_SC_RDM)
		{
			DmxRS485_RDM_DataToProcess = DmxXLR_ReceivedChrCnt;
			DmxXLR_LastReceivedChans = 0;
			return;
		}
		if (DmxXLR_LastReceivedChans == DmxXLR_ReceivedChrCnt)
		{
			if (!DmxXLR_PrevRecStartCode && !DMX_XLR_RecBuffer[0])
			{
				DMX_XLR_Data_Process = 1;
				DMX_XLR_NumOfRecChans = DmxXLR_ReceivedChrCnt;
			}
		}
		DmxXLR_LastReceivedChans = DmxXLR_ReceivedChrCnt;
		DmxXLR_PrevRecStartCode = DMX_XLR_RecBuffer[0];
		DmxXLR_ReceivedChrCnt = 0;
		DmxXLR_RecStatus = DS_BREAK_IS_AWAITING;
		return;
	}

	if (frameErr || !(ds & 0x100))
		DmxXLR_RecStatus = DS_BREAK_IS_AWAITING;

	if (DmxXLR_RecStatus == DS_RECEIVING_DATA_FRAME)
	{
			DmxXLR_CheckingLine++;
			if (DmxXLR_ReceivedChrCnt < REC_DMX_BUFFER_SIZE)
			{
					DMX_XLR_RecBuffer[DmxXLR_ReceivedChrCnt++] = d;
			}
			else
			{
					DmxXLR_RecStatus = DS_BREAK_IS_AWAITING;
					DMX_XLR_RecBuffer[0] = 0xFF;						// discard whole frame
			}
	}
	else
	{
			if (frameErr && !ds) // --> means BREAK was received
			{
					DmxXLR_CheckingLine++;
					DmxXLR_RecStatus = DS_RECEIVING_DATA_FRAME;
					DmxXLR_ReceivedChrCnt = 0;
			}
	}
}

void Process_RDM_RS485_Discovery_Response(void)
{
	if (DmxXLR_TransmitSts == TRS_TRANSMITER_INACTIVE)
	{
		LED_BlueCtc = 10;
		Reset_UART1_For_IdleState();
		DmxXLR_TransmitChrCnt = DmxRS485_RDM_DiscoveryResponse;
		DmxXLR_RDM_Response = 1;
		const char * c = (char*)&RDM_Packet;
		for (int i=0; i<DmxRS485_RDM_DiscoveryResponse; i++)
			DmxXLR_RDM_TransmitBuffer[i] = *c++;
		if (RDM_Packet.StartCode == 0xFE) // it is DISC_UNIQUE_BRANCH RESPONSE
		{
				// Upravime vysilani tak, aby se nevysilal BREAK, jen kratky MARK AFTER BREAK
				// Minimalni mezera mezi poslednim prijatym znakem a vysilanim musi byt 176us.
				// Nyni je mezera asi 200us.
				__set_tim16_interval(200);
				DmxXLR_TransmitSts = TRS_RDM_DELAY_BEFORE_TX;
		}
		else
		{
				__set_tim16_interval(200);
				DmxXLR_TransmitSts = TRS_MARK_BEFORE_BREAK;
		}
		RdmXLR_TransmittingInProcess = 1;
		return;
	}
	DmxRS485_RDM_DiscoveryResponse = 0;
}

/* 21Hz frequency = 48ms interval from main task */

void UART_to_XLR_Manager(void)
{
	if (DMX_Is_Present & DMX_PR_XLR)
	{
			DmxETH_PresentDelayCnt = 1;
			DmxXLR_PresentDelayCnt = 21*60;		// 1 minuta
			DmxXLR_LineIsFree = 0;
			XLR_Display_Status = XLR_MODE_INPUT_SIGNAL;
			return;
	}
	XLR_Display_Status = XLR_MODE_NO_DMX_SIGNAL;

	if (DmxXLR_PresentDelayCnt)
	{
			DmxXLR_PresentDelayCnt--;
			if (!DmxXLR_PresentDelayCnt) DmxXLR_LineIsFree = 1;
			return;
	}

	if (DMX_Is_Present & DMX_PR_ETH) DmxETH_PresentDelayCnt = 21*2*60;	// 2 minuty
	else if (DmxETH_PresentDelayCnt) DmxETH_PresentDelayCnt--;

	DmxXLR_OutputIsActive = UnitSettings.EthernetToXLR && Eth_ETH_Enabled && DmxETH_PresentDelayCnt && DmxXLR_LineIsFree;
	if (DmxXLR_OutputIsActive)		// XLR OUTPUT ENABLED & CRMX is present
	{
			RdmXLR_TransmittingInProcess = 0;
			DMX_to_XLR_Active = 1;
			XLR_Display_Status = XLR_MODE_OUTPUT_SIGNAL;
			if (DmxXLR_TransmitSts == TRS_TRANSMITER_INACTIVE)	// = INACTIVE
			{
					Reset_UART1_For_IdleState();
			}
			else
			if (DmxXLR_TransmitSts == TRS_IDLE_STATE)
			{
					__set_tim16_interval(88);
					DmxXLR_TransmitChrCnt = NUMBER_OF_REC_DMX_FRAMES;
					DmxXLR_TransmitSts = TRS_MARK_BEFORE_BREAK;
			}
	}
	else
	{
			if (DmxXLR_TransmitSts <= TRS_IDLE_STATE)
			{
					RdmXLR_TransmittingInProcess = 0;
					if (!UART1_RECEIVER_ENABLED)
						Reset_UART1_For_ReceivingDMX();
					DMX_to_XLR_Active = 0;
					DmxXLR_TransmitSts = TRS_TRANSMITER_INACTIVE;
					if (DMX_XLR_DIRECTION_IS_TX)
						DMX_XLR_SET_DIR_TO_RECEIVE;
			}
	}
}

// UART1 Initialization
// Ports PA10 = DMX_RXD
//		 PA9 = DMX_TXD
//		 PA8 = DMX_DIR
//
void initializationUartXLR(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB1LENR |= RCC_APB1LENR_TIM6EN;
	NVIC->ISER[1] |= (1<<(USART1_IRQn-32));				// Enable INTERRUPT in NVIC
	DmxXLR_TransmitSts = TRS_TRANSMITER_INACTIVE;
	DMX_to_XLR_Active = 0;
	DmxXLR_PresentDelayCnt = 210;		// cca 5 seconds
	DmxETH_PresentDelayCnt = 0;
	DmxXLR_LineIsFree = 0;
	DmxXLR_RDM_Response = 0;
	DmxXLR_TransmitChrCnt = 0;
	Reset_UART1_For_ReceivingDMX();
}



