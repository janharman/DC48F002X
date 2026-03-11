/*
 * dc4_Bootloader.c
 *
 *  Created on: Feb 27, 2024
 *      Author: harma
 */

#include "stm32h5xx.h"
#include <dc48_Boot_Glob.h>
#include <dc48_Boot_Init.h>
#include <dc48_Boot_Disp.h>
#include <dc48_Boot_Flash.h>
#include <dc48_Boot_Fnc.h>
#include <dc48_Boot_Rdm.h>
#include <dc48_Boot_UART.h>
#include "dc48_Globals.h"
#include "dc48_RdmDef.h"

unsigned int	CountCrc;
unsigned int	GreenFlash;
unsigned int	RedFlash;
unsigned int	DisplayRefreshCtc;

__attribute__((naked, section(".flash_BootFix"))) void BL_Reset_Handler(void)
{
	/* 1. Ruční nastavení Stack Pointeru pomocí Inline Assembleru */
    /* Předpokládáme, že symbol _estack je definován v .ld souboru */
    extern uint32_t _estack;
    __asm volatile (
        "ldr r0, =%0 \n"
        "mov sp, r0  \n"
        "b BL_Reset_Handler_Main \n"
        : : "i"(&_estack) : "r0", "memory"
	);
}

void BL_Reset_Handler_Main(void)
{
	bl_SystemInit();
	bl_initPins();
	unsigned int * gotoblcode = FlashWrBuf;
	unsigned int c = GO_TO_BOOTLOADER_CODE;
	int gotobl = 1;
	for (int i=0; i<4; i++)
	{
		if (gotoblcode[i] != c) gotobl = 0;
		c += GO_TO_BOOTLOADER_CODE_ADD;
		gotoblcode[i] = 0;
	}

	FirmwareAddresses = (tFirmwareAddresses *)&_prg_addresses;
//	gotobl = 1;
	if (!gotobl)	// if go to bootloader is not forced -> do standart starting procedure with CRC checking
	{
			CountCrc = bl_CountPrgCrc();
			LED_DEBUG_BLUE_OFF;
			LED_DEBUG_RED_ON;
			if (BootLdrFwCrc.CrcAssigned == PROGRAM_CRC_STS_ASSIGNED)
			{
				if (CountCrc == BootLdrFwCrc.Crc)
				{
//					__asm ("b Reset_Handler");
					bl_JumpTo_Reset_Handler();
				}
				else	// Bad assigned Crc -> go to Bootloader
				{

				}
			}
			else if (BootLdrFwCrc.CrcAssigned == PROGRAM_CRC_NEED_TO_ASSIGN)	// Crc is not assigned -> write Crc to FLASH
			{
				bl_memcpyi(FlashWrBuf, (void*)0x08020000, FLASH_SECTOR_SIZE>>2);
				FlashWrBuf[0x200>>2] = PROGRAM_CRC_STS_ASSIGNED;	// to je adresa 0x0800F800
				FlashWrBuf[(0x200>>2)+1] = CountCrc;
////				bl_Flash_Write_to_Flash(0x8020000, FlashWrBuf, 1);	// mandatory write due to erase sector
				bl_Flash_Write_to_Flash(0x08020000, &FlashWrBuf[0x0000>>2], 8);	// PRGADR 128 Bytes
				bl_Flash_Write_to_Flash(0x08020100, &FlashWrBuf[0x0100>>2], 2);	// JUMP 32 Bytes
				bl_Flash_Write_to_Flash(0x08020200, &FlashWrBuf[0x0200>>2], 1);	// CRC 16 Bytes
				bl_JumpTo_Reset_Handler();
			}
	}
	// program crc is not assigned or Crc is not equal stored value -> continue as Bootloader

	bl_initClock();
	bl_DisplayReset();
	bl_initVariables();

	bl_InitializationRDM();
	bl_initializationUartXLR();
	bl_DisplayInit();

//	if (gotobl)	// go to bootloader from normal fw. Response with DC4_PID_GET__HW_FW_INFO
//		bl_ForceResponseHwFwInfo();

	GreenFlash = 1;
	RedFlash = 1;
	DisplayRefreshCtc = 1;
	blFlashingInProgressCtc = 0;

	while (1)
	{
		bl_UartThread();
		if (blRxProcessFrame)
		{
			bl_ProcessRxData(blRxProcessFrame);
			blRxProcessFrame = 0;
			if (blRdmTxFramePrepared)
			{
				bl_SendRdmTxFrame((char *)&RDM_Packet, blRdmTxFramePrepared);
				blRdmTxFramePrepared = 0;
			}
		}

		if (TIM7->SR & TIM_SR_UIF)
			bl_DisplayRefresh();

		if (blFlashingInProgressCtc)
		{
			blFlashingInProgressCtc--;
			TIM3->CCR1 = 0;			// strida chan1
			TIM3->CCR2 = 3;			// strida chan2
		}
		else
		{
			TIM3->CCR1 = 3;			// strida chan1
			TIM3->CCR2 = 0;			// strida chan2
		}
		blCircularCounter++;
	}

	__asm ("b main");	// not used ever
}


