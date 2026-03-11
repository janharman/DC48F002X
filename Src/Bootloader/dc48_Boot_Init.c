/*
 * dc4_Boot_Init.c
 *
 *  Created on: Feb 28, 2024
 *      Author: harma
 */

#include <dc48_Boot_Glob.h>
#include "stm32h5xx.h"
//#include "dc4_RdmDef.h"


void bl_SystemInit(void)
{
	uint32_t reg_opsr;
	/* FPU settings ------------------------------------------------------------*/
	#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
	#endif

	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set HSION bit */
	RCC->CR = RCC_CR_HSION;

	/* Reset CFGR register */
	RCC->CFGR1 = 0U;
	RCC->CFGR2 = 0U;

	/* Reset HSEON, HSECSSON, HSEBYP, HSEEXT, HSIDIV, HSIKERON, CSION, CSIKERON, HSI48 and PLLxON bits */
	RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_HSECSSON | RCC_CR_HSEBYP | RCC_CR_HSEEXT | RCC_CR_HSIDIV | RCC_CR_HSIKERON | \
				 RCC_CR_CSION | RCC_CR_CSIKERON |RCC_CR_HSI48ON | RCC_CR_PLL1ON | RCC_CR_PLL2ON);

	/* Reset PLLxCFGR register */
	RCC->PLL1CFGR = 0U;
	RCC->PLL2CFGR = 0U;

	/* Reset PLL1DIVR register */
	RCC->PLL1DIVR = 0x01010280U;
	/* Reset PLL1FRACR register */
	RCC->PLL1FRACR = 0x00000000U;
	/* Reset PLL2DIVR register */
	RCC->PLL2DIVR = 0x01010280U;
	/* Reset PLL2FRACR register */
	RCC->PLL2FRACR = 0x00000000U;

	/* Reset HSEBYP bit */
	RCC->CR &= ~(RCC_CR_HSEBYP);

	/* Disable all interrupts */
	RCC->CIER = 0U;

	/* Configure the Vector Table location add offset address ------------------*/
	SCB->VTOR = FLASH_BASE | 0; //VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */

	/* Check OPSR register to verify if there is an ongoing swap or option bytes update interrupted by a reset */
	reg_opsr = FLASH->OPSR & FLASH_OPSR_CODE_OP;
	if ((reg_opsr == FLASH_OPSR_CODE_OP) || (reg_opsr == (FLASH_OPSR_CODE_OP_2 | FLASH_OPSR_CODE_OP_1)))
	{
		/* Check FLASH Option Control Register access */
		if ((FLASH->OPTCR & FLASH_OPTCR_OPTLOCK) != 0U)
		{
			/* Authorizes the Option Byte registers programming */
			FLASH->OPTKEYR = 0x08192A3BU;
			FLASH->OPTKEYR = 0x4C5D6E7FU;
		}
		/* Launch the option bytes change operation */
		FLASH->OPTCR |= FLASH_OPTCR_OPTSTART;

		/* Lock the FLASH Option Control Register access */
		FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
	}
}

void bl_initClock(void)
{
//	PWR->VOSCR = PWR_VOSCR_VOS;	// Voltage Scaling Control Register
//	while(!READ_BIT(PWR->VOSSR, PWR_VOSSR_VOSRDY)) {}
//
//	RCC->CR = RCC_CR_HSIDIV_0;
//	RCC->HSICFGR = 0x40 << RCC_HSICFGR_HSITRIM_Pos;	// 0x40 is default value
//	while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U){}
//
//	RCC->PLL1CFGR = RCC_PLL1CFGR_PLL1SRC_0 + (8 << RCC_PLL1CFGR_PLL1M_Pos);
//	RCC->PLL1FRACR = 0x1000 << 3;	// Fractional part of the N multiplicator
//	// PLL must be disabled (PLL1ON = 0 and PLL1RDY = 0)
//	RCC->PLL1DIVR = 123											// N
//				  + (1 << RCC_PLL1DIVR_PLL1P_Pos)				// P
//				  + (9 << RCC_PLL1DIVR_PLL1Q_Pos)				// Q	500M/10=50M
//				  + (1 << RCC_PLL1DIVR_PLL1R_Pos)				// R
//				  ;
//	RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1FRACEN_Msk;
//	RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1RGE_0;
//	RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1PEN | RCC_PLL1CFGR_PLL1QEN;
//	RCC->CR |= RCC_CR_PLL1ON;
//	while (READ_BIT(RCC->CR, RCC_CR_PLL1RDY) == 0U){}
//
//	FLASH->ACR = 0x15;	// FLASH LATENCY  + WRHIGHFREQ
//	RCC->CFGR1 = RCC_CFGR1_SW;
//	RCC->CFGR2 = (0b100 << RCC_CFGR2_PPRE2_Pos);	// APB2 PRESCALER -> rcc_pclk2 = rcc_hclk1 / 2 = 125 MHz
}

