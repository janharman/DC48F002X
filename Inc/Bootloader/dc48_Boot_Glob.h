/*
 * dc5_Boot_Glob.h
 *
 *  Created on: 28.4.2025
 *      Author: harman jan
 */

#ifndef BOOTLOADER_DC5_BOOT_GLOB_H_
#define BOOTLOADER_DC5_BOOT_GLOB_H_

#define	DC48_BOOTLOADER_VERSION			0x000B0009
#define DC48_BOOTLOADER_DATE			0x00260310
#define DC48_BOOTLOADER_VER_TXT			"B0009"
#define DC48_BOOTLOADER_DATE_TXT		"260310"
#define DC48_BOOTLOADER_ID_VER			0x80000000		// Identifikuje ze odpovida bootloader

#define BOOT_RX_BUFFER_SIZE				520

#define PROGRAM_CRC_STS_ASSIGNED		0x1CAC2ACA
#define PROGRAM_CRC_NEED_TO_ASSIGN		0xAB01CD02
#define GO_TO_BOOTLOADER_CODE			0x1234ABCD
#define GO_TO_BOOTLOADER_CODE_ADD		0x08a0cd12

#define BOOT_FW_START_ADDRESS			0x08000000
#define BOOT_FW_FW_ADR_LAST_ADDRESS		0x0803FFFF
#define BOOT_PROTOCOL_END				55
#define BOOT_PROTOCOL_VERSION			0

extern char								blRxBuffer[BOOT_RX_BUFFER_SIZE];
extern int								blRxProcessFrame;
extern int								blRxFramesCnt;
extern unsigned int						blDeviceUID;
extern unsigned int						blRdmTxFramePrepared;
extern unsigned int						blRdmPresentLED;
extern int								blFlashFramesTotal;
extern int								blFlashFrameActual;
extern int								blFlashSectorActual;
extern int								blFlashSectorsTotal;
extern unsigned int						blCircularCounter;
extern unsigned int						blFlashingInProgressCtc;

typedef struct
{
	unsigned int			Version;
	unsigned int			Date;
	char					Text[40];
} tBootLdrVersion;

typedef struct
{
	unsigned int		CrcAssigned;
	unsigned int		Crc;
} tBootLdrFwCrc;

extern const tBootLdrVersion	BootLdrVer;
extern volatile const tBootLdrFwCrc 	BootLdrFwCrc;



#endif /* BOOTLOADER_DC5_BOOT_GLOB_H_ */
