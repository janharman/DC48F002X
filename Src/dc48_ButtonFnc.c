/*
 * a2ButtonFunctions.c
 *
 *  Created on: 12. 12. 2014
 *      Author: Jan
 */

#include "stm32h523xx.h"
#include <stdint.h>
#include <string.h>
#include "dc48_Buttons.h"
#include "dc48_Globals.h"
#include "dc48_DisplayConst.h"
#include "dc48_Tools.h"
#include "dc48_Flash.h"

void IncDecEditableVariableByValue(int plus, const TDisplayButtonsFnc * dbf)
{
	EditableItemValue += plus;
	if (plus > 0)
	{
			if (EditableItemValue > dbf->Up)
			{
				EditableItemValue %= plus;
				RestoreAutorepeatDelay();
			}
	}
	else // minus
	{
			if (EditableItemValue < dbf->Down)
			{
				EditableItemValue = EditableItemValue + dbf->Up + dbf->Down;
				RestoreAutorepeatDelay();
			}
	}
	if (dbf->Type & DTP_ALWAYS_STORE_VARIABLE)	// Always store variable -> generaly for menu indexes
			*dbf->EditableVar = EditableItemValue;
}

void IncDecEditableVariable(int plus, const TDisplayButtonsFnc * dbf)
{
	if (dbf->Type & BTN_UP_DN_REVERSE_FUNC) plus = !plus;
	if (plus)
	{
			if (EditableItemValue < dbf->Up)
			{
				EditableItemValue++;
				if (EditableItemValue == dbf->Up) RestoreAutorepeatDelay();
			}
			else
				 EditableItemValue = dbf->Down;
	}
	else // minus
	{
			if (EditableItemValue > dbf->Down)
			{
				EditableItemValue--;
				if (EditableItemValue == dbf->Down) RestoreAutorepeatDelay();
			}
			else
				EditableItemValue = dbf->Up;
	}
	if (dbf->Type & DTP_ALWAYS_STORE_VARIABLE)	// Always store variable -> generaly for menu indexes
	{
			*dbf->EditableVar = EditableItemValue;
	}

	/* ON CHANGE VARIABLE ACTION only on screen:
	 * - DISP_NR_PATCH_INDIVIDUAL_DIMMERS
	 * - DISP_NR_DIMMERS_SETUP
	 * - DISP_NR_MANUAL_CONTROL
	 * - DISP_NR_SET_DIGITAL_INPUTS
	 * */

	int i, ch;
	unsigned int * p;

	if (dbf->Type & DTP_ACTION_ON_CHANGE_VARIABLE)
	{
		switch (Actual_Screen_Number)
		{
		case DISP_NR_DIMMERS_SETUP:
				Fill_DisplayDimmerSettings(DimmerSetupDimmerIndex);
				break;
		case DISP_NR_PATCH_INDIVIDUAL_DIMMERS:
				DispPatch[MenuEnterButton_SelectedChannel] = EditableItemValue;
				if (GroupOf12ChannelsIndex >= 4) break;
				p = &Patch[GroupOf12ChannelsIndex * NUM_OF_DISP_CHANNS];
				for (i=0; i<NUM_OF_DISP_CHANNS; i++)
					*p++ = DispPatch[i];
				SaveSetupParametersToFLASH(WRITE_TO_FLASH__PATCH_DIMMERS, dbf->EditableVar);
				break;
		case DISP_NR_MANUAL_CONTROL_CH:
				MenuUpDown_EditedValue = EditableItemValue;
				ch = GroupOf12ChannelsIndex * 12 + MenuEnterButton_SelectedChannel;
				UnitSettings.ManualChannels[ch] = EditableItemValue;
//				Write_Changed_Variable_To_FLASH(Actual_Screen_Number);
				break;
/*
		case DISP_NR_SET_DIGITAL_INPUTS:
				MenuUpDown_EditedValue = EditableItemValue;
				DimmerSettings[MenuEnterButton_SelectedChannel].SvetloDiaTma = EditableItemValue;
				Write_Changed_Variable_To_FLASH(Actual_Screen_Number);
				break;
*/
		}
	}
}

void Store_Editable_Value_Back_To_Variable(int * var)
{
	*var = EditableItemValue;
}

