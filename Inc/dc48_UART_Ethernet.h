/*
 * dc48_UART_Ethernet.h
 *
 *  Created on: 18. 12. 2019
 *      Author: harma
 */

#ifndef DC4_UART_ETHERNET_H_
#define DC4_UART_ETHERNET_H_

/* Global Variables and MACROS */
#define NUMBER_OF_REC_DMX_FRAMES		513
#define ETH_RX_BUFFER_SIZE				516

extern unsigned int		Eth_DisplayIpAddress[4];		// Ip adresa pro display a zmenu

void Initialization_Uart_ETHETNET(void);
void Disable_Uart_ETHERNET(void);
int Uart_ETHERNET_Enabled(void);

#endif /* DC4_UART_ETHERNET_H_ */
