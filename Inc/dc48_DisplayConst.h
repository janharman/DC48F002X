/*
 * dc48_DisplayConst.h
 *
 *  Created on: 23. 10. 2014
 *      Author: Jan
 */

#ifndef A2DISPLAYCONST_H_
#define A2DISPLAYCONST_H_

typedef const unsigned char __cu8;

extern __cu8 Ecogate_Logo[];
extern __cu8 Text_WiFi_Mode[8][4];

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fixed Texts
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

typedef struct
{
	int ItemType;
	union
	{
		void * ItemPointer;
		int * Origin;
		int FixNum;
	};
	union
	{
		void * pData;
		unsigned int Num;
	};
	int x;
	int y;
	union
	{
		unsigned char digits;
		unsigned char w;
	};
	union
	{
		unsigned char dimm;
		unsigned char h;
	};
}
	TDisplayItemStruct;

// Zobrazeni polozky - doplnkove funkce

#define SHI_DISIFZERO					0

// Display & Button types

#define BTN_ESCAPE_MASK					0x0000f000
#define BTN_ESC_CHANGE_DISPLAY			0x00001000
#define BTN_ESC_SPECIAL_FUNCTION		0x0000f000
#define BTN_DOWN_MASK					0x00000f00
#define BTN_DN_CHANGE_DISPLAY			0x00000100
#define BTN_DN_CHANGE_VARIABLE			0x00000200
#define BTN_DN_CHANGE_VARBYVAL			0x00000300		// Nelze soucasne pouzit BTN_UP_DN_REVERSE_FUNC
//#define BTN_DN_CHANGE_PATCH_VAL			0x00000400
#define BTN_DN_SET_BIT_VARIABLE			0x00000700
#define BTN_UP_MASK						0x000000f0
#define BTN_UP_CHANGE_DISPLAY			0x00000010
#define BTN_UP_CHANGE_VARIABLE			0x00000020
#define BTN_UP_CHANGE_VARBYVAL			0x00000030		// Nelze soucasne pouzit BTN_UP_DN_REVERSE_FUNC
#define BTN_UP_CHANGE_PATCH_VAL			0x00000040
#define BTN_UP_SET_BIT_VARIABLE			0x00000070
#define BTN_UP_CHG_DISP_CONDITION		0x00000080		// Zmeni display jen pokud: EditVar==(Down>>16)
#define BTN_UP_WRITE_FACTORY_SETTS		0x00000090		// FACTORY SETTINGS
#define BTN_ENTER_MASK					0x0000000f
#define BTN_ENT_CHANGE_DISPLAY			0x00000001
#define BTN_ENT_CHANGE_VARIABLE			0x00000002
#define BTN_ENT_SAVE_AND_DISPLAY		0x00000003
#define BTN_ENT_CHANGE_DYNAMIC_DISP		0x00000004
#define BTN_ENT_SAVE_EDITABLE_VAL		0x00000005
#define BTN_ENT_MENU_REFERENCE_ADR		0x00000006		// Pointer to TButtonEnterAction ->> action is the TButtonEnterAction+MenuItemIndex
#define BTN_ENT_SET_BIT_VARIABLE		0x00000007		// Pouzito jen pro nastaveni play memories
#define BTN_ENT_SET_BIT_VAR_ESCAPE		0x00000009		// Nastavi bit a zmeni obrazovku jako ESC
#define BTN_ENT_CHANGE_CROMACITY_CAL	0x0000000a		// Zmeni chromacity na kalibracni obrazovce
#define BTN_ENT_CHANGE_SELECTED_CHAN	0x0000000b		// Zmeni vybrany kanal v individual patch
#define BTN_ENT_PATCH_FROM_START_ADR	0x0000000c		// Udela Patch vsech kanalu od startovaci adresy
#define BTN_ENT_INCREMENT_VARIABLE		0x0000000d		// Increment variable with maximum value. At max go to zero
#define BTN_UP_DN_REVERSE_FUNC			0x00010000		// Prohozene tlacitka UP a DN
#define DTP_DISPLAY_TYPE_MASK			0xf0000000
#define DTP_NO_CLEAR_BKGD				0x10000000		// without clearing background
#define DTP_ALWAYS_STORE_VARIABLE		0x20000000		// Editovatelnu promennou vzdy po zmene ulozi zpatky
#define DTP_SAVE_ON_CHANGE_VARIABLE		0x40000000		// Saving variable on change
#define DTP_ACTION_ON_CHANGE_VARIABLE	0x80000000		// Some action on variable change