void PatchAllDimmersFromStartingAddress(int * adr)
{
	int a = *adr;
	for (int i=0; i<NUM_OF_CHANNELS; i++)
		Patch[i] = a++;
	SaveSetupParametersToFLASH(WRITE_TO_FLASH__PATCH_DIMMERS, adr);
}

void SaveSetupToFLASH(const TDisplayButtonsFnc * dbf)
{
	int i;
	int memory = WRITE_TO_FLASH__UNIT_SETTINGS;
	switch (Actual_Screen_Number)
	{
	case DISP_NR_1_DIMMER_SETUP:
	case DISP_NR_POPUP_DIMM_MINIMUM:
	case DISP_NR_POPUP_DIMM_MAXIMUM:
			if (DimmerSetupDimmerIndex == NUM_OF_CHANNELS)
			{
				for (i=0; i<NUM_OF_CHANNELS; i++)
					switch (OneDimmerSetupMenuItemIndex)
					{
					case 0: DimmerSetup[i].Curve = DispDimmSetup.Curve;
							break;
					case 1: DimmerSetup[i].Minimum = DispDimmSetup.Minimum;
							break;
					case 2: DimmerSetup[i].Maximum = DispDimmSetup.Maximum;
							break;
					case 3: DimmerSetup[i].Response = DispDimmSetup.Response;
							break;
					}
			}
			else
			{
				memcpy(&DimmerSetup[DimmerSetupDimmerIndex], &DispDimmSetup, sizeof(DispDimmSetup));
			}
			memory = WRITE_TO_FLASH__DIMMERS_SETTS;
			break;
	case DISP_NR_1_ADVANCED_DIMMER_SETUP:
			if (DimmerSetupDimmerIndex == NUM_OF_CHANNELS) // ALL DIMMERS
			{
				for (i=0; i<NUM_OF_CHANNELS; i++)
					switch (OneAdvDimmerSetupMenuItemIndex)
					{
					case 0: DimmerSetup[i].WorkL_ZiZ &= ~DIMSET_WZ__WORKLIGHT_ON;
							if (DispDimmSetWZIR[0]) DimmerSetup[i].WorkL_ZiZ |= DIMSET_WZ__WORKLIGHT_ON;
							break;
					case 1: DimmerSetup[i].WorkL_ZiZ &= ~DIMSET_WZ__ZERO_IS_ZERO;
							if (DispDimmSetWZIR[1]) DimmerSetup[i].WorkL_ZiZ |= DIMSET_WZ__ZERO_IS_ZERO;
							break;
					case 2: DimmerSetup[i].Ts_Ru &= ~DIMSET_TR__TRIAC_STYLE;
							if (DispDimmSetWZIR[2]) DimmerSetup[i].Ts_Ru |= DIMSET_TR__TRIAC_STYLE;
							break;
					case 3: DimmerSetup[i].Ts_Ru &= ~DIMSET_TR__RELAY_USING;
							if (DispDimmSetWZIR[3]) DimmerSetup[i].Ts_Ru |= DIMSET_TR__RELAY_USING;
							break;
					}
			}
			else
			{
					memcpy(&DimmerSetup[DimmerSetupDimmerIndex], &DispDimmSetup, sizeof(DispDimmSetup));
					DimmerSetup[DimmerSetupDimmerIndex].WorkL_ZiZ = 0;
					DimmerSetup[DimmerSetupDimmerIndex].Ts_Ru = 0;
					if (DispDimmSetWZIR[0]) DimmerSetup[DimmerSetupDimmerIndex].WorkL_ZiZ |= DIMSET_WZ__WORKLIGHT_ON;
					if (DispDimmSetWZIR[1]) DimmerSetup[DimmerSetupDimmerIndex].WorkL_ZiZ |= DIMSET_WZ__ZERO_IS_ZERO;
					if (DispDimmSetWZIR[2]) DimmerSetup[DimmerSetupDimmerIndex].Ts_Ru |= DIMSET_TR__TRIAC_STYLE;
					if (DispDimmSetWZIR[3]) DimmerSetup[DimmerSetupDimmerIndex].Ts_Ru |= DIMSET_TR__RELAY_USING;
			}
			Fill_DisplayDimmerSettings(DimmerSetupDimmerIndex);
//			WriteToFlashPending_DimmSet = 1;
			memory = WRITE_TO_FLASH__DIMMERS_SETTS;
			break;
	}
	SaveSetupParametersToFLASH(memory, dbf->EditableVar);
}
/*
 * return 0: variable wo/ change
 * return 1: changed variable
 * */
