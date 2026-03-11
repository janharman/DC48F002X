/*
 * dc48_UART_Dimmers.h
 *
 *  Created on: Jan 30, 2026
 *      Author: harma
 */

#ifndef INC_DC48_UART_DIMMERS_H_
#define INC_DC48_UART_DIMMERS_H_

extern int					ReceivedDimmerData;

void CheckDimmersReceivedData(void);
void DimmersCommThread(void);
void DimmersCommInitialization(void);

#endif /* INC_DC48_UART_DIMMERS_H_ */
