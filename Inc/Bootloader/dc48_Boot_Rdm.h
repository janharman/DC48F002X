/*
 * dc4_Boot_Rdm.h
 *
 *  Created on: Apr 9, 2024
 *      Author: harma
 */

#ifndef BOOTLOADER_DC4_BOOT_RDM_H_
#define BOOTLOADER_DC4_BOOT_RDM_H_

//void bl_ForceResponseHwFwInfo(void);
void bl_ProcessRxData(int size);
void bl_InitializationRDM(void);

#endif /* BOOTLOADER_DC4_BOOT_RDM_H_ */