void bl_initPins(void)
{
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN |
					RCC_AHB2ENR_GPIOBEN |
					RCC_AHB2ENR_GPIOCEN |
					RCC_AHB2ENR_GPIODEN |
					RCC_AHB2ENR_GPIOHEN);
	(void)RCC->AHB2ENR;	// so that port clock are stabilized
	// nastaveni pinu. interrupty si resi kazdy driver sam
	//
//-------------------------------------------------------------------------------------
// portA
//-------------------------------------------------------------------------------------
	GPIOA->MODER =	GPIO_MODER_MODE0_0 |	// Output Mode - OLED_D0
					GPIO_MODER_MODE1_0 |	// Output Mode - OLED_D1
					GPIO_MODER_MODE2_0 |	// Output Mode - OLED_D2
					GPIO_MODER_MODE3_0 |	// Output Mode - OLED_D3
					GPIO_MODER_MODE4_0 |	// Output Mode - OLED_D4
					GPIO_MODER_MODE5_0 |	// Output Mode - OLED_D5
					GPIO_MODER_MODE6_0 |	// Output Mode - OLED_D6
					GPIO_MODER_MODE7_0 |	// Output Mode - OLED_D7
					GPIO_MODER_MODE8_0 |	// Output mode - DMX DIR
					GPIO_MODER_MODE9_1 |	// Alternate Fnc - USART1_TX= DMX TXD
					GPIO_MODER_MODE10_1 |	// Alternate Fnc - USART1_RX= DMX RXD
					GPIO_MODER_MODE11_1 |	// Alternate Fnc - USART4_RX= DIM3748 TXD
					GPIO_MODER_MODE12_1 |	// Alternate Fnc - USART4_TX= DIM3748 RXD
					GPIO_MODER_MODE13_1 |	// Alternate Fnc - SWD IO
					GPIO_MODER_MODE14_1 |	// Alternate Fnc - SWD CLK
					GPIO_MODER_MODE15_1 ;	// Alternate Fnc - TIM2_CH1 = FAN PWM CTRL

	GPIOA->OTYPER = 0;						// NO OPEN DRAIN
	GPIOA->OSPEEDR = 0;						// ALL LOW SPEED
	GPIOA->PUPDR = 	GPIO_PUPDR_PUPD15_0 |	// pull-up FAN CTRL
					GPIO_PUPDR_PUPD14_1	|	// pull-dn SWD_CLK	- reset state
					GPIO_PUPDR_PUPD13_0	|	// pull-up SWD_DIO	- reset state
					0;
	GPIOA->AFR[0] 	= 0;
	GPIOA->AFR[1] 	= (7<<4) |				// PA9 = USART1_TX
					  (7<<8) |				// PA10 = USART1_RX
					  (6<<12) |				// PA11 = USART4_RX
					  (6<<16) |				// PA12 = USART4_TX
					  (0<<20) |				// PA13 = SWD_IO
					  (0<<24) |				// PA14 = SWD_CLK
					  (1<<28) |				// PA15 = TIM2_CH1
					  0;
	GPIOA->ODR = 0;