int Decode_MenuEnterAction(const TButtonEnterAction * bea)
{
	int msk, var;

	switch ((bea->BtnActionType & 0x0f000000))
	{
	case BEA_ON_OFF_VARIABLE:
			*bea->VarRef = !*bea->VarRef;
			return 1;
	case BEA_DISPLAY_SCREEN:
			Actual_Screen_Number = bea->ScrNr;
			return 0;
	case BEA_ON_OFF_BIT:
			msk = 1 << (bea->BtnActionType & 0x1F);
			*bea->VarRef ^= msk;
			return 1;
	case BEA_SET_VARIABLE_TO:
			*bea->VarRef = bea->BtnActionType & 0xFFFF;
			return 1;
	case BEA_SET_VAR_TO_OR_ZERO:
			var = bea->BtnActionType & 0xFFFF;
			if (*bea->VarRef != var) *bea->VarRef = var; else *bea->VarRef = 0;
			return 1;
	case BEA_ON_SINGLE_BIT_OR_OFF:
			var = bea->BtnActionType & 0xFFFF;
			if (*bea->VarRef & var) *bea->VarRef = 0; else *bea->VarRef = var;
			return 1;
	case BEA_ENTER_DISP_IF_NOZERO:
			var = bea->BtnActionType & 0xFFFF;
			if (*bea->VarRef) Actual_Screen_Number = var;
			return 0;
	case BEA_DISPLAY_SCREEN_INDIRECT:
			Actual_Screen_Number = *bea->VarRef;
			return 0;
	case BEA_INCREMENT_VARIABLE:
			var = bea->BtnActionType & 0xFFFF;
			(*bea->VarRef)++;
			if (*bea->VarRef > var) *bea->VarRef = 0;
			return 1;
	}
	return 0;
}

