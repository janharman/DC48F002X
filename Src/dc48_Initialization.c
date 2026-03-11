/*
 * dc48_Initialization.c
 *
 *  Created on: Jan 28, 2026
 *      Author: harman
 */

#include <stm32h5xx.h>
#include <string.h>
#include "dc48_Globals.h"
#include "OLED_CTRL_2864ASYDT01.h"
#include "dc48_RdmDef.h"

/* Clock setup to 200 MHz
 * Same code (only few constant different) as for DC5 - STM32H503
 *
 * Overwiev:
 * ---------
 * hsi_ck 								64 MHz
 * hsi_ker_ck							32 MHz
 * pll1_p_ck							200 MHz
 * pll1 meziobvod						400 MHz
 */
void iniSystem(void)
{
	ICACHE->SR = ICACHE_CR_WAYSEL | ICACHE_CR_EN;
	PWR->VOSCR = PWR_VOSCR_VOS;	// Voltage Scaling Control Register
	while(!READ_BIT(PWR->VOSSR, PWR_VOSSR_VOSRDY)) {}

	RCC->CR = RCC_CR_HSION | RCC_CR_HSIDIV_0;					// hsi_ker_ck = 64/2 = 32 MHz
	RCC->HSICFGR = 0x40 << RCC_HSICFGR_HSITRIM_Pos;	// 0x40 is default value
	while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U){}

	// ----------------------------------------------------------------------------------------
	// PLL 1				P = 200 MHz
	//						Q = 10 MHz
	// ----------------------------------------------------------------------------------------
	RCC->PLL1CFGR = RCC_PLL1CFGR_PLL1SRC_0 + (2 << RCC_PLL1CFGR_PLL1M_Pos);	// DIVISION BY 2
	RCC->PLL1FRACR = 0x1F00 << 3;	// Fractional part of the N multiplicator
	// PLL must be disabled (PLL1ON = 0 and PLL1RDY = 0)
	// Resulting Frequency is SYS_CLK = (hsi_ker_ck/M)*N/P = (32/2)*25/2 = 16*25/2 = 200
	// N = 23+1, but fractional part 0x1F00/0x1FFF=0.9689 is added
	RCC->PLL1DIVR = (23 << RCC_PLL1DIVR_PLL1N_Pos)				// N
				  + (1 << RCC_PLL1DIVR_PLL1P_Pos)				// P	pll1_p_ck => 400M/2 = 2M
				  + (39 << RCC_PLL1DIVR_PLL1Q_Pos)				// Q	pll1_q_ck => 400M/40 = 10M
				  + (127 << RCC_PLL1DIVR_PLL1R_Pos)				// R
				  ;
	RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1FRACEN;
	RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1RGE_0 | RCC_PLL1CFGR_PLL1VCOSEL;
	RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1PEN | RCC_PLL1CFGR_PLL1QEN;
	RCC->CR |= RCC_CR_PLL1ON;
	while (READ_BIT(RCC->CR, RCC_CR_PLL1RDY) == 0U){}

	FLASH->ACR = 0x15;	// FLASH LATENCY  + WRHIGHFREQ
	RCC->CFGR1 = RCC_CFGR1_SW;
	RCC->CFGR2 = (0b101 << RCC_CFGR2_PPRE2_Pos) |	// APB2 PRESCALER -> rcc_pclk2 = rcc_hclk1 / 4 = 50 MHz
				 (0b110 << RCC_CFGR2_PPRE3_Pos);	// APB3 PRESCALER -> rcc_pclk3 = rcc_hclk1 / 8 = 25 MHz

	// ----------------------------------------------------------------------------------------
	// PLL 2				1.181 MHz - NOT USED
	// ----------------------------------------------------------------------------------------
	RCC->PLL2CFGR = RCC_PLL1CFGR_PLL1SRC_0 + (32 << RCC_PLL2CFGR_PLL2M_Pos);	// DIVISION BY 32
	// PLL must be disabled (PLL1ON = 0 and PLL1RDY = 0)
	// Resulting Frequency is SYS_CLK = (hsi_ker_ck/M)*N/P = (32/32)*160/160 = 16*25/2 = 200
	// N = 23+1, but fractional part 0x1F00/0x1FFF=0.9689 is added
	RCC->PLL2DIVR = (149 << RCC_PLL1DIVR_PLL1N_Pos)				// N	1 * 160 = 160 MHz
				  + (127 << RCC_PLL1DIVR_PLL1P_Pos)				// P	150 / 127 = 1181 kHz
				  + (127 << RCC_PLL1DIVR_PLL1Q_Pos)				// Q	disabled
				  + (127 << RCC_PLL1DIVR_PLL1R_Pos)				// R	disabled
				  ;
	RCC->PLL2CFGR |= RCC_PLL2CFGR_PLL2VCOSEL;
	RCC->PLL2CFGR |= RCC_PLL2CFGR_PLL2PEN;
//	RCC->CR |= RCC_CR_PLL2ON;
//	while (READ_BIT(RCC->CR, RCC_CR_PLL2RDY) == 0U){}
}

void iniSysTick(void)
{
	// Hodnota: (200 000 000 / 10 000) - 1 = 19999
	SysTick->LOAD = (CORE_FREQUENCY / SYS_TICK_FREQUENCY) - 1;

	// 3. Nastavení priorit (volitelné, 0 je nejvyšší)
	// U Cortex-M33 je priorita SysTicku v registru SHPR[2]
	NVIC_SetPriority(SysTick_IRQn, 0);

	// 4. Konfigurace a spuštění:
	// Bit 2: CLKSOURCE = 1 (vnitřní takt procesoru)
	// Bit 1: TICKINT = 1 (povolit přerušení)
	// Bit 0: ENABLE = 1 (spustit čítač)
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk   |
					SysTick_CTRL_ENABLE_Msk;
}

void iniPorts(void)
{
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN |
					RCC_AHB2ENR_GPIOBEN |
					RCC_AHB2ENR_GPIOCEN |
					RCC_AHB2ENR_GPIODEN |
					RCC_AHB2ENR_GPIOHEN);
	(void)RCC->AHB2ENR;	// so that port clock are stabilized

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

void iniVariables(void)
{
	memcpy(Patch, 				FLASH_Patch, 			sizeof(Patch));
	memcpy(&UnitSettings,	 	&FLASH_UnitSettings, 	sizeof(UnitSettings));
	memcpy(&RdmSetup, 			&FLASH_RdmSetup,	 	sizeof(RdmSetup));
	memcpy(&WorkHours10min,		&FLASH_WorkHours10min, 	sizeof(WorkHours10min));
	memcpy(&DimmerSetup,		&FLASH_DimmerSetup, 	sizeof(DimmerSetup));
	memset(&SerialDimmer,		0, 						sizeof(SerialDimmer));
	EthIpChangingMenuIndex = 3;	// for change IP address part D
}


