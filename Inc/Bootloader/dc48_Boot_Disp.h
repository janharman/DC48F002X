/*
 * dc4_Boot_Disp.h
 *
 *  Created on: Feb 28, 2024
 *      Author: harma
 */

#ifndef BOOTLOADER_DC4_BOOT_DISP_H_
#define BOOTLOADER_DC4_BOOT_DISP_H_

//void bl_Clear_Screen(void);
extern unsigned int blCommActive;
void bl_DisplayInit(void);
void bl_DisplayReset(void);
void bl_DisplayRefresh(void);

#endif /* BOOTLOADER_DC4_BOOT_DISP_H_ */