// spodni byte se nuluje (jsou tam vlozena dalsi data, treba cislo bitu)
#define BEA_ON_OFF_VARIABLE				0x00000000
#define BEA_DISPLAY_SCREEN				0x01000000
#define BEA_ON_OFF_BIT					0x02000000		// Bit(y) pro negovani jsou ve spodnim wordu, to znamena, ze timto nelze menit horni word
#define BEA_SET_VARIABLE_TO				0x03000000		// Nastavi promennou VarRef na hodnotu udanou ve spodnim wordu
#define BEA_SET_VAR_TO_OR_ZERO			0x04000000		// Nastavi promennou VarRef na hodnotu udanou ve spodnim wordu, pokud ale VarRef jiz takova je, tak ji nuluje
#define BEA_ON_SINGLE_BIT_OR_OFF		0x05000000		// Nastavi dany bit, ostatni bity nuluje. Pokud je jiz dany bit nastaveny, tak nuluje cely int.
#define BEA_ENTER_DISP_IF_NOZERO		0x06000000		// Prepne se na obrazovku kalibrace, pokud je zapnuty servisni rezim.
#define BEA_DISPLAY_SCREEN_INDIRECT		0x07000000		// Prepne na cislo obrazovky ulozene v promenne
#define BEA_INCREMENT_VARIABLE			0x08000000		// Incrementuje promennou, maximum je ve spodnim wordu

typedef struct
{
	int BtnActionType;
	union
	{
		unsigned int * VarRef;
		int ScrNr;
	};
}
	TButtonEnterAction;

typedef struct
{
	int ItemIndex;
	int NumOfItems;
}
	TDisplayItemPointers;

typedef struct
{
	int DispTimeout;
	int * EditableVar;
	int Type;
	int Escape;
	int Down;
	int Up;
	union
	{
		int Enter;
		const TButtonEnterAction * EnterAction;
	};
	int IncVal;
}
	TDisplayButtonsFnc;

#define DIT_NORMAL_NUMBER		0
#define DIT_SMALL_NUMBER		1
#define DIT_BIG_NUMBERS			2
#define DIT_BAR_CHART_38		3		// x, y, 0, jas
#define DIT_TEXT_OFFSET			4
#define DIT_NUM_IP_ADDR			5
#define DIT_NORMAL_CHARS		6
#define DIT_NUM_MAC_ADDR		7
#define DIT_NUM_YES_NO			8
#define DIT_XBIG_NUMBERS		9
#define DIT_WORKLIGHT_MANUAL	10
#define DIT_CRMX_SIGNAL			11
#define DIT_OK_CHECK_BOX		12
#define DIT_ETHERNET_ACT		13
#define DIT_INVERSE_MENU		14
#define DIT_RADIO_BUTTON		15
#define DIT_BIG_NUM_BY_TAB		16
#define DIT_NOR_NUM_BY_TAB		17
#define DIT_MENU_ITEM			18		// nekresli se pri fixed, ale az pri refresh
#define DIT_ARTNET_UNIVERSE		19
#define DIT_12_NUMS_IN_GRID		20
#define DIT_SEL_MEM_NRS_WRKL	21
#define DIT_4BYTES_IN_TABLE		22
#define DIT_NORM_NUM_DIVIDE		23
#define DIT_DYNAMIC_DISP		24
#define DIT_SERVICE_ICON		25
#define DIT_SM_NUM_NO_NEGATIVE	26		// Zobrazi male cislo, jen kdyz neni nula
#define DIT_PLAY_MEM_BAR		27		// Progress bar pro play memory
#define DIT_OK_CHK_BOX_PLUS		28
#define DIT_NUM_IN_HEXA			29
#define DIT_P_STRING			30
#define DIT_MENU_NUMBER_ACT		31
#define DIT_MENU_NUMBER			32
#define DIT_MENU_NUMBER_ONE_CHR	33
#define DIT_BIG_DIMMER_NR		34
#define DIT_SELECT_FRAME		35
#define DIT_SMALL_BITS			36
#define DIT_REFERENCED_STRING		37
#define DIT_DIMMER_STATUS		38
#define DIT_DIMMER_RUNTIME		39
#define DIT_BAR_CHART_40		40		// x, y, 0, jas
#define	DIT_SEL_DIM_RANGE		41
#define	DIT_SEL_DIM_RNG_SM		42
#define DIT_12_NUMS_FOR_DMX		43
#define DIT_12_SMALL_TOP_NUMS	44
#define DIT_DIMMER_VERSION		45
#define DIT_CHAR_IF_VAR			46
#define FDIT_PATTERN			100
#define FDIT_SM_CHAR			101
#define FDIT_XBIG_NRS			102
#define FDIT_BIG_NRS			103
#define FDIT_STRING				104
#define FDIT_HOR_LIN			105
#define FDIT_1PX_FRAME			106
#define FDIT_P_SM_CHAR			107
#define FDIT_BUTTON				108
#define FDIT_CLEAR_AREA			109
#define FDIT_FILL_AREA			110
#define FDIT_12_CELL_GRIG		111
#define FDIT_VERTICAL_LINE		112
#define FDIT_LOGO				113

