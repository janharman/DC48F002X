/*
 * a2DisplayFunctions.c
 *
 *  Created on: 15. 10. 2014
 *      Author: Jan
 */

#include <dc48_Buttons.h>
#include <string.h>
//#include <stdint.h>
//#include <stdio.h>

/* Project Includes */
#include "dc48_Globals.h"
#include "OLED_CTRL_2864ASYDT01.h"
#include "dc48_DisplayConst.h"
#include "dc48_Flash.h"
//#include "dc12_Tools.h"
//#include "dc12_CRMX.h"
#include "dc48_Ethernet.h"
#include "dc48_Tools.h"

// Global Variables

int ScreenTimerCounter000;		// Citac

// External variables
//extern const unsigned char ButtonPatterns[4][40];

// Local variables

#define OLED_CHAR__ARROW_RIGHT					1
#define OLED_CHAR__ARROW_DOWN					2
#define OLED_CHAR__ARROW_UP						3
#define OLED_CHAR__DOT_MIDDLE_SMALL				6
#define INACTIVE_DIMMER_CHR						OLED_CHAR__DOT_MIDDLE_SMALL
#define OPTION_NOT_ACTIVE						'-'

int	Displayed_Screen_Number = -1;	// out
int DispRefreshValue[DISP_REFRESH_ITEM_NR];	// hodnoty ktere jsou refreshovany = jiz byly zapsany na display
int DispTimeoutScreen;				// display is changed after Timeout elapses by this screen
int DispTimeoutCounter;
int	ManualModeBlinkCtc;				// For blinking sign "MAN"

void Show_DisplayItem_OnScreen(const TDisplayItemStruct * it);

void TurnDisplayOff(void)
{
	Sleep(1);
	Actual_Screen_Number = DISP_NR_BLACK_OUT;
	Displayed_Screen_Number = DISP_NR_BLACK_OUT;
}

void RefreshMenuItems(const TDisplayItemStruct * it)
{
	int refval = *(int*)(*it).Origin;
	int itemnr = it->x;
	const TDisplayItemStruct * fixItem = &DisplayFixedBackgroundItems[it->y+itemnr];

	while (itemnr--)
	{
			fixItem--;
			if (fixItem->pData)
					if (!(*(int*)fixItem->pData)) FontBrightness = 0x08;
			Show_String(fixItem->ItemPointer, fixItem->x, fixItem->y, refval == itemnr, -1);
			FontBrightness = 0x0f;
	}
}

void ProcessValueChanging(int num, int*refval)
{
	if (num & DNW_DIV) *refval = *refval / (num & 0x00ffffff);
	else if (num & DNW_MOD) *refval = *refval % (num & 0x00ffffff);
	else if (num & DNW_SHR) *refval = *refval >> (num & 0x001f);
	else if (num & DNW_MSK) *refval = *refval & (num & 0x0fffffff);
	else if (num & DNW_SR4M) *refval = (*refval >> 4) & 0x0f;
	else if (num & DNW_ADD)	*refval = *refval + (num & 0x00ffffff);
	else if (num & DNW_SRXM) *refval = (*refval >> (num & 0x001f)) & 0xff;
}

void Show_DynamicDisplay(int dispNr)
{
	const TDisplayItemStruct * it = &DisplayDynamicItems[DisplayDynamicItemsPtr[dispNr].ItemIndex];
	int items_num = DisplayDynamicItemsPtr[dispNr].NumOfItems;

	while (items_num--)
	{
		Show_DisplayItem_OnScreen(it);
		it++;
	}
}

#ifdef DRAW_KNIGHT_RIDER_BAR
void Draw_Knight_Rider_Bar(int x, int y, int refval)
{
	int i;
	int xx = x;
	unsigned char p;

	for (i=0; i<20; i++)
	{
		p = 0x8;
		if (i == refval) p = 0xf;
		Draw_1px_Frame(xx, y, 5, 2, 0, p);
		xx += 6;
	}
}
#endif

