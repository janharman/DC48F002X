/*
 * dc48_DimmersCtrl.c
 *
 *  Created on: Feb 1, 2026
 *      Author: harma
 */

#include "string.h"
#include "dc48_Globals.h"
#include "dc48_DisplayConst.h"

#define SVETLO_DIA_TMA_MAXIMUM_VALUE		120000000
#define ALL_SVETLO_DIA_TMA_INPUTS		0x1FF0
#define BTN_STATUS_SAME_INPUTS_VAL		5

#ifdef nicnicnic
float __min_max_curve(float inv, TDimmerSetts * dim_set)
{
	unsigned int range = dim_set->Maximum - dim_set->Minimum;

	// Curve
	switch (dim_set->Curve)
	{
	case DIMM_CURVE_OFF:
			// do not apply minimum/maximum
			return 0;
	case DIMM_CURVE_LINEAR:
			break;
	case DIMM_CURVE_SQUARE:
			inv *= inv;
			break;
	case DIMM_CURVE_SCURVE:
			// Linear Phase --> no changing input value
			break;
	case DIMM_CURVE_NODIMM:
			// switch ON if input value is 50% and more. without hysteresis
			// do not apply minimum/maximum
			return inv>=0.5?1:0;
	}
	// Minimum / Maximum / ZeroIsZero
	if (inv == 0)
			if (dim_set->WorkL_ZiZ & DIMSET_WZ__ZERO_IS_ZERO)	// je-li vstup 0, a zapnuto ZiZ, tak vystup 0, ikdyz je nastavene minimum
					return 0;
	return (inv * range + dim_set->Minimum)  / 1000;
}
#endif
/* *********************************************************************************************
 * After FTM interrupt (reload)
 * Changing output value for FTM
 *
 * */
#ifdef nicnicnic
void Process_Half_Period_PWM_Data(void)
{
	int p, c, d;
	TPhaseMeasData * ld;
	TDmxValues * dmx;
	const unsigned int dead_zone = (Flash_SystemSettings.DeadZoneOff + Flash_SystemSettings.DeadZoneOn) >> 3;
	unsigned int mod_reg, ov;
	register float out_val, dif;
	TDimmerSetts * ds;


	for (p=0; p<3; p++)
		if (LineData[p].PeriodElapsed)
		{
				ld = &LineData[p];
				ld->PeriodElapsed = 0;
				dmx = &DmxValue[p*4];
				ds = &DimmerSettings[p*4];
				mod_reg = *Mod_Reg[p] - dead_zone;
				c = 4;

				// Process 4 channels
				while (c--)
				{
						if (1 || (ld->PhasePeriodLock == PHMEAS_OK_BITS_FOR_OUTPUT))
						{
								if (ds->Curve != DIMM_CURVE_NODIMM)
								{
										// floating output according to response time [30, 100, 300 ms]
										if (!ds->SvetloDiaTma)
										{
												switch (ds->Response)
												{
												case DIMM_RESPONSE_FAST:
														d = 2;
														break;
												case DIMM_RESPONSE_SLOW:
														d = 20;
														break;
												case DIMM_RESPONSE_VERY_SLOW:
														d = 50;
														break;
												default: // DIMM_RESPONSE_NORMAL
														d = 6;
												}
										}
										else	// extra long response time in SvetloDiaTma mode
												d = 100;
										dif = dmx->Output - dmx->OutputFinish;
										out_val = dmx->OutputFinish + dif / d;
										if (dif < 0.0001) out_val = dmx->Output;
								}
								else
										out_val = dmx->Output;
								if (out_val > 1) out_val = 1;
						}
						else
						{
								out_val = 0;
						}
						dmx->OutputFinish = out_val;

						// Store to FTM
						if (!out_val)
							ov = TIMER_VALUE_FOR_ZERO_OUTPUT;
						else
							ov = dead_zone + (mod_reg - (out_val * mod_reg));
						*(dmx->pFTM_CV) = ov;

						dmx++;
						ds++;
				}
		}
}
#endif
#ifdef nicnicnic
void Process_Svetlo_Dia_Tma_OutputLevels(void)
{
	int i, d;
	unsigned int * sdti = SvetloDiaTma_InputLevel;
	unsigned int * sdto = SvetloDiaTma_OutputLevel;

	for (i=0; i<3; i++)
	{
		d = *sdti - *sdto;
		if (d > 0)
		{
				if (d < sdt_fade_add_value) *sdto = *sdti;
				else *sdto += sdt_fade_add_value;
		}
		else
		if (d < 0)
		{
				d = -d;
				if (d < sdt_fade_add_value) *sdto = *sdti;
				else *sdto -= sdt_fade_add_value;
		}
		sdti++;
		sdto++;
	}
}

