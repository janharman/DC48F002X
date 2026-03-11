/*
 * Buttons.c
 *
 *  Created on: 15. 10. 2014
 *      Author: Jan
 */

#include <dc48_Buttons.h>
#include <stdint.h>

/* Project Includes */
#include "dc48_Globals.h"

// Global Variables
unsigned int Buttons_LongPress;
unsigned int Buttons_Pressed;
unsigned int Buttons_Released;
unsigned int Buttons_Down;
unsigned int Buttons_LastPressed;

// Local variables
unsigned int ButtonsOld;			// previous scan value
unsigned int ButtonsEn;			// enabling for pressing
unsigned int ButtonsEnRel;			// enabling for released buttons
unsigned int SameButtonsCnt;		// counter for long press delay
unsigned int ButtonsAutoRepCnt;	// counter for autorepeating
unsigned int AutorepRel;			// realoading value for autorepeat counter
unsigned int AutorepDel;			// delay counter for autorepeat counter
unsigned int ButtonsPrev;			// previous pressed buttons
unsigned int ButtonsNoPressCnt;	// No pressed button cnt. Reloaded after button is pressing.
unsigned int ButtonsFilterCnt;		// Filter pressing buttons
#define BUTTONS_FILTER_DEPTH		4

//
// Called each 10 ms interval
//
// Immediatelly after this function, function for evaluating buttons should be called;
//
void Scan_Buttons(void)
{
	unsigned char btnAct, but;

	Buttons_LongPress = 0;
	Buttons_Pressed = 0;
	Buttons_Released = 0;

	// Read actual buttons status
	btnAct = (BUTTONS_STATUS & ALL_BUTTONS_MASK);
	if (ButtonsOld == btnAct)
	{
			if (ButtonsFilterCnt >= BUTTONS_FILTER_DEPTH)
			{
					Buttons_Down = btnAct;
					Buttons_Pressed = btnAct & ButtonsEn;
					Buttons_Released = ~btnAct & ButtonsEnRel;
					SameButtonsCnt++;

					if (btnAct)
					{
							ButtonsNoPressCnt = DISPLAY_OFF_TIME_NO_KEY_PRESS;
							if (SameButtonsCnt > 50)
							{
									Buttons_LongPress = btnAct;
									SameButtonsCnt = 0;
									ButtonsEnRel = 0;
							}

							if (ButtonsAutoRepCnt)
							{
									ButtonsAutoRepCnt--;
									if (ButtonsAutoRepCnt == 1)
											if (btnAct & AUTOREP_BTNS)
											{
												Buttons_Pressed = btnAct & AUTOREP_BTNS;
												ButtonsAutoRepCnt = AutorepRel;
												if (AutorepRel > 5)
												{
														if (!--AutorepDel)
														{
																AutorepDel = 4;
																AutorepRel--;
														}
												}
											}
							}
							else //	if (!ButtonsAutoRepCnt)
							if (Buttons_Pressed & AUTOREP_BTNS)
							{
								ButtonsAutoRepCnt = 60; // 600 ms first delay to repeat
								AutorepRel = 12; 		// 120 ms next delay
								AutorepDel = 6;
							}
					}
					else
					{
							ButtonsEnRel = 0;
							if (ButtonsNoPressCnt)
								if (!--ButtonsNoPressCnt)
									Buttons_Released |= NO_KEY_PRESS_ACTION;
					}

					ButtonsEn = ~btnAct;
					but = (ButtonsPrev ^ btnAct) & btnAct;
					if (but) ButtonsEnRel = but;
					ButtonsPrev = btnAct;
			}
			else
					ButtonsFilterCnt++;
	}
	else
	{
			SameButtonsCnt = 0;
			ButtonsAutoRepCnt = 0;
			ButtonsFilterCnt = 0;
	}
	ButtonsOld = btnAct;
	if (Buttons_Pressed) Buttons_LastPressed = Buttons_Pressed;
}

void RestoreAutorepeatDelay(void)
{
	ButtonsAutoRepCnt = 0;
}