void Draw_12_Cells_Grid(int start)
{
	char c[8];
	int y = 37;
	start = start * 12 + 1;
	for (int i=0; i<3; i++)
	{
		if (start <= 9) {
			c[0] = ' ';
			c[1] = '0'+start;
		}
		else {
			c[0] = '0' + (start / 10);
			c[1] = '0' + (start % 10);
		}
		c[2] = '\x60';
		c[3] = '\x61';
		c[4] = 0;
		Show_Small_Characters_String(c, 0, y, 8);
		y += 10;
		start += 4;
	}
	Draw_1px_Frame(14, 33, 29, 31, 0, 0x06);
	Draw_1px_Frame(98, 33, 29, 31, 0, 0x06);
	Show_Fixed_Pattern(35, 34, 1, 29, 0x06);
	Draw_Horizontal_Line(21, 33, 28, 0x66);
	Draw_Horizontal_Line(7, 43, 56, 0x66);
	Draw_Horizontal_Line(7, 53, 56, 0x66);
	Draw_Horizontal_Line(21, 63, 28, 0x66);
}

void Show_12_Numbers_In_Grid(unsigned int *num, int x, int y)
{
/*	unsigned int i, d;
	int xx = x;
	for (i=0; i<12; i++)
	{
		d = 3;
		if (i == MenuEnterButton_SelectedChannel) d = 0x83;
		Write_Number_To_Pos(*num++, xx, y, d);
		xx += 14;
		if (xx > 58)
		{
			xx = x;
			y += 10;
		}
	}
*/
}

void Draw_One_Frame_Around_Bar(void)
{
	int i, x, pattern;

	x = 4;
	for (i=0; i<12; i++)
	{
		if (i == MenuEnterButton_SelectedChannel) pattern = 0x0f; else pattern = 0x00;
		Draw_1px_Frame(x, 18, 10, 40, 0, pattern);
		x += 10;
	}
}

// This is Manual Channels Bar Chart
void Draw_BarChart_38(int refval, const TDisplayItemStruct * it)
{
	int d, dim;

	// Manual channels -> Number is manual channel
	d = refval >> 8;
	// Draw small number below
	if (d < 100)
	{
			if (d) dim = 2; else dim = 8;
			Write_Small_Number_To_Pos(d, (*it).x, (*it).y, (*it).digits, dim);
	}
	else
			Show_Small_Characters_String("FL", (*it).x, (*it).y, 0);
	Draw_Vertical_Bar_Chart_100_38(d, (*it).x*2, (*it).y-40, 1, 0xbb);
	Draw_Vertical_Bar_Chart_100_38(refval & 0xff, (*it).x*2+2, (*it).y-40, 4, 0xbb);
}

// This is Main Screen Bar Chart
void Draw_BarChart_40(int refval, const TDisplayItemStruct * it)
{
	int d, dim;

	d = refval & 0xff;
	// Draw small number below
	if (d < 100)
	{
			if (d) dim = 2; else dim = 8;
			Write_Small_Number_To_Pos(d, (*it).x, (*it).y, (*it).digits, dim);
	}
	else
			Show_Small_Characters_String("FL", (*it).x, (*it).y, 0);
	Draw_Vertical_Bar_Chart_100_40(refval >> 8, (*it).x*2, (*it).y-42, 1, 0xbb);
	Draw_Vertical_Bar_Chart_100_40(d, (*it).x*2+2, (*it).y-42, 4, 0xbb);
}

void Show_WorkLight_Manual_Mode_Icons(int refval, const TDisplayItemStruct * it)
{
	Show_Fixed_Pattern(it->x, it->y, it->digits, 7, 0);
	switch (refval)
	{
	case 1: // Worklight (Shower) icon
		Show_WorkLightIcon(it->x+7, it->y+1, 1);
		break;
	case 2: // MANUAL mode = (MAN) sign
		Show_Small_Characters_String("\x68\x69\x6A\x6b\x6c\x6d", it->x, it->y+2, it->dimm);
		break;
	}
}