//-------------------------------------------------------------------------------------
// portB
//-------------------------------------------------------------------------------------
	GPIOB->MODER = 	//GPIO_MODER_MODE0 |		// Input Mode - BUTTON ENTER
					//GPIO_MODER_MODE1 |		// Input Mode - BUTTON UP
					//GPIO_MODER_MODE2 |		// Input Mode - BUTTON DN
					//GPIO_MODER_MODE3 |		// Input mode - FAN TACHO
					GPIO_MODER_MODE4_0 |		// Output mode - LED D2 BLUE
					GPIO_MODER_MODE5_0 |		// Output mode - LED D7 RED
					GPIO_MODER_MODE6_1 |		// Alternate Fnc - LPUART1_TX= DIM0112 TXD
					GPIO_MODER_MODE7_1 |		// Alternate Fnc - LPUART1_RX= DIM0112 RXD
					GPIO_MODER_MODE8_0 |		// Output mode - SVETLO 1
					//GPIO_MODER_MODE10 |		// Input mode - BUTTON ESC
					GPIO_MODER_MODE12_1 |		// Alternate Fnc - SPI2_NSS = ETHERNET CS
					GPIO_MODER_MODE13_1 |		// Alternate Fnc - SPI2_SCK = ETHERNET SCK
					GPIO_MODER_MODE14_1 |		// Alternate Fnc - SPI2_MISO = ETHERNET MISO
					GPIO_MODER_MODE15_1;		// Alternate Fnc - SPI2_MOSI = ETHERNET MOSI

	GPIOB->OTYPER = 0;							// NO OPEN DRAIN
	GPIOB->OSPEEDR = (2 << GPIO_OSPEEDR_OSPEED12_Pos) |
					 (2 << GPIO_OSPEEDR_OSPEED13_Pos) |
					 (2 << GPIO_OSPEEDR_OSPEED14_Pos) |
					 (2 << GPIO_OSPEEDR_OSPEED15_Pos);							// ALL HIGH SPEED
	GPIOB->PUPDR =	GPIO_PUPDR_PUPD0_0 |		// PULL-UP = BUTTON ENTER
					GPIO_PUPDR_PUPD1_0 |		// PULL-UP = BUTTON UP
					GPIO_PUPDR_PUPD2_0 |		// PULL-UP = BUTTON DN
					GPIO_PUPDR_PUPD3_0 |		// PULL-UP = FAN TACHO
					GPIO_PUPDR_PUPD10_0 |		// PULL-UP = BUTTON ESC
					GPIO_PUPDR_PUPD12_0 |		// PULL-UP = ETHERNET CS
					GPIO_PUPDR_PUPD13_0 |		// PULL-UP = ETHERNET CCK
					GPIO_PUPDR_PUPD14_0 |		// PULL-UP = ETHERNET MISO
					GPIO_PUPDR_PUPD15_0 |		// PULL-UP = ETHERNET MOSI
					0;
	GPIOB->AFR[0] = (8<<GPIO_AFRL_AFSEL6_Pos) |		// PB6 = LPUART1_TX
					(8<<GPIO_AFRL_AFSEL7_Pos) |		// PB7 = LPUART1_RX
					0;
	GPIOB->AFR[1] = (5<<GPIO_AFRH_AFSEL12_Pos) |	// PB12 = SPI2_NSS
					(5<<GPIO_AFRH_AFSEL13_Pos) |	// PB13 = SPI2_SCK
					(5<<GPIO_AFRH_AFSEL14_Pos) |	// PB14 = SPI2_MISO
					(5<<GPIO_AFRH_AFSEL15_Pos);		// PB15 = SPI2_MOSI
	GPIOB->ODR = 0x00;