// Prace s cislem jako takovym (deleni, shift, mask, modulo...)
#define DNW_DIV					0x80000000			// DIVISION
#define DNW_MOD					0x40000000			// MODULO
#define DNW_MSK					0x20000000			// MASK BITS
#define DNW_SHR					0x10000000			// SHIFT RIGHT
#define DNW_SR4M				0x08000000			// SHIFT RIGHT 4 BIT + MASK 0x0000000F
#define DNW_ADD					0x04000000			// ADD NUMBER IN MASK 0x00FFFFFF
#define DNW_SRXM				0x02000000			// SHIFT RIGHT + MASK 0x000000FF

//-------------------------------------------------------------------------------------------------
// Fixed screens indexes
//-------------------------------------------------------------------------------------------------

#define DISP_IDX_BKGD_00_LOGO				0
#define DISP_LEN_BKGD_00_LOGO				5
#define DISP_IDX_BKGD_01_MAIN_SCREEN		DISP_IDX_BKGD_00_LOGO+DISP_LEN_BKGD_00_LOGO
#define DISP_LEN_BKGD_01_MAIN_SCREEN		2
#define DISP_IDX_BKGD_02_MAIN_MENU			DISP_IDX_BKGD_01_MAIN_SCREEN+DISP_LEN_BKGD_01_MAIN_SCREEN
#define DISP_LEN_BKGD_02_MAIN_MENU			7
#define DISP_IDX_BKGD_03_PATCH				DISP_IDX_BKGD_02_MAIN_MENU+DISP_LEN_BKGD_02_MAIN_MENU
#define DISP_LEN_BKGD_03_PATCH				6
#define DISP_IDX_BKGD_04_DIMMER_SETUP		DISP_IDX_BKGD_03_PATCH+DISP_LEN_BKGD_03_PATCH
#define DISP_LEN_BKGD_04_DIMMER_SETUP		9
#define DISP_IDX_BKGD_05_UNIT_SETTINGS		DISP_IDX_BKGD_04_DIMMER_SETUP+DISP_LEN_BKGD_04_DIMMER_SETUP
#define DISP_LEN_BKGD_05_UNIT_SETTINGS		10
#define DISP_IDX_BKGD_06_SOURCE_CTRL		DISP_IDX_BKGD_05_UNIT_SETTINGS+DISP_LEN_BKGD_05_UNIT_SETTINGS
#define DISP_LEN_BKGD_06_SOURCE_CTRL		9
#define DISP_IDX_BKGD_07_					DISP_IDX_BKGD_06_SOURCE_CTRL+DISP_LEN_BKGD_06_SOURCE_CTRL
#define DISP_LEN_BKGD_07_					9
#define DISP_IDX_BKGD_08_					DISP_IDX_BKGD_07_+DISP_LEN_BKGD_07_
#define DISP_LEN_BKGD_08_					7
#define DISP_IDX_BKGD_09_SERVICE			DISP_IDX_BKGD_08_+DISP_LEN_BKGD_08_
#define DISP_LEN_BKGD_09_SERVICE			9
#define DISP_IDX_BKGD_10_					DISP_IDX_BKGD_09_SERVICE+DISP_LEN_BKGD_09_SERVICE
#define DISP_LEN_BKGD_10_					2
#define DISP_IDX_BKGD_11_INDIVIDUAL_P		DISP_IDX_BKGD_10_+DISP_LEN_BKGD_10_
#define DISP_LEN_BKGD_11_INDIVIDUAL_P		11
#define DISP_IDX_BKGD_12_ONE_DIMM_SET		DISP_IDX_BKGD_11_INDIVIDUAL_P+DISP_LEN_BKGD_11_INDIVIDUAL_P
#define DISP_LEN_BKGD_12_ONE_DIMM_SET		10
#define DISP_IDX_BKGD_13_DIMM_MINIMUM		DISP_IDX_BKGD_12_ONE_DIMM_SET+DISP_LEN_BKGD_12_ONE_DIMM_SET
#define DISP_LEN_BKGD_13_DIMM_MINIMUM		2
#define DISP_IDX_BKGD_14_DIMM_MAXIMUM		DISP_IDX_BKGD_13_DIMM_MINIMUM+DISP_LEN_BKGD_13_DIMM_MINIMUM
#define DISP_LEN_BKGD_14_DIMM_MAXIMUM		2
#define DISP_IDX_BKGD_15_DISP_OFF_TIME		DISP_IDX_BKGD_14_DIMM_MAXIMUM+DISP_LEN_BKGD_14_DIMM_MAXIMUM
#define DISP_LEN_BKGD_15_DISP_OFF_TIME		2
#define DISP_IDX_BKGD_16_SVC_DIMMERC		DISP_IDX_BKGD_15_DISP_OFF_TIME+DISP_LEN_BKGD_15_DISP_OFF_TIME
#define DISP_LEN_BKGD_16_SVC_DIMMERC		7
#define DISP_IDX_BKGD_17_SVC_DMX_VALS		DISP_IDX_BKGD_16_SVC_DIMMERC+DISP_LEN_BKGD_16_SVC_DIMMERC
#define DISP_LEN_BKGD_17_SVC_DMX_VALS		4
#define DISP_IDX_BKGD_18_SVC_ABOUT			DISP_IDX_BKGD_17_SVC_DMX_VALS+DISP_LEN_BKGD_17_SVC_DMX_VALS
#define DISP_LEN_BKGD_18_SVC_ABOUT			9
#define DISP_IDX_BKGD_19_SVC_PIN_CRMX		DISP_IDX_BKGD_18_SVC_ABOUT+DISP_LEN_BKGD_18_SVC_ABOUT
#define DISP_LEN_BKGD_19_SVC_PIN_CRMX		0
#define DISP_IDX_BKGD_20_				DISP_IDX_BKGD_19_SVC_PIN_CRMX+DISP_LEN_BKGD_19_SVC_PIN_CRMX
#define DISP_LEN_BKGD_20_				0
#define DISP_IDX_BKGD_21_				DISP_IDX_BKGD_20_+DISP_LEN_BKGD_20_
#define DISP_LEN_BKGD_21_				9
#define DISP_IDX_BKGD_22_				DISP_IDX_BKGD_21_+DISP_LEN_BKGD_21_
#define DISP_LEN_BKGD_22_				10
#define DISP_IDX_BKGD_23_				DISP_IDX_BKGD_22_+DISP_LEN_BKGD_22_
#define DISP_LEN_BKGD_23_				2
#define DISP_IDX_BKGD_24_				DISP_IDX_BKGD_23_+DISP_LEN_BKGD_23_
#define DISP_LEN_BKGD_24_				2
#define DISP_IDX_BKGD_25_				DISP_IDX_BKGD_24_+DISP_LEN_BKGD_24_
#define DISP_LEN_BKGD_25_				2
#define DISP_IDX_BKGD_26_				DISP_IDX_BKGD_25_+DISP_LEN_BKGD_25_
#define DISP_LEN_BKGD_26_				10
#define DISP_IDX_BKGD_27_				DISP_IDX_BKGD_26_+DISP_LEN_BKGD_26_
#define DISP_LEN_BKGD_27_				2
#define DISP_IDX_BKGD_28_				DISP_IDX_BKGD_27_+DISP_LEN_BKGD_27_
#define DISP_LEN_BKGD_28_				2
#define DISP_IDX_BKGD_29_				DISP_IDX_BKGD_28_+DISP_LEN_BKGD_28_
#define DISP_LEN_BKGD_29_				2
#define DISP_IDX_BKGD_30_				DISP_IDX_BKGD_29_+DISP_LEN_BKGD_29_
#define DISP_LEN_BKGD_30_				13
#define DISP_IDX_BKGD_31_				DISP_IDX_BKGD_30_+DISP_LEN_BKGD_30_
#define DISP_LEN_BKGD_31_				7
#define DISP_IDX_BKGD_32_				DISP_IDX_BKGD_31_+DISP_LEN_BKGD_31_
#define DISP_LEN_BKGD_32_				2
#define DISP_IDX_BKGD_33_				DISP_IDX_BKGD_32_+DISP_LEN_BKGD_32_
#define DISP_LEN_BKGD_33_				2
#define DISP_IDX_BKGD_34_				DISP_IDX_BKGD_33_+DISP_LEN_BKGD_33_
#define DISP_LEN_BKGD_34_				2
#define DISP_IDX_BKGD_35_				DISP_IDX_BKGD_34_+DISP_LEN_BKGD_34_
#define DISP_LEN_BKGD_35_				2
#define DISP_IDX_BKGD_36_				DISP_IDX_BKGD_35_+DISP_LEN_BKGD_35_
#define DISP_LEN_BKGD_36_				3
#define DISP_IDX_BKGD_37_				DISP_IDX_BKGD_36_+DISP_LEN_BKGD_36_
#define DISP_LEN_BKGD_37_				5
#define DISP_IDX_BKGD_38_				DISP_IDX_BKGD_37_+DISP_LEN_BKGD_37_
#define DISP_LEN_BKGD_38_				0
#define DISP_IDX_BKGD_39_				DISP_IDX_BKGD_38_+DISP_LEN_BKGD_38_
#define DISP_LEN_BKGD_39_				7
#define DISP_IDX_BKGD_40_				DISP_IDX_BKGD_39_+DISP_LEN_BKGD_39_
#define DISP_LEN_BKGD_40_				11
#define DISP_IDX_BKGD_41_				DISP_IDX_BKGD_40_+DISP_LEN_BKGD_40_
#define DISP_LEN_BKGD_41_				7
#define DISP_IDX_BKGD_42_				DISP_IDX_BKGD_41_+DISP_LEN_BKGD_41_
#define DISP_LEN_BKGD_42_				2
#define DISP_IDX_BKGD_43_				DISP_IDX_BKGD_42_+DISP_LEN_BKGD_42_
#define DISP_LEN_BKGD_43_				2
#define DISP_IDX_BKGD_44_				DISP_IDX_BKGD_43_+DISP_LEN_BKGD_43_
#define DISP_LEN_BKGD_44_				2
#define DISP_IDX_BKGD_45_				DISP_IDX_BKGD_44_+DISP_LEN_BKGD_44_
#define DISP_LEN_BKGD_45_				2
#define DISP_IDX_BKGD_46_				DISP_IDX_BKGD_45_+DISP_LEN_BKGD_45_
#define DISP_LEN_BKGD_46_				4
#define DISP_IDX_BKGD_47_				DISP_IDX_BKGD_46_+DISP_LEN_BKGD_46_
#define DISP_LEN_BKGD_47_				8
#define DISP_IDX_BKGD_48_				DISP_IDX_BKGD_47_+DISP_LEN_BKGD_47_
#define DISP_LEN_BKGD_48_				4
#define DISP_IDX_BKGD_49_				DISP_IDX_BKGD_48_+DISP_LEN_BKGD_48_
#define DISP_LEN_BKGD_49_				17
#define DISP_IDX_BKGD_50_				DISP_IDX_BKGD_49_+DISP_LEN_BKGD_49_
#define DISP_LEN_BKGD_50_				5