void Show_DimmerStatus(unsigned int sts, int x, int y)
{
	if (sts & SDS_STATUS_WORD_OK)
	{
		Show_CheckBox(x+3, y, !!(sts & SDS_LINE_FUSE_OK));
		Show_CheckBox(x+15, y, !!(sts & SDS_BALLAST_PRESENT));
	}
	else
	{
		Show_CheckBox(x+3, y, 4);
		Show_CheckBox(x+15, y, 4);
	}
}

void Show_DimmerRuntimes(int * val, int x, int y)
{
/*
	if (*val++ & SDS_STATUS_WORD_OK)
	{
		val++;
		Write_Small_Number_To_Pos((short)*val++, x-1, y+2, 5, 0);
		Write_Small_Number_To_Pos((short)*val++, x+11, y+2, 5, 0);
	}
	else
		Show_String("\6\6\6 \6\6\6", x, y, 0, 7);
*/
}

void Show_PatchNumbersRange(int val, int x, int y)
{
	int d = val * 12 + 1;
	Write_Number_To_Pos(d, x, y, 2);
	Show_String("-", x+7, y, 0, 1);
	Write_Number_To_Pos(d+11, x+11, y, 2);
	switch (Actual_Screen_Number)
	{
	case DISP_NR_PATCH:
	case DISP_NR_SERVICE_DMX_VALUES:
		memcpy(DispPatch, &Patch[val * 12], 12*4);
		break;
	case DISP_NR_SERVICE_ABOUT_2_DIMMERS:
	case DISP_NR_INFO_2_DIMMERS:
		memcpy(DispSerialDimm, &SerialDimmer[val*NUM_OF_DISP_CHANNS], sizeof(tSerialDimmer)*NUM_OF_DISP_CHANNS);
		break;
	}
	MenuEnterButton_SelectedChannel = 0;
	MenuUpDown_EditedValue = *DispPatch;
}

void Show_PatchNumbersRangeSmall(int val, int x, int y)
{
	int d = val * 12 + 1;
	Write_Small_Number_To_Pos(d, x, y, 2, 2);
	Show_Small_Character('-', x+4, y, 2);
	Write_Small_Number_To_Pos(d+11, x+7, y, 2, 2);
}

void Show_DimmerNumbersGroup(int val, int x, int yy)
{
	int y = yy;
	int ch = val * NUM_OF_DISP_CHANNS + 1;
	for (int i=0; i<NUM_OF_DISP_CHANNS; i++)
	{
		Write_Small_Number_To_Pos(ch++, x, y, 2, 0);
		Show_Small_Characters_String("\x3f", x+4, y, 5);
		Show_Small_Characters_String("\x3E", x+5, y, 5);
		y += 9;
		if (i==5)
		{
			x += 34;
			y = yy;
		}
	}
}

void Show_DimmerNumbersTop(int val, int x, int y)
{
	int ch = val * NUM_OF_DISP_CHANNS + 1;
	for (int i=0; i<NUM_OF_DISP_CHANNS; i++)
	{
		Write_Small_Number_To_Pos(ch++, x, y, 2, 0);
		x += 5;
	}
}

void Show_DimmerVersion(unsigned int ver, int x, int y, int digits)
{
	if (ver > 0)
	{
		char s[8];
		s[7] = 0;
		s[0] = (ver & 0x10000)? 'T': 'I';
		s[1] = ((ver >> 2) & 0x0F) + '0';
		s[2] = (ver & 0x03) + 'A';
		s[3] = ((ver >> 12) & 0x0F) + '0';
		s[4] = ((ver >> 8) & 0x0F) + '0';
		s[5] = (ver & 0x100000)?OLED_CHAR__ARROW_UP:OLED_CHAR__ARROW_DOWN;
		s[6] = (ver & 0x200000)?'R':'-';
		Show_String(s, x, y, 0, digits);
	}
	else
	{
		Show_String("\x06\x06\x06\x06\x06\x06\x06", x, y, 0, digits);
	}
}

