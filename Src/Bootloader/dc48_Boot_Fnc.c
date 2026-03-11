/*
 * dc4_Boot_Fnc.c
 *
 *  Created on: Feb 27, 2024
 *      Author: harma
 */

#include <dc48_Boot_Disp.h>
#include <dc48_Boot_Flash.h>
#include <dc48_Boot_Fnc.h>
#include <dc48_Boot_Glob.h>
#include "stm32h5xx.h"

/* ----------- Pointer to addresses --------------------------------------------------------
 * This pointer is initialized in BL_Reset_Handler_Main and points to start of the section flash_prgadr.
 * It is address 0x0800F000.
 * The section data are stored at compile time and are defined in stm32f30_flash.ld file.
 -------------------------------------------------------------------------------------------*/
const tFirmwareAddresses * FirmwareAddresses;

/* ----------- Shared function in fixed position in bootloader section.---------------------
 *
 * This is the only function to be called from main fw, because of this is on fixed address.
 * The function call other bootloader functions according to fnccode.
 -------------------------------------------------------------------------------------------*/

__attribute__((section(".flash_BootFnc"))) void bl_Function(unsigned int fnccode)
{
	if (fnccode == 1)
		blRxProcessFrame = bl_CountPrgCrc();
}

unsigned int bl_CountPrgCrc(void)
{
	unsigned int * fwa = FirmwareAddresses->FwStart;
	unsigned int fws = FirmwareAddresses->FwSize >> 2;
	if (fwa != (void*)0x08000000) return 0;	// fw must start at 0x8000000
	if (fws > (0x20000 >> 2)) return 0;	// if FW size is higher 128 kB
	unsigned int crc = 0;
	while (fws--)
		crc ^= *fwa++;
	return crc;
}

void bl_memcpyi(unsigned int * dst, unsigned int * src, int cnt)
{
	while (cnt--) *dst++ = *src++;
}

__attribute__((section(".flash_jump_to_prg"))) void bl_JumpTo_Reset_Handler(void)
{
	__asm ("b Reset_Handler");
}