void Process_Svetlo_Dia_Tma_InputLevels(void)
{
	unsigned int btn_sts;
	static unsigned int btn_sts_same_ctc = 0;
	static unsigned int	old_raw_button_sts;		// svetlo-dia-tma filtering
	int i;

	if (UnitSettings.AuditoriumDisabledIfDmx && GlobalDmxPresent)
	{
		SvetloDiaTma_InputLevel[0] = 0;
		SvetloDiaTma_InputLevel[1] = 0;
		SvetloDiaTma_InputLevel[2] = 0;
		old_raw_button_sts = -1;
		return;
	}

	// Vyhodnoceni tlacitek s filtraci
	btn_sts = RawButtonsStatus & ALL_SVETLO_DIA_TMA_INPUTS;
	if (btn_sts == old_raw_button_sts)
	{
			if (++btn_sts_same_ctc > BTN_STATUS_SAME_INPUTS_VAL)
			{
					SvetloDiaTma[0] = (RawButtonsStatus >> 4) & 7;
					SvetloDiaTma[1] = (RawButtonsStatus >> 7) & 7;
					SvetloDiaTma[2] = (RawButtonsStatus >> 10) & 7;
			}
	}
	else
	{
			old_raw_button_sts = btn_sts;
			btn_sts_same_ctc = 0;
	}

	// Urovne dle stlacenych tlacitek
	for (i=0; i<3; i++)
	{
		if (SvetloDiaTma[i] & 1) SvetloDiaTma_InputLevel[i] = SVETLO_DIA_TMA_MAXIMUM_VALUE;
		else
		if (SvetloDiaTma[i] & 2) SvetloDiaTma_InputLevel[i] = ((float)SVETLO_DIA_TMA_MAXIMUM_VALUE * UnitSettings.DiaLevel[i]) / 100;
		else
		if (SvetloDiaTma[i] & 4) SvetloDiaTma_InputLevel[i] = 0;
	}
	// fade add value counting
	sdt_fade_add_value = (SVETLO_DIA_TMA_MAXIMUM_VALUE/SVETLO_DIA_TMA_FADE_FREQUENCY) / UnitSettings.FadeTime;
}
#endif
/* end of dc12_DataProcess.c */


//void Process_HTP_on_InputDMX_Data(void)
//{
//	int l, c, i;
//	TDmxValues * dmx = DmxValue;
//	unsigned int dv;
//	TDimmerSetts * ds = DimmerSettings;
//	float input_for_bar, sdt;
//
//	i = 0;
//	for (l=0; l<3; l++)
//	{
//		for (c=0; c<4; c++)
//		{
//			if (!UnitSettings.ManualControl)
//			{
//					dv = 0;
//					if (GlobalDmxPresent)
//					{
//							dv = dmx->XLR;
//							if (dv < dmx->Ethernet) dv = dmx->Ethernet;
//							if (dv < dmx->Wifi) dv = dmx->Wifi;
//							dmx->AfterHTP = dv;
//					}
//					else
//					if (UnitSettings.WorklightEnabled && (ds->WorkL_ZiZ & DIMSET_WZ__WORKLIGHT_ON))
//					{
//							dv = 255;
//					}
//					else
//					if (UnitSettings.HoldLast)
//					{
//							dv = dmx->AfterHTP;
//					}
//					dmx->Input = (float)dv/255;
//					input_for_bar = dmx->Input;
//
//					// Svetlo-Dia-Tma -----------------------------
//					if (ds->SvetloDiaTma)
//					{
//							sdt = (float)SvetloDiaTma_InputLevel[ds->SvetloDiaTma-1]  / SVETLO_DIA_TMA_MAXIMUM_VALUE;
//							if (sdt >= dmx->Input) input_for_bar = sdt;
//							sdt = (float)SvetloDiaTma_OutputLevel[ds->SvetloDiaTma-1] / SVETLO_DIA_TMA_MAXIMUM_VALUE;
//							if (sdt > dmx->Input) dmx->Input = sdt;
//					}
//			}
//			else
//			{
//					dmx->Input = (float)UnitSettings.ManualChannels[i]/100;
//					input_for_bar = dmx->Input;
//			}
//			// Minimum, Maximum, Curve
//			dmx->Output = __min_max_curve(dmx->Input, ds);
//
//			// Display
//			dmx->BarChInOut = __display_bar_chart_values(input_for_bar, dmx->Output, i);
//
//			dmx++;
//			ds++;
//			i++;
//		}
//	}
//}


unsigned int __display_bar_chart_values(float inv, float outv, int idx)
{
	unsigned int disp_in, disp_out;

	if ((Actual_Screen_Number == DISP_NR_MANUAL_CONTROL_GRP) || (Actual_Screen_Number == DISP_NR_MANUAL_CONTROL_CH))
			inv = (float)UnitSettings.ManualChannels[idx]/100;
	disp_in = inv * 100;
	disp_out = outv * 100;
	return (disp_in << 8) + disp_out;
}