void Show_DisplayItem_OnScreen(const TDisplayItemStruct * it)
{
	int * pInt;
	unsigned char c1, c2;
	int refval, min, max, d;
	char * pc;

	if ((*it).ItemType < FDIT_PATTERN) refval = *(int*)(*it).Origin;

	switch ((*it).ItemType)
	{
	case DIT_NORMAL_NUMBER:
		if (it->Num) ProcessValueChanging(it->Num, &refval);
		Write_Number_To_Pos(refval, (*it).x, (*it).y, (*it).digits);
		if (it->dimm) Show_Chars_In_Integer(it->dimm, it->x+it->digits*3+1, it->y, 1);
		break;
	case DIT_SMALL_NUMBER:
		Write_Small_Number_To_Pos(refval, (*it).x, (*it).y, (*it).digits, it->dimm);
		break;
	case DIT_BIG_NUMBERS:
		if (it->Num) ProcessValueChanging(it->Num, &refval);
		Write_Big_Number_To_Pos(refval, (*it).x, (*it).y, (*it).digits);
		break;
	case DIT_XBIG_NUMBERS:
		Write_ExtraBig_Number_To_Pos(refval, (*it).x, (*it).y, (*it).digits);
		break;
	case DIT_BAR_CHART_38:
		Draw_BarChart_38(refval, it);
		break;
	case DIT_TEXT_OFFSET:
		Show_String(((char*)(*it).pData)+(refval*it->digits), (*it).x, (*it).y, 0, -1);
		break;
	case DIT_NUM_IP_ADDR:
		Show_Number_As_IP_Addr(refval, (*it).x, (*it).y);
		break;
	case DIT_NORMAL_CHARS:
		Show_Chars_In_Integer(refval, (*it).x, (*it).y, (*it).digits);
		break;
	case DIT_NUM_MAC_ADDR:
		Show_6Bytes_As_Mac_Address((char *)(*it).Origin, (*it).x, (*it).y);
		break;
	case DIT_NUM_YES_NO:
		Show_String_Yes_No(refval, (*it).x, (*it).y);
		break;
	case DIT_WORKLIGHT_MANUAL:
		Show_WorkLight_Manual_Mode_Icons(refval, it);
		break;
	case DIT_CRMX_SIGNAL:
		Show_CrmxSignalLevel(it->x, it->y, refval);
		break;
	case DIT_OK_CHECK_BOX:
		if (it->Num) refval = refval & it->Num;
		Show_CheckBox(it->x, it->y, refval>0);
		break;
	case DIT_ETHERNET_ACT:
		Show_EthernetStatus(it->x, it->y, refval);
		break;
	case DIT_INVERSE_MENU:
		RefreshMenuItems(it);
		break;
	case DIT_RADIO_BUTTON:
		Show_RadioButton(it->x, it->y, refval == it->Num);
		break;
	case DIT_BIG_NUM_BY_TAB:
		pInt = it->pData;
		Write_Big_Number_To_Pos(*(pInt+refval), (*it).x, (*it).y, (*it).digits);
		break;
	case DIT_NOR_NUM_BY_TAB:
		pInt = it->pData;
		Write_Number_To_Pos(*(pInt+refval), (*it).x, (*it).y, (*it).digits);
		break;
	case DIT_MENU_ITEM:
		// polozka menu se nekresli - zpracovava se to v DIT_INVERSE_MENU
		break;
	case DIT_ARTNET_UNIVERSE:
		Show_ArtNetUniverse(refval, (*it).x, (*it).y);
		break;
	case DIT_12_NUMS_IN_GRID:
		Show_12_Numbers_In_Grid(it->ItemPointer, it->x, it->y);
		break;
	case DIT_SEL_MEM_NRS_WRKL:
//		Show_30_Memory_Numbers(StandAlone_MemoryNum, SelMemoriesToPlaySeq, it->x, it->y);
		break;
	case DIT_4BYTES_IN_TABLE:
//		Show_BytesValueInTable(it);
		break;
	case DIT_NORM_NUM_DIVIDE:
		Write_Divided_Number_To_Pos(refval, (*it).x, (*it).y, (*it).digits, it->Num);
		d = ((*it).digits & 0x0f) + (((*it).digits & 0xf0) >> 4) + 1;
		if (it->dimm) Show_Chars_In_Integer(it->dimm, it->x+d*3+1, it->y, 1);
		break;
	case DIT_DYNAMIC_DISP:
		min = it->Num & 0xffff;
		max = it->Num >> 16;
		if ((refval < min) || (refval > max)) refval = min;
		Show_Fixed_Pattern(it->x, it->y, it->w, it->h, 0); // to clear area
		Show_DynamicDisplay(refval);
		break;
	case DIT_SERVICE_ICON:
		Show_ServiceIcon(it->x, it->y, refval!=0);
		break;
	case DIT_SM_NUM_NO_NEGATIVE:
		if (refval >= 0)
		{
				Write_Small_Number_To_Pos(refval, (*it).x, (*it).y, (*it).digits, 0);
				Show_Small_Character(it->dimm, it->x+(it->digits*2), it->y, 0);
		}
		else	Show_Fixed_Pattern(it->x, it->y, (it->digits+1)*2, 5, 0);
		break;
	case DIT_PLAY_MEM_BAR:
//		Draw_Knight_Rider_Bar(it->x, it->y, refval);
		break;
	case DIT_OK_CHK_BOX_PLUS:
		if (it->Num) {}
		Show_CheckBox(it->x, it->y, refval);
		break;
	case DIT_NUM_IN_HEXA:
		Show_Bytes_In_Hex((char *)&refval, it->x, it->y, it->digits);
		break;
	case DIT_P_STRING:
		pc = *(char **)(*it).ItemPointer;
		if (pc) Show_String(pc, it->x, it->y, it->dimm, it->digits);
		break;
	case DIT_MENU_NUMBER_ACT:
		d = it->y;
		max = it->x;
		while (d--) DispRefreshValue[max++] = 0x7fffffff;
		break;
	case DIT_MENU_NUMBER:
		d = it->digits;
		if (*(int*)it->pData == it->dimm) d |= 0x80; // pData je ukazatel na promennou, pokud se promenna rovna dimm, tak je cislo inverzni
		Write_Number_To_Pos(refval, (*it).x, (*it).y, d);
		break;
	case DIT_MENU_NUMBER_ONE_CHR:
		d = it->digits;
		if (*(int*)it->pData == it->dimm) d |= 0x80; // pData je ukazatel na promennou, pokud se promenna rovna dimm, tak je cislo inverzni
		Write_Central_One_Chr_To_3Pos(refval, (*it).x, (*it).y, d);
		break;
	case DIT_BIG_DIMMER_NR:
		Show_Big_Numbers_48ALL(refval, it->x, it->y);
		break;
	case DIT_SELECT_FRAME:
		Draw_One_Frame_Around_Bar();
		break;
	case DIT_SMALL_BITS:
		Write_Small_Bits_To_Pos(refval, it->x, it->y, it->digits, it->dimm);
		break;
	case DIT_REFERENCED_STRING:
		Show_String(it->pData, it->x, it->y, it->dimm, it->digits);
		break;
	case DIT_DIMMER_STATUS:
		Show_DimmerStatus(refval, it->x, it->y);
		break;
	case DIT_DIMMER_RUNTIME:
		Show_DimmerRuntimes(it->pData, it->x, it->y);
		break;
	case DIT_BAR_CHART_40:
		Draw_BarChart_40(refval, it);
		break;
	case DIT_SEL_DIM_RANGE:
		Show_PatchNumbersRange(refval, it->x, it->y);
		break;
	case DIT_SEL_DIM_RNG_SM:
		Show_PatchNumbersRangeSmall(refval, it->x, it->y);
		break;
	case DIT_12_NUMS_FOR_DMX:
		Show_DimmerNumbersGroup(refval, it->x, it->y);
		break;
	case DIT_12_SMALL_TOP_NUMS:
		Show_DimmerNumbersTop(refval, it->x, it->y);
		break;
	case DIT_DIMMER_VERSION:
		Show_DimmerVersion(refval, it->x, it->y, it->digits);
		break;
	case DIT_CHAR_IF_VAR:
		c1 = 0;
		if (refval) c1 = (*it).h; else c1 = ' ';
		Show_String((char*)&c1, (*it).x, (*it).y, 0, 1);
		break;

	// FIXED DISPLAY ITEMS

	case FDIT_PATTERN:
		Show_Pattern((*it).ItemPointer, (*it).x, (*it).w, (*it).y, (*it).h);
		break;
	case FDIT_SM_CHAR:
		Show_Small_Characters_String((void*)(*it).ItemPointer, (*it).x, (*it).y, (*it).dimm);
		break;
	case FDIT_XBIG_NRS:
		Show_ExtraBig_Numbers_String((void*)(*it).ItemPointer, (*it).x, (*it).y);
		break;
	case FDIT_BIG_NRS:
		Show_Big_Numbers_String((void*)(*it).ItemPointer, (*it).x, (*it).y);
		break;
	case FDIT_STRING:
		Show_String((void*)(*it).ItemPointer, (*it).x, (*it).y, it->h, (it->w)?it->w:-1);
		break;
	case FDIT_HOR_LIN:
		Draw_Horizontal_Line((*it).x, (*it).y, (*it).w, (*it).h);
		break;
	case FDIT_1PX_FRAME:
		c1 = (int)(*it).ItemPointer & 0xf0;
		c2 = (int)(*it).ItemPointer & 0x0f;
		Draw_1px_Frame((*it).x, (*it).y, (*it).w, (*it).h, c1, c2);
		break;
	case FDIT_P_SM_CHAR:
		pc = *(char **)(*it).ItemPointer;
		if (pc)
				Show_Small_Characters_String(pc, (*it).x, (*it).y, (*it).dimm);
		break;
	case FDIT_BUTTON:
		Show_Button_Dimmed(it->x, it->y, it->dimm, it->FixNum);
//		Show_Dimmed_Pattern(ButtonPatterns[it->FixNum], it->x, it->x+4, it->y, it->y+7, it->dimm);
		break;
	case FDIT_CLEAR_AREA:
		Show_Fixed_Pattern(it->x, it->y, it->w, it->h, 0);
		break;
	case FDIT_FILL_AREA:
		Show_Fixed_Pattern(it->x, it->y, it->w, it->h, it->Num);
		break;
	case FDIT_12_CELL_GRIG:
		refval = *(int*)(*it).Origin;
		Draw_12_Cells_Grid(refval);
		break;
	case FDIT_VERTICAL_LINE:
		Draw_Vertical_Line_1px(it->x, it->y, it->w, it->h);
		break;
	case FDIT_LOGO:
		DisplayLogo(refval, (*it).x, (*it).w, (*it).y, (*it).h);
		break;
	}
}

