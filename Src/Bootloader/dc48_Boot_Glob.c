/*
 * dc4_Boot_Glob.c
 *
 *  Created on: Mar 26, 2024
 *      Author: harma
 */

#include <dc48_Boot_Glob.h>

char							blRxBuffer[BOOT_RX_BUFFER_SIZE];
int								blRxProcessFrame;
int								blRxFramesCnt;
unsigned int					blDeviceUID;
unsigned int					blRdmTxFramePrepared;
unsigned int					blRdmPresentLED;
int								blFlashFramesTotal;
int								blFlashFrameActual;
int								blFlashSectorActual;
int								blFlashSectorsTotal;
unsigned int					blCircularCounter;
unsigned int					blFlashingInProgressCtc;

__attribute__((section(".flash_BootVer"))) const tBootLdrVersion BootLdrVer = {
	/* Version			*/	DC48_BOOTLOADER_VERSION,
	/* Date				*/	DC48_BOOTLOADER_DATE,
	/* Text				*/	"DC48-Bootloader " DC48_BOOTLOADER_VER_TXT "-" DC48_BOOTLOADER_DATE_TXT
};

__attribute__((section(".flash_PrgCrc"))) volatile const tBootLdrFwCrc BootLdrFwCrc = {
	/* CrcAssigned		*/	PROGRAM_CRC_NEED_TO_ASSIGN,
	/* Crc				*/	0x11111111
};

