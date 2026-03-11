/*
 * eil_Flash.c
 *
 *  Created on: 30. 9. 2018
 *      Author: J�n Harman
 */

#include "stm32h5xx.h"
#include <string.h>
#include <dc48_Globals.h>
//#include <dc48_RdmDef.h>
//#include <dc48_Rdm.h>

unsigned int					Flash_Address_To_Write_To;
unsigned int					SaveToFlash_DelayCtc;
#define FLASH_KEY1				0x45670123
#define FLASH_KEY2				0xCDEF89AB

int Flash_Operation_Success(void)
{
	int ret = 1;
	while (FLASH->NSSR & FLASH_SR_BSY){};
	if (FLASH->NSSR & (FLASH_SR_WRPERR | FLASH_SR_PGSERR)) ret = 0;
	//while (!(FLASH->SR & FLASH_SR_EOP)){};
	// then, clear all status bits
	FLASH->NSSR = FLASH_SR_EOP | FLASH_SR_WRPERR | FLASH_SR_PGSERR;
	return ret;
}

/***************************************************************************************
 * WRITING TO FLASH
 *
 * Zapisuje se vzdy cely sector, to je 8 kB.
 * Jedna davka zapisu je 16 Bytu = 128 bitu.
 * Cas zapisu celeho sektoru je cca 3..4 ms.
 ***************************************************************************************/

void Flash_Save_To_Flash(void)
{
//	SavingToFlashActive = 1;

	// Check proper address
	if (!(Flash_Address_To_Write_To & 0x40000)) return;
	int sector = Flash_Address_To_Write_To >> 13;	// shift by 8k due to 8k sector size
	sector &= 0x07;
	if (sector >= 4) return;	// sectors 4..7 are for bootloader

	// Check if Flash is ready to operation (BSY is not set)
	if (FLASH->NSSR & FLASH_SR_BSY) return;

	// Clear all status bits
	FLASH->NSSR = FLASH_SR_EOP | FLASH_SR_WRPERR | FLASH_SR_PGSERR;

	// Unlock flash if needed
	if((FLASH->NSCR & FLASH_CR_LOCK) != RESET)
	{
	    FLASH->NSKEYR = FLASH_KEY1;
	    FLASH->NSKEYR = FLASH_KEY2;
	}

	unsigned int * src = 0;
	unsigned int datacnt = 0;
	switch (Flash_Address_To_Write_To)
	{
	case WRITE_TO_FLASH__PATCH_DIMMERS:
			src = (unsigned int *)&Patch;
			datacnt = sizeof(Patch);
			break;
	case WRITE_TO_FLASH__UNIT_SETTINGS:
			src = (unsigned int *)&UnitSettings;
			datacnt = sizeof(UnitSettings);
			break;
	case WRITE_TO_FLASH__WHRS_SETTINGS:		// nepouzito
//			src = (unsigned int *)&WorkHours10min;
//			datacnt = sizeof(WorkHours10min);
			break;
	case WRITE_TO_FLASH__DIMMERS_SETTS:
			src = (unsigned int *)&DimmerSetup;
			datacnt = sizeof(DimmerSetup);
			break;
	case WRITE_TO_FLASH__RDM_SETTINGS:
//			src = (unsigned int *)&RdmSetup;
//			datacnt = sizeof(RdmSetup);
			break;
	default: return;
	}
	if (!datacnt || !src) return;

	// erase desired flash page
	FLASH->NSCR = FLASH_CR_BKSEL | (sector << FLASH_CR_SNB_Pos) | FLASH_CR_SER;
	FLASH->NSCR = FLASH_CR_BKSEL | (sector << FLASH_CR_SNB_Pos) | FLASH_CR_SER | FLASH_CR_START;
	if (!Flash_Operation_Success()) return;
	FLASH->NSCR = 0;

	// write to flash
	FLASH->NSCR = FLASH_CR_PG;
	unsigned int cnt = (datacnt + 15) >> 4;	// 128 bit words
	__IO uint32_t* addr = (__IO uint32_t*)Flash_Address_To_Write_To;
	while (cnt--)
	{
		*addr++ = *src++;
		*addr++ = *src++;
		*addr++ = *src++;
		*addr++ = *src++;
		if (!Flash_Operation_Success()) return;
	}
	FLASH->NSCR = FLASH_CR_LOCK;
//	SavingToFlashActive = 0;
}

void SaveSetupParametersToFLASH(unsigned int addr, int * var)
{
	Flash_Address_To_Write_To = addr;
	SaveToFlash_DelayCtc = 12;

//	if (var)
//		Write_To_MessageQueue_by_VarAddress(var);
}

void SaveToFlash_Thread(void)
{
	if (Flash_Address_To_Write_To)
	{
		if (SaveToFlash_DelayCtc)
		{
			SaveToFlash_DelayCtc--;
			return;
		}
		Flash_Save_To_Flash();
		Flash_Address_To_Write_To = 0;
	}
}

