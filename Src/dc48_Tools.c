/*
 * dc48_Tools.c
 *
 *  Created on: 2. 2. 2026
 *      Author: harma
 */

#include <stm32h5xx.h>
#include <string.h>
#include "dc48_Globals.h"

int __byteToStr(unsigned short val, char *str)
{
    int i = 0;

    // Pokud je hodnota 0, vyřešíme ji hned
    if (val == 0) {
        str[i++] = '0';
    } else {
        // Pro 0-255 máme max 3 cifry
        if (val >= 100) str[i++] = (val / 100) + '0';
        if (val >= 10)  str[i++] = ((val / 10) % 10) + '0';
        str[i++] = (val % 10) + '0';
    }

    str[i] = '\0'; // Ukončovací nula
    return i;
}

void Fill_DisplayDimmerSettings(unsigned int dim_idx)
{
	int i;
	unsigned int x = (dim_idx<NUM_OF_CHANNELS)?dim_idx:0;

	memcpy(&DispDimmSetup, &DimmerSetup[x], sizeof(DispDimmSetup));
	if (DispDimmSetup.Response > 2) DispDimmSetup.Response = 2;
	// tyto int muze uzivatel zmenit, bude je potom potreba pri zapisu prepsat do skutecne promenne
	DispDimmSetWZIR[0] = !!(DispDimmSetup.WorkL_ZiZ & DIMSET_WZ__WORKLIGHT_ON);
	DispDimmSetWZIR[1] = !!(DispDimmSetup.WorkL_ZiZ & DIMSET_WZ__ZERO_IS_ZERO);
	DispDimmSetWZIR[2] = !!(DispDimmSetup.Ts_Ru & DIMSET_TR__TRIAC_STYLE);
	DispDimmSetWZIR[3] = !!(DispDimmSetup.Ts_Ru & DIMSET_TR__RELAY_USING);
	// tyto znaky jsou jen pro zobrazeni na displeji
	DispDimmSetupWZIR[0] = DispDimmSetWZIR[0]?'W':'-';
	DispDimmSetupWZIR[1] = DispDimmSetWZIR[1]?'Z':'-';
	DispDimmSetupWZIR[2] = DispDimmSetWZIR[2]?'T':'-';
	DispDimmSetupWZIR[3] = DispDimmSetWZIR[3]?'R':'-';

	if (dim_idx >= NUM_OF_CHANNELS)
	{
		for (i=1; i<NUM_OF_CHANNELS; i++)
		{
			if (DimmerSetup[i].Curve != DimmerSetup[0].Curve) DispDimmSetup.Curve = -1;
			if (DimmerSetup[i].Minimum != DimmerSetup[0].Minimum) DispDimmSetup.Minimum = -1;
			if (DimmerSetup[i].Maximum != DimmerSetup[0].Maximum) DispDimmSetup.Maximum = -1;
			if (DimmerSetup[i].Response != DimmerSetup[0].Response) DispDimmSetup.Response = -1;


			x = DimmerSetup[i].WorkL_ZiZ ^ DimmerSetup[0].WorkL_ZiZ;
			if (x & DIMSET_WZ__WORKLIGHT_ON)
			{
				DispDimmSetWZIR[0] = 3;
				DispDimmSetupWZIR[0] = '?';
			}
			if (x & DIMSET_WZ__ZERO_IS_ZERO)
			{
				DispDimmSetWZIR[1] = 3;
				DispDimmSetupWZIR[1] = '?';
			}
			x = DimmerSetup[i].Ts_Ru ^ DimmerSetup[0].Ts_Ru;
			if (x & DIMSET_TR__TRIAC_STYLE)
			{
				DispDimmSetWZIR[2] = 3;
				DispDimmSetupWZIR[2] = '?';
			}
			if (x & DIMSET_TR__RELAY_USING)
			{
				DispDimmSetWZIR[3] = 3;
				DispDimmSetupWZIR[3] = '?';
			}
		}
	}

}

/* ***************************************************************************************************
 *
 * FAN
 *
 * *************************************************************************************************** */