#define DISP_IDX_BKGD_51_				DISP_IDX_BKGD_50_+DISP_LEN_BKGD_50_
#define DISP_LEN_BKGD_51_				0
#define DISP_IDX_BKGD_52_				DISP_IDX_BKGD_51_+DISP_LEN_BKGD_51_
#define DISP_LEN_BKGD_52_				0
#define DISP_IDX_BKGD_53_				DISP_IDX_BKGD_52_+DISP_LEN_BKGD_52_
#define DISP_LEN_BKGD_53_				0
#define DISP_IDX_BKGD_54_				DISP_IDX_BKGD_53_+DISP_LEN_BKGD_53_
#define DISP_LEN_BKGD_54_				0
#define DISP_IDX_BKGD_55_				DISP_IDX_BKGD_54_+DISP_LEN_BKGD_54_
#define DISP_LEN_BKGD_55_				0
#define DISP_IDX_BKGD_56_				DISP_IDX_BKGD_55_+DISP_LEN_BKGD_55_
#define DISP_LEN_BKGD_56_				0
#define DISP_IDX_BKGD_57_				DISP_IDX_BKGD_56_+DISP_LEN_BKGD_56_
#define DISP_LEN_BKGD_57_				0
#define DISP_IDX_BKGD_58_				DISP_IDX_BKGD_57_+DISP_LEN_BKGD_57_
#define DISP_LEN_BKGD_58_				0
#define DISP_IDX_BKGD_59_				DISP_IDX_BKGD_58_+DISP_LEN_BKGD_58_
#define DISP_LEN_BKGD_59_				0
#define DISP_IDX_BKGD_60_				DISP_IDX_BKGD_59_+DISP_LEN_BKGD_59_
#define DISP_LEN_BKGD_60_				0

