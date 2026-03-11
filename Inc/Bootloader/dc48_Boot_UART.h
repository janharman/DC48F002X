/*
 * dc5_Boot_UART.h
 *
 *  Created on: 28.4.2025
 *      Author: harman Jan
 */

#ifndef BOOTLOADER_DC5_BOOT_UART_H_
#define BOOTLOADER_DC5_BOOT_UART_H_

void bl_SendRdmTxFrame(char * b, int cnt);
void bl_UartThread(void);
void bl_initializationUartXLR(void);

#endif /* BOOTLOADER_DC5_BOOT_UART_H_ */
