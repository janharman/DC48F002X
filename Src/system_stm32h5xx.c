/**
  ******************************************************************************
  * @file    system_stm32h5xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M33 Device Peripheral Access Layer System Source File
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32h5xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *   After each device reset the HSI (64 MHz) is used as system clock source.
  *   Then SystemInit() function is called, in "startup_stm32h5xx.s" file, to
  *   configure the system clock before to branch to main program.
  *
  *   This file configures the system clock as follows:
  *=============================================================================
  *-----------------------------------------------------------------------------
  *        System Clock source                     | HSI
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                              | 64000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                                | 64000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                           | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB3 Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        HSI Division factor                     | 1
  *-----------------------------------------------------------------------------
  *        PLL1_SRC                                | No clock
  *-----------------------------------------------------------------------------
  *        PLL1_M                                  | Prescaler disabled
  *-----------------------------------------------------------------------------
  *        PLL1_N                                  | 129
  *-----------------------------------------------------------------------------
  *        PLL1_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL1_Q                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL1_R                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL1_FRACN                              | 0
  *-----------------------------------------------------------------------------
  *        PLL2_SRC                                | No clock
  *-----------------------------------------------------------------------------
  *        PLL2_M                                  | Prescaler disabled
  *-----------------------------------------------------------------------------
  *        PLL2_N                                  | 129
  *-----------------------------------------------------------------------------
  *        PLL2_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL2_Q                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL2_R                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL2_FRACN                              | 0
  *-----------------------------------------------------------------------------
  *        PLL3_SRC                                | No clock
  *-----------------------------------------------------------------------------
  *        PLL3_M                                  | Prescaler disabled
  *-----------------------------------------------------------------------------
  *        PLL3_N                                  | 129
  *-----------------------------------------------------------------------------
  *        PLL3_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL3_Q                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL3_R                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL3_FRACN                              | 0
  *-----------------------------------------------------------------------------
  *=============================================================================
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup STM32H5xx_system
  * @{
  */

/** @addtogroup STM32H5xx_System_Private_Includes
  * @{
  */

#include "stm32h5xx.h"

/**
  * @}
  */

/** @addtogroup STM32H5xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H5xx_System_Private_Defines
  * @{
  */

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    (25000000UL) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (CSI_VALUE)
  #define CSI_VALUE    (4000000UL)  /*!< Value of the Internal oscillator in Hz*/
#endif /* CSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    (64000000UL) /*!< Value of the Internal oscillator in Hz */
#endif /* HSI_VALUE */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00U /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32H5xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H5xx_System_Private_Variables
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
  uint32_t SystemCoreClock = 200000000U;

  const uint8_t  AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
  const uint8_t  APBPrescTable[8] =  {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};

void SystemInit(void)
{
  uint32_t reg_opsr;

  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
   SCB->CPACR |= ((3UL << 20U)|(3UL << 22U));  /* set CP10 and CP11 Full Access */
  #endif

  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR = RCC_CR_HSION;

  /* Reset CFGR register */
  RCC->CFGR1 = 0U;
  RCC->CFGR2 = 0U;

  /* Reset HSEON, HSECSSON, HSEBYP, HSEEXT, HSIDIV, HSIKERON, CSION, CSIKERON, HSI48 and PLLxON bits */
#if defined(RCC_CR_PLL3ON)
  RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_HSECSSON | RCC_CR_HSEBYP | RCC_CR_HSEEXT | RCC_CR_HSIDIV | RCC_CR_HSIKERON | \
               RCC_CR_CSION | RCC_CR_CSIKERON |RCC_CR_HSI48ON | RCC_CR_PLL1ON | RCC_CR_PLL2ON | RCC_CR_PLL3ON);
#else
  RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_HSECSSON | RCC_CR_HSEBYP | RCC_CR_HSEEXT | RCC_CR_HSIDIV | RCC_CR_HSIKERON | \
               RCC_CR_CSION | RCC_CR_CSIKERON |RCC_CR_HSI48ON | RCC_CR_PLL1ON | RCC_CR_PLL2ON);
#endif

  /* Reset PLLxCFGR register */
  RCC->PLL1CFGR = 0U;
  RCC->PLL2CFGR = 0U;
#if defined(RCC_CR_PLL3ON)
  RCC->PLL3CFGR = 0U;
#endif /* RCC_CR_PLL3ON */

  /* Reset PLL1DIVR register */
  RCC->PLL1DIVR = 0x01010280U;
  /* Reset PLL1FRACR register */
  RCC->PLL1FRACR = 0x00000000U;
  /* Reset PLL2DIVR register */
  RCC->PLL2DIVR = 0x01010280U;
  /* Reset PLL2FRACR register */
  RCC->PLL2FRACR = 0x00000000U;
#if defined(RCC_CR_PLL3ON)
  /* Reset PLL3DIVR register */
  RCC->PLL3DIVR = 0x01010280U;
  /* Reset PLL3FRACR register */
  RCC->PLL3FRACR = 0x00000000U;
#endif /* RCC_CR_PLL3ON */

  /* Reset HSEBYP bit */
  RCC->CR &= ~(RCC_CR_HSEBYP);

  /* Disable all interrupts */
  RCC->CIER = 0U;

  /* Configure the Vector Table location add offset address ------------------*/
  #ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM1_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
  #else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
  #endif /* VECT_TAB_SRAM */

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