#define DISP_IDX_BKGD_LAST_EMPTY_POS		DISP_IDX_BKGD_60_+DISP_LEN_BKGD_60_

//-------------------------------------------------------------------------------------------------
// Refreshable screens indexes
//-------------------------------------------------------------------------------------------------
#define REF_SCR_00_IDX			0
#define REF_SCR_00_LEN			0
#define REF_SCR_01_IDX			REF_SCR_00_IDX + REF_SCR_00_LEN
#define REF_SCR_01_LEN			18
#define REF_SCR_02_IDX			REF_SCR_01_IDX + REF_SCR_01_LEN
#define REF_SCR_02_LEN			1
#define REF_SCR_03_IDX			REF_SCR_02_IDX + REF_SCR_02_LEN
#define REF_SCR_03_LEN			17
#define REF_SCR_04_IDX			REF_SCR_03_IDX + REF_SCR_03_LEN
#define REF_SCR_04_LEN			6
#define REF_SCR_05_IDX			REF_SCR_04_IDX + REF_SCR_04_LEN
#define REF_SCR_05_LEN			6
#define REF_SCR_06_IDX			REF_SCR_05_IDX + REF_SCR_05_LEN
#define REF_SCR_06_LEN			4
#define REF_SCR_07_IDX			REF_SCR_06_IDX + REF_SCR_06_LEN
#define REF_SCR_07_LEN			4
#define REF_SCR_08_IDX			REF_SCR_07_IDX + REF_SCR_07_LEN
#define REF_SCR_08_LEN			2
#define REF_SCR_09_IDX			REF_SCR_08_IDX + REF_SCR_08_LEN
#define REF_SCR_09_LEN			3
#define REF_SCR_10_IDX			REF_SCR_09_IDX + REF_SCR_09_LEN
#define REF_SCR_10_LEN			1
#define REF_SCR_11_IDX			REF_SCR_10_IDX + REF_SCR_10_LEN
#define REF_SCR_11_LEN			14
#define REF_SCR_12_IDX			REF_SCR_11_IDX + REF_SCR_11_LEN
#define REF_SCR_12_LEN			7
#define REF_SCR_13_IDX			REF_SCR_12_IDX + REF_SCR_12_LEN
#define REF_SCR_13_LEN			1
#define REF_SCR_14_IDX			REF_SCR_13_IDX + REF_SCR_13_LEN
#define REF_SCR_14_LEN			1
#define REF_SCR_15_IDX			REF_SCR_14_IDX + REF_SCR_14_LEN
#define REF_SCR_15_LEN			1
#define REF_SCR_16_IDX			REF_SCR_15_IDX + REF_SCR_15_LEN
#define REF_SCR_16_LEN			3
#define REF_SCR_17_IDX			REF_SCR_16_IDX + REF_SCR_16_LEN
#define REF_SCR_17_LEN			26
#define REF_SCR_18_IDX			REF_SCR_17_IDX + REF_SCR_17_LEN
#define REF_SCR_18_LEN			0
#define REF_SCR_19_IDX			REF_SCR_18_IDX + REF_SCR_18_LEN
#define REF_SCR_19_LEN			0
#define REF_SCR_20_IDX			REF_SCR_19_IDX + REF_SCR_19_LEN
#define REF_SCR_20_LEN			0
#define REF_SCR_21_IDX			REF_SCR_20_IDX + REF_SCR_20_LEN
#define REF_SCR_21_LEN			5
#define REF_SCR_22_IDX			REF_SCR_21_IDX + REF_SCR_21_LEN
#define REF_SCR_22_LEN			2
#define REF_SCR_23_IDX			REF_SCR_22_IDX + REF_SCR_22_LEN
#define REF_SCR_23_LEN			1
#define REF_SCR_24_IDX			REF_SCR_23_IDX + REF_SCR_23_LEN
#define REF_SCR_24_LEN			1
#define REF_SCR_25_IDX			REF_SCR_24_IDX + REF_SCR_24_LEN
#define REF_SCR_25_LEN			1
#define REF_SCR_26_IDX			REF_SCR_25_IDX + REF_SCR_25_LEN
#define REF_SCR_26_LEN			2
#define REF_SCR_27_IDX			REF_SCR_26_IDX + REF_SCR_26_LEN
#define REF_SCR_27_LEN			1
#define REF_SCR_28_IDX			REF_SCR_27_IDX + REF_SCR_27_LEN
#define REF_SCR_28_LEN			1
#define REF_SCR_29_IDX			REF_SCR_28_IDX + REF_SCR_28_LEN
#define REF_SCR_29_LEN			1
#define REF_SCR_30_IDX			REF_SCR_29_IDX + REF_SCR_29_LEN
#define REF_SCR_30_LEN			8
#define REF_SCR_31_IDX			REF_SCR_30_IDX + REF_SCR_30_LEN
#define REF_SCR_31_LEN			6
#define REF_SCR_32_IDX			REF_SCR_31_IDX + REF_SCR_31_LEN
#define REF_SCR_32_LEN			1
#define REF_SCR_33_IDX			REF_SCR_32_IDX + REF_SCR_32_LEN
#define REF_SCR_33_LEN			1
#define REF_SCR_34_IDX			REF_SCR_33_IDX + REF_SCR_33_LEN
#define REF_SCR_34_LEN			1
#define REF_SCR_35_IDX			REF_SCR_34_IDX + REF_SCR_34_LEN
#define REF_SCR_35_LEN			1
#define REF_SCR_36_IDX			REF_SCR_35_IDX + REF_SCR_35_LEN
#define REF_SCR_36_LEN			15
#define REF_SCR_37_IDX			REF_SCR_36_IDX + REF_SCR_36_LEN
#define REF_SCR_37_LEN			0
#define REF_SCR_38_IDX			REF_SCR_37_IDX + REF_SCR_37_LEN
#define REF_SCR_38_LEN			0
#define REF_SCR_39_IDX			REF_SCR_38_IDX + REF_SCR_38_LEN
#define REF_SCR_39_LEN			2
#define REF_SCR_40_IDX			REF_SCR_39_IDX + REF_SCR_39_LEN
#define REF_SCR_40_LEN			14
#define REF_SCR_41_IDX			REF_SCR_40_IDX + REF_SCR_40_LEN
#define REF_SCR_41_LEN			6
#define REF_SCR_42_IDX			REF_SCR_41_IDX + REF_SCR_41_LEN
#define REF_SCR_42_LEN			1
#define REF_SCR_43_IDX			REF_SCR_42_IDX + REF_SCR_42_LEN
#define REF_SCR_43_LEN			1
#define REF_SCR_44_IDX			REF_SCR_43_IDX + REF_SCR_43_LEN
#define REF_SCR_44_LEN			1
#define REF_SCR_45_IDX			REF_SCR_44_IDX + REF_SCR_44_LEN
#define REF_SCR_45_LEN			1
#define REF_SCR_46_IDX			REF_SCR_45_IDX + REF_SCR_45_LEN		// ABOUT 2 DIMMERS
#define REF_SCR_46_LEN			14
#define REF_SCR_47_IDX			REF_SCR_46_IDX + REF_SCR_46_LEN
#define REF_SCR_47_LEN			5
#define REF_SCR_48_IDX			REF_SCR_47_IDX + REF_SCR_47_LEN		// INFO 2 DIMMERS
#define REF_SCR_48_LEN			14
#define REF_SCR_49_IDX			REF_SCR_48_IDX + REF_SCR_48_LEN
#define REF_SCR_49_LEN			0
#define REF_SCR_50_IDX			REF_SCR_49_IDX + REF_SCR_49_LEN
#define REF_SCR_50_LEN			16

