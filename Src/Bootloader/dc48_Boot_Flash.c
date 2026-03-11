/*
 * dc4_Boot_Flash.c
 *
 *  Created on: 19. 3. 2023
 *      Author: Jan Harman
 */

#include <string.h>
#include "stm32h5xx.h"
#include <dc48_Boot_Fnc.h>
#include <dc48_Boot_Glob.h>
#include "dc48_Globals.h"

#define BOOT_FLASH_SECTOR_SIZE			8192
#define BOOT_FLASH_RDN_PART_SIZE		208
#define BOOT_FLASH_RDN_PART_REST		176

//unsigned int bl_FlashWrBuffer[BOOT_FLASH_SECTOR_SIZE/4+BOOT_FLASH_SECTOR_SIZE];	// 2kB buffer - je tady o jeden sektor vic, kvuli kopirovani
unsigned int bl_FlashWrBuffAddr;
//unsigned int bl_FlashWrBuffFill;
#define FLASH_KEY1				0x45670123
#define FLASH_KEY2				0xCDEF89AB

int bl_Flash_Operation_Success(void)
{
	int ret = 1;
	while (FLASH->NSSR & FLASH_SR_BSY){};
	if (FLASH->NSSR & (FLASH_SR_WRPERR | FLASH_SR_PGSERR)) ret = 0;
	//while (!(FLASH->SR & FLASH_SR_EOP)){};
	// then, clear all status bits
	FLASH->NSSR = FLASH_SR_EOP | FLASH_SR_WRPERR | FLASH_SR_PGSERR;
	return ret;
}

/* ----------------------------------------------------------------------------------------------
 * FLASH_SECTOR_SIZE = 8 kB = 0x2000
 *
 *
 */

int sector_erased;

void bl_Flash_Write_to_Flash(unsigned int addr, unsigned int * d, int cnt)
{
	SavingToFlashActive = 1;

	// Check proper address
	if (addr > BOOT_FW_FW_ADR_LAST_ADDRESS) return;
	int base = addr & (FLASH_SECTOR_SIZE-1);
	int sector = (addr >> 13) & 0x07F;	// shift by 8k due to 8k sector size
	if (sector >= 0x1F) return;	// sectors 32 and more are from Bank 1 => it is not allowed to write to bank in which bootloader resides

	// Check if Flash is ready to operation (BSY is not set)
	if (FLASH->NSSR & FLASH_SR_BSY) return;

	// Clear all status bits
	FLASH->NSSR = FLASH_SR_EOP | FLASH_SR_WRPERR | FLASH_SR_PGSERR | FLASH_SR_STRBERR;

	// Unlock flash if needed
	if((FLASH->NSCR & FLASH_CR_LOCK) != RESET)
	{
	    FLASH->NSKEYR = FLASH_KEY1;
	    FLASH->NSKEYR = FLASH_KEY2;
	}

	// erase desired flash sector if writing is on zero base 0x0000
	// sector_erased != sector it means that this sector is writen for the first time -> so we have to erase sector
	if ((base == 0) || (sector_erased != sector))
	{
		sector_erased = sector;
		FLASH->NSCR = (0 << FLASH_CR_BKSEL_Pos) | (sector << FLASH_CR_SNB_Pos) | FLASH_CR_SER;
		FLASH->NSCR = (0 << FLASH_CR_BKSEL_Pos) | (sector << FLASH_CR_SNB_Pos) | FLASH_CR_SER | FLASH_CR_START;
		if (!bl_Flash_Operation_Success()) return;
		FLASH->NSCR = 0;
	}

	// write to flash
	FLASH->NSCR = FLASH_CR_PG;
	unsigned int * a = (unsigned int *)addr;
	while (cnt--)
	{
		*(__IO uint32_t*)a++ = *d++;
		*(__IO uint32_t*)a++ = *d++;
		*(__IO uint32_t*)a++ = *d++;
		*(__IO uint32_t*)a++ = *d++;
		if (!bl_Flash_Operation_Success()) return;
	}
	FLASH->NSCR = FLASH_CR_LOCK;
	SavingToFlashActive = 0;
}



