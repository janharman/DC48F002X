/*
 * dc4_Flash.h
 *
 *  Created on: 30. 9. 2018
 *      Author: J�n Harman
 */

#ifndef DC5_FLASH_H_
#define DC5_FLASH_H_

void SaveSetupParametersToFLASH(unsigned int addr, int * var);
void SaveToFlash_Thread(void);

#endif /* DC5_FLASH_H_ */
