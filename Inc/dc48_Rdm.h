/*
 * a2Rdm.h
 *
 *  Created on: 11. 1. 2016
 *      Author: J�n Harman
 */

#ifndef A3RDM_H_
#define A3RDM_H_

#define QUEUED_MESSAGES_BUFF_MAX		128
extern unsigned int			RDM_QueuedMessages[QUEUED_MESSAGES_BUFF_MAX];// Message Queue
extern unsigned int			RDM_QueuedMsgPtr;		// Pointer to first free message record. If zero, no message is in the queue
extern int					RDM_WriteCalibrationDataConfirm;
extern int					RDM_WriteDataError;

void Process_DMX_RS485_RDM_Data_Packet(void);
void Initialization_RDM(void);
void Write_To_MessageQueue_by_VarAddress(unsigned int * varaddr);

#endif /* A3RDM_H_ */
