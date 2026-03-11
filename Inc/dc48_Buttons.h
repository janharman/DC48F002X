/*
 * Buttons.h
 *
 *  Created on: 15. 10. 2014
 *      Author: Jan
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

//****************************************************************
// #include MCU aware header files (e.g. reading port...)
#include "stm32h523xx.h"
//****************************************************************

extern unsigned int Buttons_LongPress;
extern unsigned int Buttons_Pressed;
extern unsigned int Buttons_Released;
extern unsigned int Buttons_Down;
extern unsigned int Buttons_LastPressed;

// Call every 10 ms
void Scan_Buttons(void);
// Call for restore autorepeat delay for all buttons
void RestoreAutorepeatDelay(void);

//****************************************************************
// Relocatable definitions
//
// Change #define expressions to fit to your MCU type
//****************************************************************

// Aliases for bits occupy buttons
#define	BUTTON_ESCAPE			0x08
#define BUTTON_DOWN				0x04
#define BUTTON_UP				0x02
#define BUTTON_ENTER			0x01
#define NO_KEY_PRESS_ACTION		0x80;

// Mask for buttons used for autorepeat function
#define AUTOREP_BTNS		(BUTTON_DOWN | BUTTON_UP)
#define ALL_BUTTONS_MASK	(AUTOREP_BTNS | BUTTON_ESCAPE | BUTTON_ENTER)

// Reading button SW1 = ESCAPE and shift it to position bit 0
#define BUTTON_SW1_STATUS	(((~GPIOB->IDR)>>7) & BUTTON_ESCAPE)
// Reading button SW234 {DN, UP, ENTER} and shift it to position bit 2
#define BUTTON_SW234_STATUS	((~GPIOB->IDR) & (BUTTON_DOWN | BUTTON_UP | BUTTON_ENTER))
// Reading all buttons
#define BUTTONS_STATUS		(BUTTON_SW1_STATUS | BUTTON_SW234_STATUS)

// Display Off Time if no key is pressed. The number is in 10 ms units. Number of seconds * 100
#define DISPLAY_OFF_TIME_NO_KEY_PRESS	1000

// Aliases for button actions
#define BUTTON_ESCAPE_PRESSED		(Buttons_Pressed & BUTTON_ESCAPE)
#define BUTTON_DOWN_PRESSED			(Buttons_Pressed & BUTTON_DOWN)
#define BUTTON_UP_PRESSED			(Buttons_Pressed & BUTTON_UP)
#define BUTTON_ENTER_PRESSED		(Buttons_Pressed & BUTTON_ENTER)
#define BUTTON_ANY_PRESSED			(Buttons_Pressed & ALL_BUTTONS_MASK)
#define LAST_BUTTON_ENTER_PRESSED		(Buttons_LastPressed & BUTTON_ENTER)

//****************************************************************

#endif /* BUTTONS_H_ */