void Evaluate_Buttons_Actions(void)
{
	const TDisplayButtonsFnc * dbf = &DisplayButtonsFunc[Actual_Screen_Number];
	int i;

	if (BUTTON_ANY_PRESSED)
	{
		if (Actual_Screen_Number == DISP_NR_BLACK_OUT)
		{
			Actual_Screen_Number = DISP_NR_MAIN_SCREEN;
			return;
		}
//		if (Actual_Screen_Number > DISP_NR_LOGO)
//			DispTimeoutCounter = dbf->DispTimeout;
	}

	if (BUTTON_ESCAPE_PRESSED)
	{
		switch (dbf->Type & BTN_ESCAPE_MASK)
		{
		case BTN_ESC_CHANGE_DISPLAY:				// 0 = change display
				Actual_Screen_Number = dbf->Escape;
				break;
		case BTN_ESC_SPECIAL_FUNCTION:
				break;
		}
	}

	if (BUTTON_DOWN_PRESSED)
	{
		// Display Button functions

		switch (dbf->Type & BTN_DOWN_MASK)
		{
		case BTN_DN_CHANGE_DISPLAY:				// 0 = change display
				Actual_Screen_Number = dbf->Down;
				break;
		case BTN_DN_CHANGE_VARIABLE:
				IncDecEditableVariable(0, dbf);
				break;
		case BTN_DN_CHANGE_VARBYVAL:
				IncDecEditableVariableByValue(-dbf->IncVal, dbf);
				break;
		case BTN_DN_SET_BIT_VARIABLE:
				*dbf->EditableVar |= dbf->Down;
				break;
		}
	}

	if (BUTTON_UP_PRESSED)
	{
		switch (dbf->Type & BTN_UP_MASK)
		{
		case BTN_UP_CHANGE_DISPLAY:				// 0 = change display
				Actual_Screen_Number = dbf->Up;
				break;
		case BTN_UP_CHANGE_VARIABLE:
				IncDecEditableVariable(1, dbf);
				break;
		case BTN_UP_CHANGE_VARBYVAL:
				IncDecEditableVariableByValue(dbf->IncVal, dbf);
				break;
		case BTN_UP_SET_BIT_VARIABLE:
				*dbf->EditableVar |= dbf->Up;
				Actual_Screen_Number = dbf->Enter;
				break;
		case BTN_UP_CHG_DISP_CONDITION:
				if (*dbf->EditableVar == (dbf->Up >> 16))
						Actual_Screen_Number = dbf->Up & 0xff;
				break;
		case BTN_UP_WRITE_FACTORY_SETTS:
/*
				Initiate_FlashWritingProcess(WRITE_TO_FLASH_CODE__FACTR);
				Actual_Screen_Number = dbf->Enter;
*/
				break;
		}
	}

	if (BUTTON_ENTER_PRESSED)
	{
		switch (dbf->Type & BTN_ENTER_MASK)
		{
		case BTN_ENT_CHANGE_DISPLAY:				// 0 = change display
				Actual_Screen_Number = dbf->Enter;
				break;
		case BTN_UP_CHANGE_VARIABLE:
				EditableItemValue = !EditableItemValue;
				break;
		case BTN_ENT_SAVE_AND_DISPLAY:
				Store_Editable_Value_Back_To_Variable(dbf->EditableVar);
				SaveSetupToFLASH(dbf);
				Actual_Screen_Number = dbf->Enter;
				break;
		case BTN_ENT_CHANGE_DYNAMIC_DISP:
				Actual_Screen_Number = dbf->Enter + *dbf->EditableVar;
				break;
		case BTN_ENT_SAVE_EDITABLE_VAL:
				Store_Editable_Value_Back_To_Variable(dbf->EditableVar);
				break;
		case BTN_ENT_MENU_REFERENCE_ADR:
				Store_Editable_Value_Back_To_Variable(dbf->EditableVar);
				i = Decode_MenuEnterAction(dbf->EnterAction+EditableItemValue);
				// ON CHANGE VARIABLE ACTION
				if ((dbf->Type & DTP_SAVE_ON_CHANGE_VARIABLE) && i)
					SaveSetupToFLASH(dbf);
				break;
		case BTN_ENT_SET_BIT_VARIABLE:
				*dbf->EditableVar = EditableItemValue;
				*(int*)dbf->Enter ^= 1<<(EditableItemValue-1);
//				SaveEditedParameterToEEPROM_ByVarAddr(&SelMemoriesToPlaySeq);
				break;
		case BTN_ENT_SET_BIT_VAR_ESCAPE:
				*dbf->EditableVar |= dbf->Enter;
				Actual_Screen_Number = dbf->Escape;
				break;
		case BTN_ENT_CHANGE_SELECTED_CHAN:
				MenuEnterButton_SelectedChannel++;
				if (MenuEnterButton_SelectedChannel > 11) MenuEnterButton_SelectedChannel = 0;
				switch (Actual_Screen_Number)
				{
				case DISP_NR_PATCH_INDIVIDUAL_DIMMERS:
						MenuUpDown_EditedValue = DispPatch[MenuEnterButton_SelectedChannel];
						break;
				case DISP_NR_MANUAL_CONTROL_CH:
						MenuUpDown_EditedValue = UnitSettings.ManualChannels[MenuEnterButton_SelectedChannel];
						break;
				case DISP_NR_SET_DIGITAL_INPUTS:
						MenuUpDown_EditedValue = DimmerSetup[MenuEnterButton_SelectedChannel].SvetloDiaTma;
						break;
				}
				EditableItemValue = MenuUpDown_EditedValue;
				break;
		case BTN_ENT_PATCH_FROM_START_ADR:
				Store_Editable_Value_Back_To_Variable(dbf->EditableVar);
				PatchAllDimmersFromStartingAddress(dbf->EditableVar);
				Actual_Screen_Number = dbf->Enter;
				break;
		case BTN_ENT_INCREMENT_VARIABLE:
				IncDecEditableVariable(1, dbf);
				break;
		}
	}

//	if ((Buttons_Down == (BUTTON_ESCAPE | BUTTON_UP)) && (Actual_Screen_Number == DISP_NR_LOGO))
//			ServiceMode = SERVICE_MODE_IS_ACTIVE;
#ifdef DEBUG_MODE
	ServiceMode = SERVICE_MODE_IS_ACTIVE;
#endif
}

