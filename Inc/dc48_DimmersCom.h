/*
 * dc48_DimmersCom.h
 *
 *  Created on: 9. 2. 2026
 *      Author: harma
 */

#ifndef INC_DC48_DIMMERSCOM_H_
#define INC_DC48_DIMMERSCOM_H_

void ProcessDimmerFrequency(unsigned int start, unsigned int end);
void ProcessDimmersRxData(int grp, char * rx);
void DimmersCommTimed(void);

#endif /* INC_DC48_DIMMERSCOM_H_ */