#define REF_SCR_51_IDX			REF_SCR_50_IDX + REF_SCR_50_LEN
#define REF_SCR_51_LEN			0
#define REF_SCR_52_IDX			REF_SCR_51_IDX + REF_SCR_51_LEN
#define REF_SCR_52_LEN			0
#define REF_SCR_53_IDX			REF_SCR_52_IDX + REF_SCR_52_LEN
#define REF_SCR_53_LEN			0
#define REF_SCR_54_IDX			REF_SCR_53_IDX + REF_SCR_53_LEN
#define REF_SCR_54_LEN			0
#define REF_SCR_55_IDX			REF_SCR_54_IDX + REF_SCR_54_LEN
#define REF_SCR_55_LEN			0
#define REF_SCR_56_IDX			REF_SCR_55_IDX + REF_SCR_55_LEN
#define REF_SCR_56_LEN			0
#define REF_SCR_57_IDX			REF_SCR_56_IDX + REF_SCR_56_LEN
#define REF_SCR_57_LEN			0
#define REF_SCR_58_IDX			REF_SCR_57_IDX + REF_SCR_57_LEN
#define REF_SCR_58_LEN			0
#define REF_SCR_59_IDX			REF_SCR_58_IDX + REF_SCR_58_LEN
#define REF_SCR_59_LEN			0
#define REF_SCR_60_IDX			REF_SCR_59_IDX + REF_SCR_59_LEN
#define REF_SCR_60_LEN			0