#define FAN_TIM_ARR_VALUE			952
#define FAN_MINIMUM_SPEED			FAN_TIM_ARR_VALUE * 0.3
#define FAN_STARTING_SPEED			FAN_TIM_ARR_VALUE * 0.5
#define FAN_REGULATION_RANGE		(FAN_TIM_ARR_VALUE - FAN_MINIMUM_SPEED)
// Fan speed is in range 0..100
#define FAN_SPEED_CTRL_REG			TIM2->CCR1
#define FAN_START_COOLING_TEMP		550
#define FAN_STOP_COOLING_TEMP		500
#define FAN_FULL_SPEED_COOLING_TEMP	750
#define FAN_REGULATION_RANGE_TEMP	(FAN_FULL_SPEED_COOLING_TEMP-FAN_START_COOLING_TEMP)
#define FAN_TACHO_RELOAD_VALUE		200

int FanOutPwmValue;
float floatFanOutPwmValue;

void General_Timed_Function(void)
{
	int temperature = (CpuTemperature > ExtTemperature)? CpuTemperature: ExtTemperature;
	DisplayFanTemperature = temperature;

	// --------------------------------------------- FAN ctrl ------------------------------------------
	if (temperature > FAN_FULL_SPEED_COOLING_TEMP)
	{
		FanOutPwmValue = FAN_TIM_ARR_VALUE+1;
	}
	else
	if (temperature > FAN_START_COOLING_TEMP)
	{
		int diff = temperature - FAN_START_COOLING_TEMP;
		float t = diff * FAN_REGULATION_RANGE / FAN_REGULATION_RANGE_TEMP;
		FanOutPwmValue = t + FAN_MINIMUM_SPEED;
	}
	else
	if (temperature < FAN_STOP_COOLING_TEMP)
	{
		// Turn Off FAN
		FanOutPwmValue = 0;
	}

	if (FanOutPwmValue)
	{
		if (!floatFanOutPwmValue)
		{
			floatFanOutPwmValue = FAN_STARTING_SPEED;
			Display_fan_tacho = FAN_TACHO_RELOAD_VALUE;
		}
		else
		{
			float d = FanOutPwmValue - floatFanOutPwmValue;
			floatFanOutPwmValue += d/32;
		}
	}
	else
	{
		floatFanOutPwmValue = 0;
	}
	FAN_SPEED_CTRL_REG = (int)floatFanOutPwmValue;
	Display_fan_output = (floatFanOutPwmValue*100)/FAN_TIM_ARR_VALUE;
}

void Fan_CheckRotaion(void)
{
	static int fanRotationB = 0;
	if (fanRotationB)
	{
		if (FAN_ROTATION_IN)
		{
			fanRotationB = 0;
			Display_fan_tacho = FAN_TACHO_RELOAD_VALUE;
			return;
		}
	}
	else
	{
		if (!FAN_ROTATION_IN)
		{
			fanRotationB = 1;
			Display_fan_tacho = FAN_TACHO_RELOAD_VALUE;
			return;
		}
	}
	if (Display_fan_tacho) Display_fan_tacho--;
}

/* TIM2 CH1 as PWM generator for FAN
 *
 * - output on PA15 (pin 50)
 * - input frequency for timer is 200 MHz
 * - after prescaler PSC, the frequency is 20 MHz
 * - FAN PWM frequency is 21kHz with ARR = 952
 * - positive pulses as active
 * - full speed = high
 * - stop = low
 */
void Fan_Initialization(void)
{
	RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
	(void)RCC->APB1LENR;
	TIM2->PSC = 9;							// PRESCALER = 2 --> 250MHz/10 = 25MHz
	TIM2->ARR = FAN_TIM_ARR_VALUE;			//
	TIM2->CCMR1 = 0x0060;					// CH1 as PWM output
	TIM2->CCER = TIM_CCER_CC1E;
	FAN_SPEED_CTRL_REG = 0;
	FanOutPwmValue = floatFanOutPwmValue = 0;
	TIM2->CR1 = TIM_CR1_CEN; // + TIM_CR1_ARPE;
}


