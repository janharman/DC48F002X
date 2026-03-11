/*
 * dc5_Boot_Flash.h
 *
 *  Created on: 29.4.2025
 *      Author: harman jan
 */

#ifndef BOOTLOADER_DC5_BOOT_FLASH_H_
#define BOOTLOADER_DC5_BOOT_FLASH_H_

extern unsigned int bl_FlashWrBuffer[512];	// 2kB buffer

void bl_Flash_Write_to_Flash(unsigned int addr, unsigned int * d, int cnt);

#endif /* BOOTLOADER_DC5_BOOT_FLASH_H_ */