//-------------------------------------------------------------------------------------
// portC
//-------------------------------------------------------------------------------------
	GPIOC->MODER = 	GPIO_MODER_MODE0_0 |	// Output Mode - OLED RES
					GPIO_MODER_MODE1_0 |	// Output Mode - OLED D
					GPIO_MODER_MODE2_0 |	// Output Mode - OLED FW
					GPIO_MODER_MODE3_0 |	// Output mode - OLED E
					GPIO_MODER_MODE4_0 |	// Output mode - LED DMX GREEN
					GPIO_MODER_MODE5 |		// Analog mode - Temperature Sensor
					GPIO_MODER_MODE6_1 |	// Alternate Fnc - UART6_TX = ETHERNET TXD
					GPIO_MODER_MODE7_1 |	// Alternate Fnc - UART6_RX = ETHERNET RXD
					//GPIO_MODER_MODE8_0 |	// Input mode - ETHERNET IRQ
					GPIO_MODER_MODE9_0 |	// Output mode - LED ERROR RED
					GPIO_MODER_MODE10_1 |	// Alternate Fnc - UART3_TX - DIM2336 TXD
					GPIO_MODER_MODE11_1 |	// Alternate Fnc - UART3_RX - DIM2336 RXD
					GPIO_MODER_MODE12_1 |	// Alternate Fnc - UART5_TX - DIM1324 TXD
					GPIO_MODER_MODE13_0 |	// Output Mode - SDTA
					GPIO_MODER_MODE14_0 |	// Output Mode - SDTB
					GPIO_MODER_MODE15_0;	// Output Mode - SDTC

	GPIOC->OTYPER = 0;						// NO OPEN DRAIN
	GPIOC->OSPEEDR = GPIO_OSPEEDR_OSPEED12;		//
	GPIOC->PUPDR =	0;
	GPIOC->AFR[0] = (7<<GPIO_AFRL_AFSEL6_Pos) |	// PC6 = UART6_TX
					(7<<GPIO_AFRL_AFSEL7_Pos) |	// PC7 = UART6_RX
					0;
	GPIOC->AFR[1] = (7<<GPIO_AFRH_AFSEL10_Pos) |	// PC10 = UART3_TX
					(7<<GPIO_AFRH_AFSEL11_Pos) |	// PC11 = UART3_RX
					(8<<GPIO_AFRH_AFSEL12_Pos) |	// PC12 = UART5_TX
					0;
	GPIOC->ODR = 0x00;

//-------------------------------------------------------------------------------------
// portD
//-------------------------------------------------------------------------------------
	GPIOD->MODER = 	GPIO_MODER_MODE2_1 |	// Alternate Fnc - UART5_RX - DIM1324 RXD
					0;

	GPIOD->OTYPER = 0;						// NO OPEN DRAIN
	GPIOD->OSPEEDR = 0;						// ALL LOW SPEED
	GPIOD->PUPDR =	0;
	GPIOD->AFR[0] = (8<<GPIO_AFRL_AFSEL2_Pos) |	// PD2 = UART5_RX
					0;
	GPIOD->AFR[1] = 0;
	GPIOD->ODR = 0x00;

//-------------------------------------------------------------------------------------
// portH
//-------------------------------------------------------------------------------------
	GPIOH->MODER = 	GPIO_MODER_MODE0_0 |	// Output Mode - SDT
					GPIO_MODER_MODE1_0 |	// Output Mode - OLED CS
					0;

	GPIOH->OTYPER = 0;						// NO OPEN DRAIN
	GPIOH->OSPEEDR = 0;						// ALL LOW SPEED
	GPIOH->PUPDR =	0;
	GPIOH->AFR[0] = 0;
	GPIOH->AFR[1] = 0;
	GPIOH->ODR = 0x00;
}

void bl_initVariables(void)
{
	blRxFramesCnt = 0;
	blRdmPresentLED = 10;	// short blink LED after start

	// init TIM4 for display refresh
	// 64MHz is input clock
	RCC->APB1LENR |= RCC_APB1LENR_TIM7EN;
	TIM7->PSC = 640;	// -> resulting clock is 100kHz
	TIM7->ARR = 1250;	// -> period 12.5mS = 80Hz refresh rate for display
	TIM7->CR1 = TIM_CR1_CEN;

	blFlashFrameActual =
	blFlashFramesTotal =
	blFlashSectorActual =
	blFlashSectorsTotal = 0;
}

