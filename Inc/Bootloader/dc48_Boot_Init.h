/*
 * dc4_Boot_Init.h
 *
 *  Created on: Feb 28, 2024
 *      Author: harma
 */

#ifndef BOOTLOADER_DC4_BOOT_INIT_H_
#define BOOTLOADER_DC4_BOOT_INIT_H_

void bl_SystemInit(void);
void bl_initClock(void);
void bl_initPins(void);
void bl_initVariables(void);

#endif /* BOOTLOADER_DC4_BOOT_INIT_H_ */