void Count_StartDmxAddress(void)
{
	int i;
	int ok = 1;
	unsigned int a = Patch[0]+1;
	for (i=1; i<NUM_OF_CHANNELS; i++)
		if (a++ != Patch[i])
		{
			ok = 0;
			break;
		}
	if (!ok) strcpy(DispDmxStartAddress, "???");
	else __byteToStr(Patch[0], DispDmxStartAddress);
}

/* Displayed_Screen_Number = old screen
 * Actual_Screen_Number = new screen
 * */
void On_Exit_On_Enter_Display(void)
{
	unsigned int ipa;
	/* On Exit */
	switch (Displayed_Screen_Number)
	{
	case DISP_NR_MANUAL_CONTROL_CH:
			SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, (int *)UnitSettings.ManualChannels);
			break;
	}

	/* On Enter */
	switch (Actual_Screen_Number)
	{
	case DISP_NR_PATCH:
			Count_StartDmxAddress();
			break;
	case DISP_NR_DIMMERS_SETUP:
			Fill_DisplayDimmerSettings(DimmerSetupDimmerIndex);
			break;
	case DISP_NR_INPUT_CONTROL:
			if (Displayed_Screen_Number == DISP_NR_MAIN_MENU)
			{
				Eth_Disp_ETH_Enabled = Eth_ETH_Enabled;
				Eth_Disp_sACN_Universe = Eth_sACN_Universe;
				Eth_Disp_ArtNet_Universe = Eth_ArtNet_Universe;
			}
			else // check changes
			{
				if (Eth_Disp_sACN_Universe != Eth_sACN_Universe) Eth_Universes_ToChange = 1;
				if (Eth_Disp_ArtNet_Universe != Eth_ArtNet_Universe) Eth_Universes_ToChange = 1;
			}
			break;
	case DISP_NR_POPUP_PATCH_STRT_ADR:
			DmxStartAddress = Patch[0];
			if (DmxStartAddress > 465) DmxStartAddress = 465;
			break;
	case DISP_NR_PATCH_INDIVIDUAL_DIMMERS:
			MenuUpDown_EditedValue = DispPatch[MenuEnterButton_SelectedChannel];
			break;
	case DISP_NR_IP_ADDRESS_CHANGING:
			if (Displayed_Screen_Number == DISP_NR_SERVICE_PLUGIN_ETHERNET)
			{
					Eth_DisplayIpAddress[0]	= Eth_IP_Address >> 24;				// A = 192
					Eth_DisplayIpAddress[1]	= (Eth_IP_Address >> 16) & 0xff;	// B = 168
					Eth_DisplayIpAddress[2]	= (Eth_IP_Address >> 8) & 0xff;		// C = 1
					Eth_DisplayIpAddress[3]	= Eth_IP_Address & 0xff;			// D = 223
			}
			else	// from Pop-up changing part of IP
			{
					ipa = (Eth_DisplayIpAddress[0] << 24) +
						  ((Eth_DisplayIpAddress[1] << 16) & 0x00ff0000) +
						  ((Eth_DisplayIpAddress[2] << 8) & 0x0000ff00) +
						  (Eth_DisplayIpAddress[3] & 0xff);
					if (ipa != Eth_IP_Address)
					{
						Eth_IP_Address_ToChange = ipa;
					}
			}

			break;
	case DISP_NR_MANUAL_CONTROL_GRP:
			MenuUpDown_EditedValue = UnitSettings.ManualChannels[MenuEnterButton_SelectedChannel];
			break;
	}
}

