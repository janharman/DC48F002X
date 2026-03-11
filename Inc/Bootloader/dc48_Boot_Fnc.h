/*
 * dc4_Boot_Fnc.h
 *
 *  Created on: Feb 27, 2024
 *      Author: harma
 */

#ifndef BOOTLOADER_DC4_BOOT_FNC_H_
#define BOOTLOADER_DC4_BOOT_FNC_H_

typedef struct
{
	unsigned int *		FwStart;
	unsigned int		FwSize;
	unsigned int *		ImgStart;
	unsigned int *		ImgEnd;
	unsigned int		ImgSize;
	void *				Bl_Function;
	unsigned int		FlashUnitSettings;
	unsigned int *		Reset_Handler;
	unsigned int *		BL_Reset_Handler;
} tFirmwareAddresses;

extern unsigned int _prg_addresses;		// declared in *.ld file

extern const tFirmwareAddresses * FirmwareAddresses;

void bl_Function(unsigned int fnccode);
unsigned int bl_CountPrgCrc(void);
void bl_JumpTo_Reset_Handler(void);
void bl_memcpyi(unsigned int * dst, unsigned int * src, int cnt);

#endif /* BOOTLOADER_DC4_BOOT_FNC_H_ */