#define DISP_MAX_SCREEN_NR					60
#define DISP_REFRESH_ITEM_NR				REF_SCR_60_IDX+REF_SCR_60_LEN

// Display indexes
#define DISP_NR_BLACK_OUT					-1
#define	DISP_NR_LOGO						0
#define DISP_NR_MAIN_SCREEN					1
#define DISP_NR_MAIN_MENU					2
#define DISP_NR_PATCH						3
#define DISP_NR_DIMMERS_SETUP				4
#define DISP_NR_UNIT_SETTINGS				5
#define DISP_NR_INPUT_CONTROL				6
#define DISP_NR_DMX_INP_STATUS_XLR			7
#define DISP_NR_INFO_1						8
#define DISP_NR_SERVICE_MENU				9
#define DISP_NR_POPUP_PATCH_STRT_ADR		10
#define DISP_NR_PATCH_INDIVIDUAL_DIMMERS	11
#define DISP_NR_1_DIMMER_SETUP				12
#define DISP_NR_POPUP_DIMM_MINIMUM			13
#define DISP_NR_POPUP_DIMM_MAXIMUM			14
#define DISP_NR_POPUP_DISPLAY_OFF_TIME		15
#define DISP_NR_SERVICE_DEVICE_DATA			16
#define DISP_NR_SERVICE_DMX_VALUES			17
#define DISP_NR_SERVICE_ABOUT				18
#define DISP_NR_SERVICE_PLUGIN_CRMX			19
#define DISP_NR_DMX_INP_STATUS_CRMX			20
#define DISP_NR_DMX_INP_STATUS_ETHERNET		21
#define DISP_NR_SACN_UNIVERSE				22
#define DISP_NR_POPUP_SACN_UNIV100S			23
#define DISP_NR_POPUP_SACN_UNIV1S			24
#define DISP_NR_POPUP_SACN_UNIV				25
#define DISP_NR_ARTNET_NSNU					26
#define DISP_NR_POPUP_ARTNET_NET			27
#define DISP_NR_POPUP_ARTNET_SUBNET			28
#define DISP_NR_POPUP_ARTNET_UNIVERSE		29
#define DISP_NR_SERVICE_PLUGIN_ETHERNET		30
#define DISP_NR_IP_ADDRESS_CHANGING			31
#define DISP_NR_IP_ADDRESS_A_CHANGING		32
#define DISP_NR_IP_ADDRESS_B_CHANGING		33
#define DISP_NR_IP_ADDRESS_C_CHANGING		34
#define DISP_NR_IP_ADDRESS_D_CHANGING		35
#define DISP_NR_MANUAL_CONTROL_GRP			36	// this only select group
#define DISP_NR_POPUP_FACTORY_SETTINGS		37
#define DISP_NR_POPUP_UNLINK_CRMX_CONFIRM	38
#define DISP_NR_ADVANCED					39
#define DISP_NR_SET_DIGITAL_INPUTS			40
#define DISP_NR_DIA_LEVELS					41
#define DISP_NR_FADE_TIME					42
#define DISP_NR_DIA_LEVEL_1					43
#define DISP_NR_DIA_LEVEL_2					44
#define DISP_NR_DIA_LEVEL_3					45
#define DISP_NR_SERVICE_ABOUT_2_DIMMERS		46
#define DISP_NR_1_ADVANCED_DIMMER_SETUP		47
#define DISP_NR_INFO_2_DIMMERS				48
#define DISP_NR_INFO_3_DIMMERS				49
#define DISP_NR_MANUAL_CONTROL_CH			50	// editing channels

