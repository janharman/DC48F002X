/*
 * dc48_Tools.h
 *
 *  Created on: 2. 2. 2026
 *      Author: harma
 */

#ifndef INC_DC48_TOOLS_H_
#define INC_DC48_TOOLS_H_

int __byteToStr(unsigned short val, char *str);
void Fill_DisplayDimmerSettings(unsigned int dim_idx);
void Fan_Initialization(void);
void General_Timed_Function(void);
void Fan_CheckRotaion(void);

#endif /* INC_DC48_TOOLS_H_ */
