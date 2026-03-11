/*
 * dc48_DimmersCom.c
 *
 *  Created on: 9. 2. 2026
 *      Author: harma
 */

#include <math.h>
#include "dc48_Globals.h"
#include "dc48_DisplayConst.h"
#include "dc48_UART_Dimmers.h"

#define	TICKS_FOR_13_FRAMES	(((1.0/9600.0)*10.0*13.0)*200000000.0) * 1.003

unsigned int 				DimmerIndex;
unsigned int				Frequency[3];

void ProcessDimmerFrequency(unsigned int start, unsigned int end)
{
	if (start < end)
	{
		tSerialDimmer * sd = &SerialDimmer[DimmerIndex];
		unsigned int ticks = end - start;
		sd->FreqMulFactor = ticks / TICKS_FOR_13_FRAMES;
	}
}

int _frame_crc_ok(short * d, int cnt)
{
	int crc = 0;
	while (cnt--)
		crc ^= *d++;
	return crc == *d;
}

void ProcessDimmersRxData(int grp, char * rx)
{
	if (!_frame_crc_ok((short *)rx, 6)) return;
	unsigned int dimidx = *rx & 0x0F;
	if (dimidx >= NUM_OF_DISP_CHANNS) return;
	DimmerIndex = grp*NUM_OF_DISP_CHANNS+dimidx;
	tSerialDimmer * sd = &SerialDimmer[DimmerIndex];

	sd->Live = 25;	// 2.5 seconds to death
	sd->Runtime = rx[2] + (rx[3] << 8) + SDS_STATUS_WORD_OK;
	sd->LineFrequency = rx[4] + (rx[5] << 8);
	sd->LineVoltage = rx[6] + (rx[7]<<8);
	sd->HwFwSt = (rx[1] << 16) +	// Dimmer Type (0=I=IGBT; 1=T=TRIAC; ...)
				 (rx[8]) +			// Hw version
				 (rx[9] << 8) + 	// Fw version
				 (rx[10] << 20) + 	// Triac Style + Relay Using
				 0;

	int fidx = dimidx / 4;
	Frequency[fidx] = (int)round(sd->LineFrequency * sd->FreqMulFactor) >> 1;
	LineFrequency[fidx] = Frequency[fidx];

	if (grp != GroupOf12ChannelsIndex) return;

	switch (Actual_Screen_Number)
	{
	case DISP_NR_SERVICE_ABOUT_2_DIMMERS:
			DispSerialDimm[dimidx].HwFwSt = sd->HwFwSt;
			break;
	case DISP_NR_INFO_2_DIMMERS:
			DispSerialDimm[dimidx].Runtime = sd->Runtime;
			break;
	}
}

void DimmersCommTimed(void)
{
	tSerialDimmer * sd = SerialDimmer;
	for (int i=0; i<NUM_OF_CHANNELS; i++)
	{
		if (sd->Live)
		{
			sd->Live--;
			if (!sd->Live)
			{
				sd->HwFwSt = 0;
				sd->Runtime = 0;
				sd->LineVoltage = 0;
				int grp = i / 12;
				if (grp == GroupOf12ChannelsIndex)
				{
					switch (Actual_Screen_Number)
					{
					case DISP_NR_SERVICE_ABOUT_2_DIMMERS:
							DispSerialDimm[i % 12].HwFwSt = 0;
							break;
					case DISP_NR_INFO_2_DIMMERS:
							DispSerialDimm[i % 12].Runtime = 0;
							break;
					}
				}
			}
		}
		sd++;
	}
}