void _ManualCtrlDimmers(void)
{
	unsigned int * identifiing = RDM_IdentifiingDevice;
	float ov;
	float * dim = DimmerInput;

	for (int i=0; i<NUM_OF_CHANNELS; i++)
	{
		if (*identifiing)
		{
			ov = (*identifiing > 50)? 0.7: 0.3;
			(*identifiing)--;
			if (*identifiing <= 2) *identifiing = 100;
		}
		else
		{
			ov = UnitSettings.ManualChannels[i] / 100.0f;
		}
		identifiing++;
		*dim++ = ov;
	}
}

void _DmxPlusCtrlDimmers(void)
{
	unsigned int * identifiing = RDM_IdentifiingDevice;
	TDimmerSetup * setup = DimmerSetup;
	float * dim = DimmerInput;
	unsigned int * htp = DMX_HTP_Value;
	unsigned int dv;
	unsigned int * xlr = DMX_XLR_Value;
	unsigned int * eth = DMX_ETH_Value;

	for (int i=0; i<NUM_OF_CHANNELS; i++)
	{
		if (*identifiing)
		{
			dv = (*identifiing > 50)? 180: 76;
			(*identifiing)--;
			if (*identifiing <= 2) *identifiing = 100;
		}
		else
		{
			dv = 0;
			if (DMX_Is_Present & DMX_PR_ANY)
			{
				dv = *xlr;
				if (dv < *eth) dv = *eth;
				*htp = dv;
			}
			else	// DMX is NOT present
			if (UnitSettings.WorklightEn && (setup->WorkL_ZiZ & DIMSET_WZ__WORKLIGHT_ON))
			{
					dv = 255;
			}
			else
			if (UnitSettings.HoldLast)
			{
					dv = *htp;
			}
		}
		identifiing++;
		*dim++ = (float)dv / 255.0f;
		xlr++;
		eth++;
	}
}

void _ProcessCurves(float * src, float * dst)
{
	const TDimmerSetup * setup = DimmerSetup;
	float f;
	for (int i=0; i<NUM_OF_CHANNELS; i++)
	{
		f = *src++;
		switch (setup->Curve)
		{
		case DIMM_CURVE_OFF:
			f = 0;
			break;
		case DIMM_CURVE_LINEAR:
			break;
		case DIMM_CURVE_SQUARE:
			f = f * f;
			break;
		case DIMM_CURVE_SCURVE:
			break;
		case DIMM_CURVE_NODIMM:
			f = (f>=0.5)?1:0;
			break;
		case DIMM_CURVE_ALW_ON:
			f = 1;
			break;
		}
		*dst++ = f;
		setup++;
	}
}

void DimmersCtrlThread(void)
{
	if (UnitSettings.ManualControl)
		_ManualCtrlDimmers();
	else
		_DmxPlusCtrlDimmers();

	_ProcessCurves(DimmerInput, DimmerOutput);

	if ((Actual_Screen_Number == DISP_NR_MAIN_SCREEN) ||
		(Actual_Screen_Number == DISP_NR_MANUAL_CONTROL_GRP) ||
		(Actual_Screen_Number == DISP_NR_MANUAL_CONTROL_CH))
	{
		float * src = &DimmerInput[GroupOf12ChannelsIndex*NUM_OF_DISP_CHANNS];
		float * dst = &DimmerOutput[GroupOf12ChannelsIndex*NUM_OF_DISP_CHANNS];
		int * disp = DmxDisplay;
		for (int i=0; i<NUM_OF_DISP_CHANNS; i++)
			*disp++ = ((unsigned int)(*src++ * 100) << 8) + (*dst++ * 100);
	}
	else
	if (Actual_Screen_Number == DISP_NR_SERVICE_DMX_VALUES)
	{
		unsigned int * dmx = &DMX_XLR_Value[GroupOf12ChannelsIndex*NUM_OF_DISP_CHANNS];
		int * disp = DispDmxValue;
		int i = NUM_OF_DISP_CHANNS;
		while (i--)
			*disp++ = *dmx++;
	}
}

void DimmersCtrlInitialization(void)
{
	memset(DimmerInput, 0, sizeof(DimmerInput));
	memset(DMX_XLR_Value, 0, sizeof(DMX_XLR_Value));
	memset(DMX_ETH_Value, 0, sizeof(DMX_ETH_Value));
	memset(DMX_XLR_RecBuffer, 0, sizeof(DMX_XLR_RecBuffer));
	memset(DMX_ETH_RecBuffer, 0, sizeof(DMX_ETH_RecBuffer));
}