//---------------------------------------------------------------------
// Backgroud for the selected screen
//---------------------------------------------------------------------
void Display_Screen_Background(int bkgd)
{
	const TDisplayButtonsFnc * dbf = &DisplayButtonsFunc[bkgd];
	const TDisplayItemStruct *item = &DisplayFixedBackgroundItems[DisplayFixedItemsPtr[bkgd].ItemIndex];
	int items_num = DisplayFixedItemsPtr[bkgd].NumOfItems;
	int disp_type = dbf->Type;

	if (bkgd != DISP_NR_MAIN_SCREEN)
	{
			// Timeout pro dany display. Po vyprseni casu (ulozeno v hornich 3 bytech),
			// se prepne na hodnotu ulozenou ve spodnim byte.
			DispTimeoutCounter = dbf->DispTimeout;
			DispTimeoutScreen = dbf->Escape;
	}
	else
	{
			DispTimeoutCounter = UnitSettings.DisplayOffTime * 60 * 1/0.008;	// cas je udany v minutach. Decrement kazdych 32 ms.
			DispTimeoutScreen = DISP_NR_BLACK_OUT;
	}

	if ((Displayed_Screen_Number >= 0) && (DisplayButtonsFunc[Displayed_Screen_Number].Type & DTP_NO_CLEAR_BKGD))
	{
		// Jen prvni polozku - jde o pop-up screen, takze vymazat jen pozadi pop-up screen.
		Show_DisplayItem_OnScreen(&DisplayFixedBackgroundItems[DisplayFixedItemsPtr[Displayed_Screen_Number].ItemIndex]);
	}
	else
	if (!(disp_type & DTP_NO_CLEAR_BKGD))
			Clear_Screen();
	// vykresleni obrazovky pozadi, dle parametru
	// vykreslovani probiha od zacatku, tak jak je to nadefinovane
	while (items_num--)
	{
		Show_DisplayItem_OnScreen(item);
		item++;
	}

	// Process Editable Variable
	if (dbf->EditableVar)
	{
			EditableItemValue = *dbf->EditableVar;
	};
}