/* DYNAMIC DISPLAY ITEMS INDEXES */

#define DISP_DYN_IDX_00_				0
#define DISP_DYN_LEN_00_				4
#define DISP_DYN_IDX_01_				DISP_DYN_IDX_00_+DISP_DYN_LEN_00_
#define DISP_DYN_LEN_01_				1
#define DISP_DYN_IDX_02_				DISP_DYN_IDX_01_+DISP_DYN_LEN_01_
#define DISP_DYN_LEN_02_				2
#define DISP_DYN_IDX_03_				DISP_DYN_IDX_02_+DISP_DYN_LEN_02_
#define DISP_DYN_LEN_03_				4
#define DISP_DYN_IDX_04_				DISP_DYN_IDX_03_+DISP_DYN_LEN_03_
#define DISP_DYN_LEN_04_				2
#define DISP_DYN_IDX_05_				DISP_DYN_IDX_04_+DISP_DYN_LEN_04_
#define DISP_DYN_LEN_05_				1

#define DYNAMIC_DISPLAY_ITEMS_COUNT		DISP_DYN_IDX_05_+DISP_DYN_LEN_05_
#define DISP_DYNAMIC_SCREEN_COUNT		6

extern const TDisplayItemStruct		DisplayFixedBackgroundItems[DISP_IDX_BKGD_LAST_EMPTY_POS];
extern const TDisplayItemPointers	DisplayFixedItemsPtr[DISP_MAX_SCREEN_NR+1];
extern const TDisplayItemStruct		DisplayRefreshItems[DISP_REFRESH_ITEM_NR];
extern const TDisplayItemPointers	DisplayRefreshItemsPtr[DISP_MAX_SCREEN_NR+1];
extern const TDisplayButtonsFnc		DisplayButtonsFunc[DISP_MAX_SCREEN_NR+1];
extern const TDisplayItemStruct 	DisplayDynamicItems[DYNAMIC_DISPLAY_ITEMS_COUNT];
extern const TDisplayItemPointers	DisplayDynamicItemsPtr[DISP_DYNAMIC_SCREEN_COUNT];

// Textove konstanty
extern const char TextSp_5[];
extern const char TextSm_WiFi[];

#endif /* A2DISPLAYCONST_H_ */