//---------------------------------------------------------------------�
// GLOBAL FUNCTIONS
//---------------------------------------------------------------------
//
// Refreshing all display content according to display content variables
// Called from main each 32 ms
//
void Refresh_Display_Content(void)
{
	int items_num;
	int item_index;
	int i;
	TDisplayItemStruct const * item;
	int * pDrv;

	if (Actual_Screen_Number == DISP_NR_BLACK_OUT) return;

	items_num = DisplayRefreshItemsPtr[Actual_Screen_Number].NumOfItems;
	item_index = DisplayRefreshItemsPtr[Actual_Screen_Number].ItemIndex;

	if (Displayed_Screen_Number != Actual_Screen_Number)
	{
			if (Displayed_Screen_Number == DISP_NR_BLACK_OUT) OLED_Init();
			On_Exit_On_Enter_Display();
			Display_Screen_Background(Actual_Screen_Number);
			Displayed_Screen_Number = Actual_Screen_Number;
			// Je potreba znicit vsechny hodnoty v zrcadle, aby nastalo refreshovani pri zmene displeje
			for (i=item_index;i<item_index+items_num;i++)
					DispRefreshValue[i] = 0x7fffffff;
	}

	// Refreshing dynamic items
	item = &DisplayRefreshItems[item_index];
	while (items_num--)
	{
		if (!item->Origin) break;
		pDrv = &DispRefreshValue[item_index];
		if (*pDrv != *(int*)(*item).Origin) // pokud je hodnota v zrcadle jina nez promenna, tak refreshnout
		{
				*pDrv = *(int*)(*item).Origin;
				Show_DisplayItem_OnScreen(item);
		}
		item_index++;
		item++;
	}

	// Timeout processing
	if (DispTimeoutCounter)
			if (!--DispTimeoutCounter)
			{
					if (DispTimeoutScreen != DISP_NR_BLACK_OUT)
							Actual_Screen_Number = DispTimeoutScreen;
					else
							TurnDisplayOff();
			}

	if (ScreenTimerCounter000)
	{
			ScreenTimerCounter000--;
			return;
	}

}

/* Called from main loop each 40 ms (25 Hz)
 *
 * */

void Display_Other_Time_Actions(void)
{

	if (DMX_Is_Present & DMX_PR_ANY)	// DMX signal is present (XLR or ETH)
	{
			LED_DMX_ON;
	}
	else											// DMX signal is not present
	{
			LED_DMX_OFF;
	}

	switch (Actual_Screen_Number)
	{
	case DISP_NR_INPUT_CONTROL:	// to save data to Ethernet Module (if present)
			if (!(Eth_EthernetStatus & ETHERNET_STS_PLUGIN_MODULE)) break;
			if (Eth_Disp_ETH_Enabled != Eth_ETH_Enabled) Eth_Universes_ToChange = 1;
			break;
	}

	// ERROR LED
	LED_ERR_OFF;

	// Worklight / Manual Mode icon management
	if (UnitSettings.ManualControl)
	{
			ManualModeBlinkCtc--;
			if (ManualModeBlinkCtc > 9)
					Display_Worklight_Manual_Mode = 2;
			else
			{
					Display_Worklight_Manual_Mode = 0;
					if (ManualModeBlinkCtc < 0)
							ManualModeBlinkCtc = 25;
			}
	}
	else
	if (UnitSettings.WorklightEn)
			Display_Worklight_Manual_Mode = 1;
	else
			Display_Worklight_Manual_Mode = 0;
}

void Display_Initialization(void)
{
	OLED_Init();
	Actual_Screen_Number = 0;
	DispTimeoutCounter = 0;			// no timeout is performed
	ManualModeBlinkCtc = 10;
	MenuEnterButton_SelectedChannel = 0;
	GroupOf12ChannelsIndex = 0;
//	EthIpChangingMenuIndex = 3;
}


