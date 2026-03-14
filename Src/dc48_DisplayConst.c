/*
 * gbDisplayConst.c
 *
 *  Created on: 23. 10. 2014
 *      Author: Jan
 */

#include "dc48_DisplayConst.h"
#include "dc48_Globals.h"
#include "dc48_Ethernet.h"
#include "dc48_RdmDef.h"
#include <dc48_Boot_Glob.h>

#define BUTTON_ESCAPE_ICON	.FixNum=0
#define BUTTON_DOWN_ICON	.FixNum=1
#define BUTTON_UP_ICON		.FixNum=2
#define BUTTON_ENTER_ICON	.FixNum=3

const char TextSp_5[] = 		"     ";
const char TextNm_DimmCurve[7][12] = {
		"         ?",
		"       Off",
		"    Linear",
		"    Square",
		"   S-Curve",
		"   NoDim50",
		" Always ON"
};
const char TextNm_DimmResponse[6][12] = {
		"         ?",
		"      Fast",
		"    Normal",
		"      Slow",
		" Very Slow",
		" Undefined"
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  FIXED DISPLAY ITEMS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define SP_L_LINE " \xa3"
#define SP_T_LINE " \xa4"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

const TDisplayItemStruct DisplayFixedBackgroundItems[DISP_IDX_BKGD_LAST_EMPTY_POS] = {
//--------------------------------------------------------------------------------------------
// 00 Logo Screen								pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_LOGO,			0,							0,		19,		0,		45,		53},  // TS_DIMMERS_Logo, 0, 0, 22, 39
	{FDIT_SM_CHAR,		"LOADING\x60\x61",			0,		1,		59, 	0, 		5},
	{FDIT_SM_CHAR,		"VERSION:",					0,		38, 	59, 	0, 		5},
	{FDIT_HOR_LIN,		0,							0,		1, 		57, 	62, 	0x8A},
	{FDIT_SM_CHAR,		FIRMWARE_VERSION,			0,		55, 	59,		4, 		6		},
//--------------------------------------------------------------------------------------------
// 01 Main Screen									pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,							0,		0, 		56,		64, 	0x77},
	{FDIT_SM_CHAR,		"XLR",						0,		0,	 	59, 	0, 		7},
//--------------------------------------------------------------------------------------------
// 02 MAIN MENU										pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{DIT_MENU_ITEM,		"PATCH             ",		0,		1,		1,		0,		0},
	{DIT_MENU_ITEM,		"DIMMER SETUP      ",		0,		1,		10,		0,		0},
	{DIT_MENU_ITEM,		"UNIT SETTINGS     ",		0,		1,		19,		0,		0},
	{DIT_MENU_ITEM,		"INPUT CONTROL     ",		0,		1,		28,		0,		0},
	{DIT_MENU_ITEM,		"DMX STATUS        ",		0,		1,		37,		0,		0},
	{DIT_MENU_ITEM,		"INFO              ",		0,		1,		46,		0,		0},
	{DIT_MENU_ITEM,		"SERVICE MENU      ",		0,		1,		55,		0,		0},
//--------------------------------------------------------------------------------------------
// 03 PATCH											pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,							0,		0, 		0,		64, 	0x77},
	{FDIT_HOR_LIN,		0,							0,		0, 		10,		64, 	0x77},
	{DIT_MENU_ITEM,		"PATCH         ",			0,		1,		2,		0, 		0},
	{DIT_MENU_ITEM,		"Start Address ",			0,		1,		13,		0,		0},
	{DIT_MENU_ITEM,		"Patch Dimmers ",			0,		1,		23,		0,		0},
	{FDIT_BUTTON,		BUTTON_ENTER_ICON,			0,		59,		23,		0, 		7},
//--------------------------------------------------------------------------------------------
// 04 DIMMER SETUP									pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,							0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Dimmer Setup",				0,		3,		6,		12,		1},
	{FDIT_BUTTON,		BUTTON_ENTER_ICON,			0,		59,		9,		0, 		7},
	{FDIT_HOR_LIN,		0,							0,		0, 		18,		64, 	0x77},
	{FDIT_STRING,		"Curve",					0,		2,		20,		0, 		0},
	{FDIT_STRING,		"Minimum",					0,		2,		29,		0, 		0},
	{FDIT_STRING,		"Maximum",					0,		2,		38,		0, 		0},
	{FDIT_STRING,		"Response",					0,		2,		47,		0, 		0},
	{FDIT_STRING,		"Advanced",					0,		2,		56,		0, 		0},
//--------------------------------------------------------------------------------------------
// 05 UNIT SETTINGS									pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,							0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Unit Settings",			0,		13,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,							0,		0, 		10,		64, 	0x77},
	{DIT_MENU_ITEM,		"Worklights Enabled",		0,		1,		14,		0, 		0},
	{DIT_MENU_ITEM,		"Hold Last",				0,		1,		24,		0,		0},
	{DIT_MENU_ITEM,		"Display Off Time  ",		0,		1,		34,		0,		0},
	{DIT_MENU_ITEM,		"Factory Settings  ",		0,		1,		44,		0,		0},
	{DIT_MENU_ITEM,		"Wi-Fi or ETH \x1 XLR",		0,		1,		54,		0,		0},
	{FDIT_SM_CHAR,		"AFTER DMX FAIL",			0,		30,		26, 	0, 		5},
	{FDIT_BUTTON,		BUTTON_ENTER_ICON,			0,		58,		44,		0, 		2},
//--------------------------------------------------------------------------------------------
// 06 DMX INPUT CONTROL
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"DMX Input Control",				0,			6,		2,		17,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		10,		64, 	0x69},
	{DIT_MENU_ITEM,			"Ethernet sACN",					0,			1,		24,		0,		0},
	{DIT_MENU_ITEM,			"sACN Universe",					0,			1,		34,		0,		0},
	{DIT_MENU_ITEM,			"Ethernet ArtNet",					0,			1,		44,		0,		0},
	{DIT_MENU_ITEM,			"Net:SN:Uni=",						0,			1,		54,		0,		0},
	{DIT_NORMAL_NUMBER,		&Eth_Disp_sACN_Universe,			0,			49,		34,		5,		0},
	{DIT_ARTNET_UNIVERSE,	&Eth_Disp_ArtNet_Universe,			0,			37,		54,		2,		0},
//--------------------------------------------------------------------------------------------
// 07 DMX SOURCE STATUS 1/3										pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"DMX Input Status 1/2",				0,			2,		2,		0,		0},
	{FDIT_STRING,			"XLR DMX-512 In/Out",				0,			5,		11,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		19,		64, 	0x69},
	{FDIT_STRING,			"Signal is Present",				0,			0,		23,		0,		0},
	{FDIT_STRING,			"Received Channels",				0,			0,		33,		0,		0},
	{FDIT_STRING,			"RDM is Present",					0,			0,		43,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		52,		64, 	0xaa},
	{FDIT_STRING,			"XLR as DMX Output",				0,			0,		55,		0,		0},
//--------------------------------------------------------------------------------------------
// 08 INFO
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"Info 1/2",							0,			19,		2,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		10,		64, 	0x69},
	{FDIT_STRING,			"MCU Temp.",						0,			1,		44,		9,		0},
	{FDIT_STRING,			"\xb0""C",							0,			58,		44,		0,		0},
	{FDIT_STRING,			"Ext Temp.",						0,			1,		54,		9,		0},
	{FDIT_STRING,			"\xb0""C",							0,			58,		54,		0,		0},
//	{FDIT_STRING,			"L1",								0,			1,		14,		0,		0},
//	{FDIT_STRING,			"L2",								0,			1,		24,		0,		0},
//	{FDIT_STRING,			"L3",								0,			1,		34,		0,		0},
//	{FDIT_STRING,			"Hz",								0,			58,		14,		0,		0},
//	{FDIT_STRING,			"Hz",								0,			58,		24,		0,		0},
//	{FDIT_STRING,			"Hz",								0,			58,		34,		0,		0},
//--------------------------------------------------------------------------------------------
// 09 SERVICE 												pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Service",								0,		21,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{DIT_MENU_ITEM,		"Manual Control   ",					0,		1,		12,		0, 		0},
	{DIT_MENU_ITEM,		SP_L_LINE"Manual Channels",				0,		1,		21,		0,		0},
	{DIT_MENU_ITEM,		"DMX Values       ",					0,		1,		30,		0,		0},
	{DIT_MENU_ITEM,		"Advanced         ",					0,		1,		39,		0,		0},
	{DIT_MENU_ITEM,		"Ethernet Module  ",					0,		1,		48, 	0, 		0},
	{DIT_MENU_ITEM,		"About ...        ",					0,		1,		57, 	0, 		0},
//--------------------------------------------------------------------------------------------
// 10 popup display START ADDRESS
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		41,		6,		23,		22},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		83,		6,		45,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 11 PATCH INDIVIDUAL DIMMERS									pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"INDIVIDUAL PATCH",						0,		8,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{FDIT_BUTTON,		BUTTON_ENTER_ICON,						0,		2,		12,		0, 		7},
	{FDIT_SM_CHAR,		"SELECT DIMMER",						0,		9,		14, 	0, 		7},
	{FDIT_BUTTON,		BUTTON_DOWN_ICON,						0,		2,		22,		0, 		7},
	{FDIT_BUTTON,		BUTTON_UP_ICON,							0,		8,		22,		0, 		7},
	{FDIT_SM_CHAR,		"PATCH DIMMER",							0,		14,		24, 	0, 		7},
	{FDIT_BUTTON,		BUTTON_ESCAPE_ICON,						0,		45,		12,		0, 		7},
	{FDIT_SM_CHAR,		"EXIT",									0,		52,		14, 	0, 		7},
	{FDIT_12_CELL_GRIG,	&GroupOf12ChannelsIndex,				0,		0,		0,		0,		0},
//--------------------------------------------------------------------------------------------
// 12 ONE DIMMER SETUP											pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Dimmer Setup",							0,		3,		6,		0, 		0},
	{DIT_BIG_DIMMER_NR,	&DimmerSetupDimmerIndex,				.Num=DNW_ADD+1,		44,		2,		2,		0		},
	{FDIT_HOR_LIN,		0,										0,		0, 		18,		64, 	0x77},
	{DIT_MENU_ITEM,		"Curve     ",							0,		2,		20,		0, 		0},
	{DIT_MENU_ITEM,		"Minimum   ",							0,		2,		29,		0, 		0},
	{DIT_MENU_ITEM,		"Maximum   ",							0,		2,		38,		0, 		0},
	{DIT_MENU_ITEM,		"Response  ",							0,		2,		47,		0, 		0},
	{DIT_MENU_ITEM,		"Advanced \x01",						0,		2,		56,		0, 		0},
	{FDIT_BUTTON,		BUTTON_ENTER_ICON,						0,		33,		56,		0, 		7},
//--------------------------------------------------------------------------------------------
// 13 popup display DIMMER MINIMUM
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		39,		23,		25,		17},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		78,		24,		50,		15},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 14 popup display DIMMER MAXIMUM
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		39,		33,		25,		17},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		78,		34,		50,		15},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 15 popup display DISPLAY OFF TIME
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		40,		25,		24,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		81,		26,		46,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 16 SERVICE - DIMMERCONTROLLER								pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Dimmercontroller",						0,		8,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{FDIT_STRING,		"Temperature",							0,		0,		13, 	11,		0},
	{FDIT_STRING,		"\xb0""C",								0,		58,		13,		0,		0},
	{FDIT_STRING,		"FAN Speed",							0,		0,		23, 	9, 		0},
	{FDIT_STRING,		"FAN Rotation",							0,		0,		33, 	12,		0},
//--------------------------------------------------------------------------------------------
// 17 SERVICE - DMX VALUES										pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"DMX VALUES",							0,		1,		2,		0, 		1},		// inverted
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{FDIT_VERTICAL_LINE,0,										0,		63,		12,		52,		0x09},
//--------------------------------------------------------------------------------------------
// 18 SERVICE - ABOUT											pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"About DC48 1/2",						0,		12,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{FDIT_STRING,		"Hardware:",							0,		0,		12,		0, 		0},
	{FDIT_STRING,		HARDWARE_VERSION,						0,		52,		12,		0, 		0},
	{FDIT_STRING,		"Firmware:",							0,		0,		22,		0, 		0},
	{FDIT_STRING,		FIRMWARE_VERSION,						0,		52,		22,		0, 		0},
	{FDIT_STRING,		"Fw Date:",								0,		0,		32,		8, 		0},
	{FDIT_STRING,		FIRMWARE_DATE_TXT,						0,		46,		32,		0, 		0},
	{FDIT_STRING,		"Serial Nr:",							0,		0,		42,		0, 		0},
	{DIT_NORMAL_NUMBER,	(int*)&FLASH_RdmSetup.DeviceSerialNr,	0,		52,		42,		4, 		0},
	{FDIT_STRING,		"Bootloader Ver:",						0,		0,		52,		15,		0},
	{DIT_NORMAL_NUMBER,	(int*)&BootLdrVer.Version,	.Num=DNW_MSK+0xFFFF,52,		52,		4, 		0},
//--------------------------------------------------------------------------------------------
// 19 SERVICE - PLUGIN MODULES	(CRMX 1/2) - NEPOUZITO!!!!!!!!!!!!!!!!!						pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
//	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0xac},
//	{FDIT_STRING,			"Plug-in Module CRMX",				0,			2,		2,		0,		0},
//	{FDIT_HOR_LIN,			0,									0,			0, 		10,		64, 	0xac},
//	{FDIT_STRING,			"Driver:",							0,			0,		12,		0,		0},
//	{DIT_NUM_IP_ADDR,		&CRMX_DriverVersion,				0,			35,		12,		3,		0},		// cislo se vydeli 100 a potom se ukaze
//	{FDIT_STRING,			"Hardware:",						0,			0,		21,		0,		0},
//	{DIT_NUM_IP_ADDR,		&CRMX_HardwareVersion,				0,			35,		21,		3,		0},		// cislo se vydeli 100 a potom se ukaze
//	{FDIT_STRING,			"Link Quality",						0,			0,		30,		0,		0},
//	{FDIT_1PX_FRAME,		.FixNum=0x1C,						0,			1,		44,		126,	20},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 20 DMX SOURCE STATUS 2/3 - CRMX - NEPOUZITO!!!!!!!!!!!!!!!!!								pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
//	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0x96},
//	{FDIT_STRING,			"DMX Input Status 2/3",				0,			2,		2,		0,		0},
//	{FDIT_STRING,			"CRMX wireless Input",				0,			4,		11,		0,		0},
//	{FDIT_HOR_LIN,			0,									0,			0, 		19,		64, 	0x69},
//	{FDIT_STRING,			"CRMX is Enabled",					0,			0,		21,		0,		0},
//	{FDIT_STRING,			"Module is Present",				0,			0,		30,		0,		0},
//	{FDIT_STRING,			"Linked to transc.",				0,			0,		39,		0,		0},
//	{FDIT_STRING,			"Signal is OK",						0,			0,		48,		0,		0},
//	{FDIT_STRING,			"Protocol is active",				0,			0,		57,		0,		0},
//--------------------------------------------------------------------------------------------
// 21 DMX SOURCE STATUS 2/2	- ETH								pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"DMX Input Status 2/2",				0,			2,		2,		0,		0},
	{FDIT_STRING,			"Ethernet Input",					0,			12,		11,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		19,		64, 	0x69},
	{FDIT_STRING,			"Ethernet is Enabled",				0,			0,		21,		0,		0},
	{FDIT_STRING,			"Module is Present",				0,			0,		30,		0,		0},
	{FDIT_STRING,			"Connected to Network",				0,			0,		39,		0,		0},
	{FDIT_STRING,			"Signal is Present",				0,			0,		48,		0,		0},
	{FDIT_STRING,			"Universe is Present",				0,			0,		57,		0,		0},
//--------------------------------------------------------------------------------------------
// 22 ETHERNET SACN UNIVERSE									pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"DMX Input Control",				0,			6,		2,		0,		0},
	{FDIT_STRING,			"sACN Universe",					0,			12,		12,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		20,		64, 	0x69},
	{DIT_MENU_ITEM,			"Universe 100s",					0,			1,		23,		0,		0},
	{DIT_MENU_ITEM,			"Universe 1s",						0,			1,		33,		0,		0},
	{DIT_MENU_ITEM,			"Universe",							0,			1,		52,		0,		0},
	{DIT_NORMAL_NUMBER,		&Eth_Disp_sACN_Universe,	.Num=DNW_DIV+100,	55,		23,		3,		0},		// cislo se vydeli 100 a potom se ukaze
	{DIT_NORMAL_NUMBER,		&Eth_Disp_sACN_Universe,	.Num=DNW_MOD+100,	58,		33,		2,		0},		// udela se modulo 100 cisla a potom se ukaze
	{DIT_NORMAL_NUMBER,		&Eth_Disp_sACN_Universe,			0,			49,		52,		5,		0},
//--------------------------------------------------------------------------------------------
// 23 popup display sACN Universe 100s
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,							0,		38,		15,		26,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,				0,		78,		16,		49,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 24 popup display sACN Universe 1s
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,							0,		46,		25,		18,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,				0,		93,		26,		35,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 25 popup display sACN Universe
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,							0,		28,		42,		36,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,				0,		57,		43,		71,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 26 ETHERNET ARTNET NET-SUBNET-UNIVERSE						pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"DMX Input Control",				0,			6,		2,		0,		0},
	{FDIT_STRING,			"ArtNet Universe",					0,			10,		12,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		20,		64, 	0x69},
	{DIT_MENU_ITEM,			"Net",								0,			1,		23,		0,		0},
	{DIT_MENU_ITEM,			"Sub-Net",							0,			1,		33,		0,		0},
	{DIT_MENU_ITEM,			"Universe",							0,			1,		43,		0,		0},
	{DIT_NORMAL_NUMBER,		&Eth_Disp_ArtNet_Universe,	.Num=DNW_SHR+8,		55,		23,		3,		0},		// cislo se vydeli 256 a potom se ukaze
	{DIT_NORMAL_NUMBER,		&Eth_Disp_ArtNet_Universe,	.Num=DNW_SR4M,		58,		33,		2,		0},		// cislo se vydeli 16 a potom se ukaze, ale jen 4 bity
	{DIT_NORMAL_NUMBER,		&Eth_Disp_ArtNet_Universe,	.Num=DNW_MSK+0xF,	49,		43,		5,		0},		// ukazuji se jen dolni 4 bity
//--------------------------------------------------------------------------------------------
// 27 popup display ARTNET NET
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,							0,		38,		15,		26,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,				0,		78,		16,		50,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 28 popup display ARTNET SUB-NET
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,							0,		45,		25,		19,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,				0,		92,		26,		36,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 29 popup display ART NET UNIVERSE
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,							0,		45,		35,		19,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,				0,		92,		36,		36,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 30 SERVICE - PLUG-IN MODULE ETHERNET (2/2)
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0xac},
	{FDIT_STRING,			"Ethernet Module",					0,			10,		2,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		10,		64, 	0xac},
	{FDIT_STRING,			"IP:",								0,			0,		12,		0,		0},
	{FDIT_STRING,			"MAC",								0,			0,		21,		0,		0},
	{FDIT_STRING,			"HW:",								0,			0,		30,		0,		0},
	{FDIT_STRING,			"FW:",								0,			41,		30,		0,		0},
	{FDIT_STRING,			"Rec Frames:",						0,			0,		39,		0,		0},
	{FDIT_STRING,			"Raw Status:",						0,			0,		48,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		57,		64, 	0x46},
	{FDIT_SM_CHAR,			"PRESS",							0,			6,		59,		0, 		8},
	{FDIT_BUTTON,			BUTTON_UP_ICON,						0,			17,		56,		0, 		8},
	{FDIT_SM_CHAR,			"TO SET IP ADDRESS",				0,			23,		59,		0, 		8},
//--------------------------------------------------------------------------------------------
// 31 IP ADDRESS SETTING
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0xac},
	{FDIT_STRING,			"IP Address Setup",					0,			8,		2,		0,		0},
	{FDIT_HOR_LIN,			0,									0,			0, 		10,		64, 	0xac},
	{DIT_MENU_ITEM,			"part A:",							0,			14,		17,		0,		0},
	{DIT_MENU_ITEM,			"part B:",							0,			14,		27,		0,		0},
	{DIT_MENU_ITEM,			"part C:",							0,			14,		37,		0,		0},
	{DIT_MENU_ITEM,			"part D:",							0,			14,		47,		0,		0},
//--------------------------------------------------------------------------------------------
// 32 popup display - IP ADDRESS A SETTING
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,			39,		11,		25,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,			80,		12,		48,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 33 popup display - IP ADDRESS B SETTING
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,			39,		19,		25,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,			80,		20,		48,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 34 popup display - IP ADDRESS C SETTING
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,			39,		29,		25,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,			80,		30,		48,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 35 popup display - IP ADDRESS D SETTING
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,			39,		39,		25,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,			80,		40,		48,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 36 MANUAL CONTROL											pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,			0, 		0, 		64, 	0xac},
	{FDIT_STRING,			"Manual Control",					0,			1,		2,		0,		1},
	{FDIT_HOR_LIN,			0,									0,			0, 		10,		64, 	0xac},
//--------------------------------------------------------------------------------------------
// 37 popup display CONFIRM FACTORY SETTINGS
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,			0,								0,			13,		26,		38,		28		},
	{FDIT_1PX_FRAME,			.FixNum=0x1F,					0,			27,		27,		74,		26		},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
	{FDIT_SM_CHAR,				"CONFIRM FACTORY",				0,			17,		32,		0, 		5		},
	{FDIT_SM_CHAR,				"SETTINGS BY",					0,			18,		42,		0, 		5		},
	{FDIT_BUTTON,				BUTTON_UP_ICON,					0,			41,		40,		0, 		5		},
//--------------------------------------------------------------------------------------------
// 38 pop-up display CONFIRM CRMX UNLINK - NOT USED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!						pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
//	{FDIT_CLEAR_AREA,		0,									0,			9,		19,		46,		33},
//	{FDIT_1PX_FRAME,		.FixNum=0x1F,						0,			19,		20,		90,		31},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//	{FDIT_SM_CHAR,			"PRESS",							0,			13,		25,		0, 		0},
//	{FDIT_BUTTON,			BUTTON_UP_ICON,						0,			24,		23,		0, 		0},
//	{FDIT_SM_CHAR,			"TO CONFIRM",						0,			31,		25,		0, 		0},
//	{FDIT_SM_CHAR,			"UNLINK OR ANY KEY",				0,			14,		33,		0, 		0},
//	{FDIT_SM_CHAR,			"FOR DISCART",						0,			22,		41,		0, 		0},
//--------------------------------------------------------------------------------------------
// 39 ADVANCED	 												pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Advanced",								0,		20,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{DIT_MENU_ITEM,		"Auditorium         ",					0,		1,		12,		0, 		0},
	{DIT_MENU_ITEM,		SP_T_LINE"Dia Levels & Time",			0,		1,		21,		0,		0},
	{DIT_MENU_ITEM,		SP_L_LINE"Disable if DMX   ",			0,		1,		30,		0,		0},
	{DIT_MENU_ITEM,		"Service Data       ",					0,		1,		39,		0,		0},
//--------------------------------------------------------------------------------------------
// 40 SET DIGITAL INPUTS									pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Auditorium",							0,		17,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{FDIT_BUTTON,		BUTTON_ENTER_ICON,						0,		2,		12,		0, 		7},
	{FDIT_SM_CHAR,		"SELECT CHANNEL",						0,		9,		14, 	0, 		7},
	{FDIT_BUTTON,		BUTTON_DOWN_ICON,						0,		2,		22,		0, 		7},
	{FDIT_BUTTON,		BUTTON_UP_ICON,							0,		8,		22,		0, 		7},
	{FDIT_SM_CHAR,		"SET INPUT",							0,		14,		24, 	0, 		7},
	{FDIT_BUTTON,		BUTTON_ESCAPE_ICON,						0,		45,		12,		0, 		7},
	{FDIT_SM_CHAR,		"EXIT",									0,		52,		14, 	0, 		7},
	{FDIT_12_CELL_GRIG,	&GroupOf12ChannelsIndex,				0,		0,		0,		0,		0},
//--------------------------------------------------------------------------------------------
// 41 DIA LEVELS											pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,		0,										0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,		"Dia Lvls & Fade Time",					0,		2,		2,		0, 		0},
	{FDIT_HOR_LIN,		0,										0,		0, 		10,		64, 	0x77},
	{DIT_MENU_ITEM,		"Fade Time   ",							0,		1,		15,		0, 		0},
	{DIT_MENU_ITEM,		"Dia 1 Level ",							0,		1,		24,		0, 		0},
	{DIT_MENU_ITEM,		"Dia 2 Level ",							0,		1,		33,		0, 		0},
	{DIT_MENU_ITEM,		"Dia 3 Level ",							0,		1,		42,		0, 		0},
//--------------------------------------------------------------------------------------------
// 42 popup display FADE TIME
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		40,		4,		24,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		81,		5,		47,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 43 popup display DIA LEVEL 1
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		40,		13,		24,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		81,		14,		47,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 44 popup display DIA LEVEL 2
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		40,		22,		24,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		81,		23,		47,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 45 popup display DIA LEVEL 3
//--------------------------------------------------------------------------------------------
	{FDIT_CLEAR_AREA,	0,										0,		40,		31,		24,		23},
	{FDIT_1PX_FRAME,	.FixNum=0x1F,							0,		81,		32,		47,		21},	// Ptr: Hi Nibble = rounded; Lo Nibble = brightness
//--------------------------------------------------------------------------------------------
// 46 SERVICE ABOUT 2 DIMMERS
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,		0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"About Dimmers",					0,		1,		2,		0,		1},
	{FDIT_HOR_LIN,			0,									0,		0, 		10,		64, 	0x69},
	{FDIT_VERTICAL_LINE,	0,									0,		64,		11,		53,		0x08},
//--------------------------------------------------------------------------------------------
// 47 ONE ADVANCED DIMMER SETUP									pData	x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,		0, 		0,		64, 	0x77},
	{FDIT_STRING,			"Dimmer Setup",						0,		3,		6,		0, 		0},
	{DIT_BIG_DIMMER_NR,		&DimmerSetupDimmerIndex,			.Num=DNW_ADD+1,		44,		2,		2,		0		},
	{FDIT_HOR_LIN,			0,									0,		0, 		18,		64, 	0x77},
	{DIT_MENU_ITEM,			"Worklight   ",						0,		2,		20,		0, 		0},
	{DIT_MENU_ITEM,			"Zero is Zero",						0,		2,		29,		0, 		0},
	{DIT_MENU_ITEM,			"Triac Style ",						0,		2,		38,		0, 		0},
	{DIT_MENU_ITEM,			"Relay Using ",						0,		2,		47,		0, 		0},
//--------------------------------------------------------------------------------------------
// 48 INFO 2 DIMMERS
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,		0, 		0, 		64, 	0x96},
	{FDIT_STRING,			"Dimmers LiF+Bal",					0,		1,		2,		15,		1},
	{FDIT_HOR_LIN,			0,									0,		0, 		10,		64, 	0x69},
	{FDIT_VERTICAL_LINE,	0,									0,		64,		11,		53,		0x08},
//--------------------------------------------------------------------------------------------
// 49 INFO 3 DIMMERS
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,			0,									0,		0, 		0, 		64, 	0x96	},
	{FDIT_STRING,			"D# [V] [A]",						0,		0,		2,		0,		0		},
	{FDIT_HOR_LIN,			0,									0,		0, 		10,		64, 	0x69	},
	{FDIT_STRING,			"D1",								0,		0,		12,		0,		0		},
	{FDIT_STRING,			"D2",								0,		0,		21,		0,		0		},
	{FDIT_STRING,			"D3",								0,		0,		30,		0,		0		},
	{FDIT_STRING,			"D4",								0,		0,		39,		0,		0		},
	{FDIT_STRING,			"D5",								0,		0,		48,		0,		0		},
	{FDIT_STRING,			"D6",								0,		0,		57,		0,		0		},
	{FDIT_STRING,			"D# [V] [A]",						0,		34,		2,		0,		0		},
	{FDIT_STRING,			"D7",								0,		34,		12,		0,		0		},
	{FDIT_STRING,			"D8",								0,		34,		21,		0,		0		},
	{FDIT_STRING,			"D9",								0,		34,		30,		0,		0		},
	{FDIT_STRING,			"10",								0,		34,		39,		0,		0		},
	{FDIT_STRING,			"11",								0,		34,		48,		0,		0		},
	{FDIT_STRING,			"12",								0,		34,		57,		0,		0		},
	{FDIT_VERTICAL_LINE,	0,									0,		64,		1,		63,		0x08	},
//--------------------------------------------------------------------------------------------
// 50 MANUAL CONTROL											pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_HOR_LIN,				0,								0,		0, 		0, 		64, 	0xac	},
	{FDIT_STRING,				"Manual Control",				0,		1,		2,		0,		0		},
	{FDIT_HOR_LIN,				0,								0,		0, 		10,		64, 	0xac	},
	{DIT_SEL_DIM_RNG_SM,		&GroupOf12ChannelsIndex,		0,		45,		3,		0,		0		},
	{DIT_12_SMALL_TOP_NUMS,		&GroupOf12ChannelsIndex,		0,		3,		12,		2,		3		},
};

const TDisplayItemPointers DisplayFixedItemsPtr[DISP_MAX_SCREEN_NR+1] = {
		{DISP_IDX_BKGD_00_LOGO,				DISP_LEN_BKGD_00_LOGO},
		{DISP_IDX_BKGD_01_MAIN_SCREEN,		DISP_LEN_BKGD_01_MAIN_SCREEN},
		{DISP_IDX_BKGD_02_MAIN_MENU,		DISP_LEN_BKGD_02_MAIN_MENU},
		{DISP_IDX_BKGD_03_PATCH,			DISP_LEN_BKGD_03_PATCH},
		{DISP_IDX_BKGD_04_DIMMER_SETUP,		DISP_LEN_BKGD_04_DIMMER_SETUP},
		{DISP_IDX_BKGD_05_UNIT_SETTINGS,	DISP_LEN_BKGD_05_UNIT_SETTINGS},
		{DISP_IDX_BKGD_06_SOURCE_CTRL,		DISP_LEN_BKGD_06_SOURCE_CTRL},
		{DISP_IDX_BKGD_07_,					DISP_LEN_BKGD_07_},
		{DISP_IDX_BKGD_08_,					DISP_LEN_BKGD_08_},
		{DISP_IDX_BKGD_09_SERVICE,			DISP_LEN_BKGD_09_SERVICE},
		{DISP_IDX_BKGD_10_,					DISP_LEN_BKGD_10_},
		{DISP_IDX_BKGD_11_INDIVIDUAL_P,		DISP_LEN_BKGD_11_INDIVIDUAL_P},
		{DISP_IDX_BKGD_12_ONE_DIMM_SET,		DISP_LEN_BKGD_12_ONE_DIMM_SET},
		{DISP_IDX_BKGD_13_DIMM_MINIMUM,		DISP_LEN_BKGD_13_DIMM_MINIMUM},
		{DISP_IDX_BKGD_14_DIMM_MAXIMUM,		DISP_LEN_BKGD_14_DIMM_MAXIMUM},
		{DISP_IDX_BKGD_15_DISP_OFF_TIME,	DISP_LEN_BKGD_15_DISP_OFF_TIME},
		{DISP_IDX_BKGD_16_SVC_DIMMERC,		DISP_LEN_BKGD_16_SVC_DIMMERC},
		{DISP_IDX_BKGD_17_SVC_DMX_VALS,		DISP_LEN_BKGD_17_SVC_DMX_VALS},
		{DISP_IDX_BKGD_18_SVC_ABOUT,		DISP_LEN_BKGD_18_SVC_ABOUT},
		{DISP_IDX_BKGD_19_SVC_PIN_CRMX,		DISP_LEN_BKGD_19_SVC_PIN_CRMX},
		{DISP_IDX_BKGD_20_,					DISP_LEN_BKGD_20_},
		{DISP_IDX_BKGD_21_,					DISP_LEN_BKGD_21_},
		{DISP_IDX_BKGD_22_,					DISP_LEN_BKGD_22_},
		{DISP_IDX_BKGD_23_,					DISP_LEN_BKGD_23_},
		{DISP_IDX_BKGD_24_,					DISP_LEN_BKGD_24_},
		{DISP_IDX_BKGD_25_,					DISP_LEN_BKGD_25_},
		{DISP_IDX_BKGD_26_,					DISP_LEN_BKGD_26_},
		{DISP_IDX_BKGD_27_,					DISP_LEN_BKGD_27_},
		{DISP_IDX_BKGD_28_,					DISP_LEN_BKGD_28_},
		{DISP_IDX_BKGD_29_,					DISP_LEN_BKGD_29_},
		{DISP_IDX_BKGD_30_,					DISP_LEN_BKGD_30_},
		{DISP_IDX_BKGD_31_,					DISP_LEN_BKGD_31_},
		{DISP_IDX_BKGD_32_,					DISP_LEN_BKGD_32_},
		{DISP_IDX_BKGD_33_,					DISP_LEN_BKGD_33_},
		{DISP_IDX_BKGD_34_,					DISP_LEN_BKGD_34_},
		{DISP_IDX_BKGD_35_,					DISP_LEN_BKGD_35_},
		{DISP_IDX_BKGD_36_,					DISP_LEN_BKGD_36_},
		{DISP_IDX_BKGD_37_,					DISP_LEN_BKGD_37_},
		{DISP_IDX_BKGD_38_,					DISP_LEN_BKGD_38_},
		{DISP_IDX_BKGD_39_,					DISP_LEN_BKGD_39_},
		{DISP_IDX_BKGD_40_,					DISP_LEN_BKGD_40_},
		{DISP_IDX_BKGD_41_,					DISP_LEN_BKGD_41_},
		{DISP_IDX_BKGD_42_,					DISP_LEN_BKGD_42_},
		{DISP_IDX_BKGD_43_,					DISP_LEN_BKGD_43_},
		{DISP_IDX_BKGD_44_,					DISP_LEN_BKGD_44_},
		{DISP_IDX_BKGD_45_,					DISP_LEN_BKGD_45_},
		{DISP_IDX_BKGD_46_,					DISP_LEN_BKGD_46_},
		{DISP_IDX_BKGD_47_,					DISP_LEN_BKGD_47_},
		{DISP_IDX_BKGD_48_,					DISP_LEN_BKGD_48_},
		{DISP_IDX_BKGD_49_,					DISP_LEN_BKGD_49_},
		{DISP_IDX_BKGD_50_,					DISP_LEN_BKGD_50_},
		{DISP_IDX_BKGD_51_,					DISP_LEN_BKGD_51_},
		{DISP_IDX_BKGD_52_,					DISP_LEN_BKGD_52_},
		{DISP_IDX_BKGD_53_,					DISP_LEN_BKGD_53_},
		{DISP_IDX_BKGD_54_,					DISP_LEN_BKGD_54_},
		{DISP_IDX_BKGD_55_,					DISP_LEN_BKGD_55_},
		{DISP_IDX_BKGD_56_,					DISP_LEN_BKGD_56_},
		{DISP_IDX_BKGD_57_,					DISP_LEN_BKGD_57_},
		{DISP_IDX_BKGD_58_,					DISP_LEN_BKGD_58_},
		{DISP_IDX_BKGD_59_,					DISP_LEN_BKGD_59_},
		{DISP_IDX_BKGD_60_,					DISP_LEN_BKGD_60_},
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  REFRESHED DISPLAY ITEMS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

const TDisplayItemStruct DisplayRefreshItems[DISP_REFRESH_ITEM_NR] =
{
//-----------------------------------------------------------------------------------------------------------
//	00	LOGO SCREEN				* Origin						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//	01	MAIN SCREEN				* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_OK_CHK_BOX_PLUS,	&XLR_Display_Status,			0,			7,	 	59,		0,		0		},
	{DIT_WORKLIGHT_MANUAL,	&Display_Worklight_Manual_Mode,	0, 			52,	 	57,		12,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[0],					0,			3,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[1],					0,			8,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[2],					0,			13,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[3],					0,			18,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[4],					0,			23,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[5],					0,			28,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[6],					0,			33,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[7],					0,			38,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[8],					0,			43,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[9],					0,			48,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[10],				0,			53,		49,		2,		0		},
	{DIT_BAR_CHART_40,		&DmxDisplay[11],				0,			58,		49,		2,		0		},
	{DIT_SEL_DIM_RNG_SM,	&GroupOf12ChannelsIndex,		0,			40,		59,		0,		0		},
	{DIT_12_SMALL_TOP_NUMS,	&GroupOf12ChannelsIndex,		0,			3,		0,		2,		3		},
	{DIT_ETHERNET_ACT,		&Eth_EthernetStatus,			0, 			27,	 	59,		0, 		0		},
	{FDIT_P_SM_CHAR,		&DispEthernetActiveTxt,			0,			18,		59,		0, 		7		},
//-----------------------------------------------------------------------------------------------------------
//	02	MAIN MENU				* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,		&EditableItemValue,				0,			7,	DISP_IDX_BKGD_02_MAIN_MENU,	0,		0},
//-----------------------------------------------------------------------------------------------------------
//	03	PATCH					* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,		&EditableItemValue,				0,			3,	DISP_IDX_BKGD_03_PATCH+2,	0,		0},
	{DIT_SEL_DIM_RANGE,		&GroupOf12ChannelsIndex,		0,			47,		2,		0,		0		},
	{FDIT_STRING,			DispDmxStartAddress,			0,			55,		13,		0,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[0],					0,			10,		35,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[1],					0,			24,		35,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[2],					0,			38,		35,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[3],					0,			52,		35,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[4],					0,			10,		45,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[5],					0,			24,		45,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[6],					0,			38,		45,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[7],					0,			52,		45,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[8],					0,			10,		55,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[9],					0,			24,		55,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[10],					0,			38,		55,		3,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[11],					0,			52,		55,		3,		0		},
	{FDIT_P_SM_CHAR,		&FlashWritingProgress,			0,			60,		3,		2,		7		},
	{FDIT_12_CELL_GRIG,		&GroupOf12ChannelsIndex,		0,			0,		0,		0,		0		},
//-----------------------------------------------------------------------------------------------------------
//	04	DIMMER SETUP			* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BIG_DIMMER_NR,		&EditableItemValue,		.Num=DNW_ADD+1,		44,		2,		2,		0		},
	{DIT_TEXT_OFFSET,		&DispDimmSetup.Curve,	(unsigned char *)&TextNm_DimmCurve[1],	34,	20,	12,		0		},
	{DIT_NORM_NUM_DIVIDE,	&DispDimmSetup.Minimum, 		.Num=10,	45,		29,		0x31,	'%'		},
	{DIT_NORM_NUM_DIVIDE,	&DispDimmSetup.Maximum,		 .Num=10,	45,		38,		0x31,	'%'		},
	{DIT_TEXT_OFFSET,		&DispDimmSetup.Response, (unsigned char *)&TextNm_DimmResponse[1], 34, 47, 12,	0		},
	{FDIT_STRING,			&DispDimmSetupWZIR,				0,			52,	 	57,		4, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	05	UNIT SETTINGS			* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,		&EditableItemValue,				0,			5,	DISP_IDX_BKGD_05_UNIT_SETTINGS+3,	0,		0},
	{DIT_OK_CHECK_BOX,		&UnitSettings.WorklightEn,		0, 			60,	 	15,		0, 		0		},
	{DIT_OK_CHECK_BOX,		&UnitSettings.HoldLast,			0, 			60,	 	25,		0, 		0		},
	{FDIT_P_SM_CHAR,		&FlashWritingProgress,			0,			60,		3,		2,		7		},
	{DIT_NORMAL_NUMBER,		&UnitSettings.DisplayOffTime, 	0,			57,		34,		2,		0		},
	{DIT_OK_CHECK_BOX,		&UnitSettings.EthernetToXLR,	0, 			60,	 	55,		0, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	06	DMX INPUT CONTROL		* Origin							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,		&EditableItemValue,						0,			4,	DISP_IDX_BKGD_06_SOURCE_CTRL+3,0,		0		},
	{DIT_OK_CHECK_BOX,		&Eth_Disp_ETH_Enabled,					.Num=2,	  	60,		25, 	0,		0		},
	{DIT_OK_CHECK_BOX,		&Eth_Disp_ETH_Enabled,					.Num=1,		60,		45, 	0,		0		},
	{FDIT_P_SM_CHAR,		&FlashWritingProgress,					0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	07	DMX SOURCE STATUS 1/3 - XLR		* Origin			pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_OK_CHECK_BOX,		&DMX_Is_Present,		.Num=DMX_PR_XLR, 	60,		25,		0, 		0		},
	{DIT_NORMAL_NUMBER,		&DMX_XLR_NumOfRecChans,			0, 			55,	 	33,		3, 		0		},
	{DIT_OK_CHECK_BOX,		&DMX_Is_Present,		.Num=DMX_PR_RDM, 	60, 	45,		0, 		0		},
	{DIT_OK_CHECK_BOX,		&XLR_Display_Status,	.Num=XLR_MODE_OUTPUT_SIGNAL, 60, 	57,		0, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	08	INFO						* Origin				pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
//	{DIT_NORMAL_NUMBER,		&LineVoltage[0],				0, 			20,	 	14,		3, 		'V'		},
//	{DIT_NORMAL_NUMBER,		&LineVoltage[1],				0, 			20,	 	24,		3, 		'V'		},
//	{DIT_NORMAL_NUMBER,		&LineVoltage[2],				0, 			20,	 	34,		3, 		'V'		},
//	{DIT_NORM_NUM_DIVIDE,	&LineFrequency[0],			.Num=100,		42,	 	14,		0x22,	0		},
//	{DIT_NORM_NUM_DIVIDE,	&LineFrequency[1],			.Num=100,		42,	 	24,		0x22,	0		},
//	{DIT_NORM_NUM_DIVIDE,	&LineFrequency[2],			.Num=100,		42,	 	34,		0x22,	0		},
	{DIT_NORM_NUM_DIVIDE,	&DisplayMcuTemperature,		.Num=10,		45,	 	44,		0x21,	0		},
	{DIT_NORM_NUM_DIVIDE,	&DisplayExtTemperature,		.Num=10,		45,	 	54,		0x21,	0		},
//-----------------------------------------------------------------------------------------------------------
//	09	SERVICE					* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,		&EditableItemValue,				0,			6,	DISP_IDX_BKGD_09_SERVICE+3,0,		0		},
	{DIT_OK_CHECK_BOX,		&UnitSettings.ManualControl,	.Num=1,		60,	 	14,		0, 		0		},
	{FDIT_P_SM_CHAR,		&FlashWritingProgress,			0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	10	popup display START ADDRESS							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BIG_NUMBERS,		&EditableItemValue,				0,			44,		9,		3,		0		},
//-----------------------------------------------------------------------------------------------------------
//	11	INDIVIDUAL PATCH		* Origin						pData				x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_MENU_NUMBER,		&DispPatch[0],		&MenuEnterButton_SelectedChannel,	10,		35,		3,		0		},
	{DIT_MENU_NUMBER,		&DispPatch[1],		&MenuEnterButton_SelectedChannel,	24,		35,		3,		1		},
	{DIT_MENU_NUMBER,		&DispPatch[2],		&MenuEnterButton_SelectedChannel,	38,		35,		3,		2		},
	{DIT_MENU_NUMBER,		&DispPatch[3],		&MenuEnterButton_SelectedChannel,	52,		35,		3,		3		},
	{DIT_MENU_NUMBER,		&DispPatch[4],		&MenuEnterButton_SelectedChannel,	10,		45,		3,		4		},
	{DIT_MENU_NUMBER,		&DispPatch[5],		&MenuEnterButton_SelectedChannel,	24,		45,		3,		5		},
	{DIT_MENU_NUMBER,		&DispPatch[6],		&MenuEnterButton_SelectedChannel,	38,		45,		3,		6		},
	{DIT_MENU_NUMBER,		&DispPatch[7],		&MenuEnterButton_SelectedChannel,	52,		45,		3,		7		},
	{DIT_MENU_NUMBER,		&DispPatch[8],		&MenuEnterButton_SelectedChannel,	10,		55,		3,		8		},
	{DIT_MENU_NUMBER,		&DispPatch[9],		&MenuEnterButton_SelectedChannel,	24,		55,		3,		9		},
	{DIT_MENU_NUMBER,		&DispPatch[10],		&MenuEnterButton_SelectedChannel,	38,		55,		3,		10		},
	{DIT_MENU_NUMBER,		&DispPatch[11],		&MenuEnterButton_SelectedChannel,	52,		55,		3,		11		},
	{DIT_MENU_NUMBER_ACT,	&MenuEnterButton_SelectedChannel,	0,	REF_SCR_11_IDX,		12,		0,		0		},
//	{DIT_12_NUMS_IN_GRID,	Patch,							0, 			10,	 	35,		3, 		0		},
	{FDIT_P_SM_CHAR,		&FlashWritingProgress,			0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	12	ONE DIMMER SETUP					* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,		&EditableItemValue,					0,			5,	DISP_IDX_BKGD_12_ONE_DIMM_SET+4,0,		0		},
	{DIT_TEXT_OFFSET,		&DispDimmSetup.Curve,				(unsigned char *)&TextNm_DimmCurve[1],	34,	20,	12,		0		},
	{DIT_NORM_NUM_DIVIDE,	&DispDimmSetup.Minimum,			 	.Num=10,	45,		29,		0x31,	'%'		},
	{DIT_NORM_NUM_DIVIDE,	&DispDimmSetup.Maximum, 			.Num=10,	45,		38,		0x31,	'%'		},
	{DIT_TEXT_OFFSET,		&DispDimmSetup.Response,			 (unsigned char *)&TextNm_DimmResponse[1], 34, 47, 12,	0		},
	{FDIT_STRING,			&DispDimmSetupWZIR,					0,			52,	 	57,		4, 		0		},
	{FDIT_P_SM_CHAR,		&FlashWritingProgress,				0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	13	popup display DIMMER MINIMUM						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_NORM_NUM_DIVIDE,	&EditableItemValue,				.Num=10,	43,		28,		0x31,		'%'		},
//-----------------------------------------------------------------------------------------------------------
//	14	popup display DIMMER MAXIMUM						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_NORM_NUM_DIVIDE,	&EditableItemValue,				.Num=10,	43,		38,		0x31,		'%'		},
//-----------------------------------------------------------------------------------------------------------
//	15	popup display DISPLAY OFF TIME						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BIG_NUMBERS,		&EditableItemValue,				0,			48,		29,		2,		0		},
//-----------------------------------------------------------------------------------------------------------
//	16	SERVICE - DIMMERCONTROLLER							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_NORM_NUM_DIVIDE,	&DisplayFanTemperature,		.Num=10,		45,	 	13,		0x21,	0		},
	{DIT_NORMAL_NUMBER,		&Display_fan_output,			0, 			51,	 	23,		3, 		'%'		},
	{DIT_OK_CHECK_BOX,		&Display_fan_tacho,				0, 			60,	 	33,		3, 		96		},
//-----------------------------------------------------------------------------------------------------------
//	17	SERVICE - DMX VALUES								pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_SEL_DIM_RANGE,		&GroupOf12ChannelsIndex,		0,			47,		2,		0,		0		},
	{DIT_12_NUMS_FOR_DMX,	&GroupOf12ChannelsIndex,		0,			0,		13,		0,		0		},
	{DIT_NORMAL_NUMBER,		&DispPatch[0],					0, 			8,	 	12,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[1],					0, 			8,	 	21,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[2],					0, 			8,	 	30,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[3],					0, 			8,	 	39,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[4],					0, 			8,	 	48,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[5],					0, 			8,	 	57,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[6],					0, 			42,	 	12,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[7],					0, 			42,	 	21,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[8],					0, 			42,	 	30,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[9],					0, 			42,	 	39,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[10],					0, 			42,	 	48,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispPatch[11],					0,	 		42,	 	57,		3, 		':'		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[0],				0, 			21,	 	12,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[1],				0, 			21,	 	21,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[2],				0, 			21,	 	30,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[3],				0, 			21,	 	39,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[4],				0, 			21,	 	48,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[5],				0, 			21,	 	57,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[6],				0, 			55,	 	12,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[7],				0, 			55,	 	21,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[8],				0, 			55,	 	30,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[9],				0, 			55,	 	39,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[10],				0, 			55,	 	48,		3, 		0		},
	{DIT_NORMAL_NUMBER,		&DispDmxValue[11],				0, 			55,	 	57,		3, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	18	SERVICE - ABOUT										pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//	19	SERVICE - PLUGIN CRMX - NEPOUZITO!!!!!!!!!!!!!								pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
//	{DIT_NORMAL_NUMBER,		&CRMX_LinkQuality,				0, 			50,	 	30,		3, 		'%'		},
//	{DIT_DYNAMIC_DISP,		&CRMX_DynamicDispUnlink,	.Num=(0+2)<<16,	2,		45,		60,		18		},
//-----------------------------------------------------------------------------------------------------------
//	20	DMX SOURCE STATUS - CRMX - NEPOUZITO!!!!!!!!!!!!!	* Origin						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
//	{	DIT_OK_CHECK_BOX,		&CRMX_Status,		.Num=CRMX_STS_ENABLED,			60,		21,		0, 		0		},
//	{	DIT_OK_CHECK_BOX,		&CRMX_Status,		.Num=CRMX_STS_MODULE_PRESENT,	60,		30,		0, 		0		},
//	{	DIT_OK_CHECK_BOX,		&CRMX_Status,		.Num=CRMX_STS_LINKED_TO_TRANSC,	60,		39,		0, 		0		},
//	{	DIT_OK_CHECK_BOX,		&CRMX_Status,		.Num=CRMX_STS_RADIO_LINK,		60,	 	48,		0, 		0		},
//	{	DIT_OK_CHECK_BOX,		&CRMX_Status,		.Num=CRMX_STS_DMX_OK,			60,	 	57,		0, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	21	DMX SOURCE STATUS 2/2 - ETHERNET	* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_OK_CHECK_BOX,		&Eth_EthernetStatus,	.Num=ETHERNET_STS_ENABLED_ARTNET+ETHERNET_STS_ENABLED_SACN,	60,		21,		0, 		0		},
	{	DIT_OK_CHECK_BOX,		&Eth_EthernetStatus,	.Num=ETHERNET_STS_PLUGIN_MODULE,60,		30,		0, 		0		},
	{	DIT_OK_CHECK_BOX,		&Eth_EthernetStatus,	.Num=ETHERNET_STS_LINK,		60,	 	39,		0, 		0		},
	{	DIT_OK_CHECK_BOX,		&Eth_EthernetStatus,	.Num=ETHERNET_STS_PROTOCOL,	60,	 	48,		0, 		0		},
	{	DIT_OK_CHECK_BOX,		&Eth_EthernetStatus,	.Num=ETHERNET_STS_DMX_OK,	60,	 	57,		0, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	22	SACN UNIVERSE											pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_INVERSE_MENU,		&EditableItemValue,				0,			3,	DISP_IDX_BKGD_22_+4,0,		0		},
	{	FDIT_P_SM_CHAR,			&FlashWritingProgress,			0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	23	popup display SACN UNIVERSE 100s						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,		.Num=DNW_DIV+100,	42,		19,		3,		0		},		// Divide 100
//-----------------------------------------------------------------------------------------------------------
//	24	popup display SACN UNIVERSE 1s							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,		.Num=DNW_MOD+100,	49,		29,		2,		0		},		// Modulo 100
//-----------------------------------------------------------------------------------------------------------
//	25	popup display SACN UNIVERSE								pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,				0,			31,		46,		5,		0		},
//-----------------------------------------------------------------------------------------------------------
//	26	ARTNET UNIVERSE (NET:SUBNET:UNIVERSE)						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_INVERSE_MENU,		&EditableItemValue,					0,			3,	DISP_IDX_BKGD_26_+4,0,		0		},
	{	FDIT_P_SM_CHAR,			&FlashWritingProgress,				0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	27	popup display ARTNET UNIVERSE - NET							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,			.Num=DNW_SHR+8,		42,		19,		3,		0		},		// cislo se vydeli 256 a potom se ukaze
//-----------------------------------------------------------------------------------------------------------
//	28	popup display ARTNET UNIVERSE - SUB-NET						pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,			.Num=DNW_SR4M,		49,		29,		2,		0		},		// cislo se vydeli 16 a potom se ukaze, ale jen 4 bity
//-----------------------------------------------------------------------------------------------------------
//	29	popup display ARTNET UNIVERSE - UNIVERSE					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,			.Num=DNW_MSK+0xF,	49,		39,		2,		0		},		// ukazuji se jen dolni 4 bity
//-----------------------------------------------------------------------------------------------------------
//	30	SERVICE: PLUGIN - ETHERNET									pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_NUM_IP_ADDR,		&Eth_IP_Address,							0,			20,		12,		0,		0	},
	{	DIT_NUM_MAC_ADDR,		Eth_MAC_Address,						0,			13,		21,		0,		0	},
	{	FDIT_STRING,			Eth_Mod_HwVerTxt,						0,			11,		30,		0,		0	},
	{	FDIT_STRING,			Eth_Mod_FwVerTxt,						0,			51,		30,		0,		0	},
	{	DIT_NORMAL_NUMBER,		&Eth_ReceivedFPS,						0,			35,		39,		4,		'/'	},
	{	DIT_NORMAL_NUMBER,		&Eth_AcceptedFPS,						0,			51,		39,		4,		0	},
	{	DIT_NUM_IN_HEXA,		&Eth_ModuleStatusBits,					0,			51,		48,		2,		0	},
	{	FDIT_P_SM_CHAR,			&FlashWritingProgress,					0,			60,		3,		2,		7},
//-----------------------------------------------------------------------------------------------------------
//	31	IP ADDRESS SETTING												pData			x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_INVERSE_MENU,		&EditableItemValue,						0,			4,		DISP_IDX_BKGD_31_+3,	0,		0		},
	{	DIT_NORMAL_NUMBER,		&Eth_DisplayIpAddress[0],				0,			41,		17,		3,		0},
	{	DIT_NORMAL_NUMBER,		&Eth_DisplayIpAddress[1],				0,			41,		27,		3,		0},
	{	DIT_NORMAL_NUMBER,		&Eth_DisplayIpAddress[2],				0,			41,		37,		3,		0},
	{	DIT_NORMAL_NUMBER,		&Eth_DisplayIpAddress[3],				0,			41,		47,		3,		0},
	{	FDIT_P_SM_CHAR,			&FlashWritingProgress,					0,			60,		3,		2,		7},
//-----------------------------------------------------------------------------------------------------------
//	32	IP ADDRESS A SETTING											pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,						0,			42,		15,		3,		0	},
//-----------------------------------------------------------------------------------------------------------
//	33	IP ADDRESS B SETTING											pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,						0,			42,		23,		3,		0	},
//-----------------------------------------------------------------------------------------------------------
//	34	IP ADDRESS C SETTING											pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,						0,			42,		33,		3,		0	},
//-----------------------------------------------------------------------------------------------------------
//	35	IP ADDRESS D SETTING											pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{	DIT_BIG_NUMBERS,		&EditableItemValue,						0,			42,		43,		3,		0	},
//-----------------------------------------------------------------------------------------------------------
//	36	MANUAL CONTROL - SELECT GROUP									pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BAR_CHART_38,			&DmxDisplay[0],							0,			3,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[1],							0,			8,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[2],							0,			13,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[3],							0,			18,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[4],							0,			23,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[5],							0,			28,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[6],							0,			33,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[7],							0,			38,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[8],							0,			43,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[9],							0,			48,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[10],						0,			53,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[11],						0,			58,		59,		2,		0		},
	{FDIT_P_SM_CHAR,			&FlashWritingProgress,					0,			60,		3,		2,		7		},
	{DIT_SEL_DIM_RNG_SM,		&GroupOf12ChannelsIndex,				0,			45,		3,		0,		0		},
	{DIT_12_SMALL_TOP_NUMS,		&GroupOf12ChannelsIndex,				0,			3,		12,		2,		3		},
//--------------------------------------------------------------------------------------------
//  37 popup display CONFIRM FACTORY SETTINGS
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//  38 popup display CONFIRM CRMX UNLINK
//--------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//	39	ADVANCED							* Origin				pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,		&EditableItemValue,						0,			4,		DISP_IDX_BKGD_39_+3, 0,		0		},
	{DIT_OK_CHECK_BOX,		&UnitSettings.AuditoriumDisabledIfDmx,	.Num=1,		60,	 	31,		0, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	40	SET DIGITAL INPUTS		* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[0].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	10,		35,		3,		0		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[1].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	24,		35,		3,		1		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[2].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	38,		35,		3,		2		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[3].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	52,		35,		3,		3		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[4].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	10,		45,		3,		4		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[5].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	24,		45,		3,		5		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[6].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	38,		45,		3,		6		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[7].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	52,		45,		3,		7		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[8].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	10,		55,		3,		8		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[9].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	24,		55,		3,		9		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[10].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	38,		55,		3,		10		},
	{DIT_MENU_NUMBER_ONE_CHR,		&DimmerSetup[11].SvetloDiaTma,		&MenuEnterButton_SelectedChannel,	52,		55,		3,		11		},
	{DIT_MENU_NUMBER_ACT,			&MenuEnterButton_SelectedChannel,		0,	REF_SCR_40_IDX,		12,		0,		0		},
	{FDIT_P_SM_CHAR,				&FlashWritingProgress,					0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	41	DIA LEVELS							* Origin				pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,				&EditableItemValue,					0,			4,		DISP_IDX_BKGD_41_+3, 0,		0		},
	{DIT_NORMAL_NUMBER,				&UnitSettings.AuditoriumFadeTime,	0, 			51,	 	15,		3, 		's'		},
	{DIT_NORMAL_NUMBER,				&UnitSettings.DiaLevel[0],			0, 			51,	 	24,		3, 		'%'		},
	{DIT_NORMAL_NUMBER,				&UnitSettings.DiaLevel[1],			0, 			51,	 	33,		3, 		'%'		},
	{DIT_NORMAL_NUMBER,				&UnitSettings.DiaLevel[2],			0, 			51,	 	42,		3, 		'%'		},
	{FDIT_P_SM_CHAR,				&FlashWritingProgress,				0,			60,		3,		2,		7		},
//-----------------------------------------------------------------------------------------------------------
//	42	popup display FADE TIME								pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BIG_NUMBERS,				&EditableItemValue,				0,			44,		8,		3,		0		},
//-----------------------------------------------------------------------------------------------------------
//	43	popup display DIA LEVEL 1							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BIG_NUMBERS,				&EditableItemValue,				0,			44,		17,		3,		0		},
//-----------------------------------------------------------------------------------------------------------
//	44	popup display DIA LEVEL 2							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BIG_NUMBERS,				&EditableItemValue,				0,			44,		26,		3,		0		},
//-----------------------------------------------------------------------------------------------------------
//	45	popup display DIA LEVEL 3							pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BIG_NUMBERS,				&EditableItemValue,				0,			44,		35,		3,		0		},
//-----------------------------------------------------------------------------------------------------------
//	46	SERVICE ABOUT 2 DIMMERS		* Origin					pData	x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_SEL_DIM_RANGE,			&GroupOf12ChannelsIndex,		0,		47,		2,		0,		0		},
	{DIT_12_NUMS_FOR_DMX,		&GroupOf12ChannelsIndex,		0,		0,		13,		0,		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[0].HwFwSt,		0,		9, 		12,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[1].HwFwSt,		0,		9, 		21,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[2].HwFwSt,		0,		9, 		30,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[3].HwFwSt,		0,		9, 		39,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[4].HwFwSt,		0,		9, 		48,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[5].HwFwSt,		0,		9, 		57,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[6].HwFwSt,		0,		43,		12,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[7].HwFwSt,		0,		43,		21,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[8].HwFwSt,		0,		43,		30,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[9].HwFwSt,		0,		43,		39,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[10].HwFwSt,		0,		43,		48,		7, 		0		},
	{DIT_DIMMER_VERSION,		&DispSerialDimm[11].HwFwSt,		0,		43,		57,		7, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	47	ONE ADVANCED DIMMER SETUP				* Origin		pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_INVERSE_MENU,			&EditableItemValue,				0,			4,	DISP_IDX_BKGD_47_+4,	0,	0		},
	{DIT_OK_CHK_BOX_PLUS,		&DispDimmSetWZIR[0],			0,			60,	 	21,		0, 		0		},
	{DIT_OK_CHK_BOX_PLUS,		&DispDimmSetWZIR[1],			0,			60,	 	30,		0, 		0		},
	{DIT_OK_CHK_BOX_PLUS,		&DispDimmSetWZIR[2],			0,			60,	 	39,		0, 		0		},
	{DIT_OK_CHK_BOX_PLUS,		&DispDimmSetWZIR[3],			0,			60,	 	48,		0, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	48	INFO 2 DIMMERS				* Origin					pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_SEL_DIM_RANGE,			&GroupOf12ChannelsIndex,			0,		47,		2,		0,		0		},
	{DIT_12_NUMS_FOR_DMX,		&GroupOf12ChannelsIndex,			0,		0,		13,		0,		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[0].Runtime,			0,		9, 		12,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[1].Runtime,			0,		9, 		21,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[2].Runtime,			0,		9, 		30,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[3].Runtime,			0,		9, 		39,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[4].Runtime,			0,		9, 		48,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[5].Runtime,			0,		9, 		57,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[6].Runtime,			0,		43,		12,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[7].Runtime,			0,		43,		21,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[8].Runtime,			0,		43,		30,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[9].Runtime,			0,		43,		39,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[10].Runtime,		0,		43,		48,		7, 		0		},
	{DIT_DIMMER_STATUS,			&DispSerialDimm[11].Runtime,		0,		43,		57,		7, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	49	INFO 3 DIMMERS				* Origin					pData					x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[0].DispChng,	SerialDimmer[0].DimRuntime,	9, 		12,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[1].DispChng,	SerialDimmer[1].DimRuntime,	9, 		21,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[2].DispChng,	SerialDimmer[2].DimRuntime,	9, 		30,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[3].DispChng,	SerialDimmer[3].DimRuntime,	9, 		39,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[4].DispChng,	SerialDimmer[4].DimRuntime,	9, 		48,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[5].DispChng,	SerialDimmer[5].DimRuntime,	9, 		57,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[6].DispChng,	SerialDimmer[6].DimRuntime,	43,		12,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[7].DispChng,	SerialDimmer[7].DimRuntime,	43,		21,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[8].DispChng,	SerialDimmer[8].DimRuntime,	43,		30,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[9].DispChng,	SerialDimmer[9].DimRuntime,	43,		39,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[10].DispChng,	SerialDimmer[10].DimRuntime,43,		48,		7, 		0		},
//	{DIT_DIMMER_RUNTIME,		&SerialDimmer[11].DispChng,	SerialDimmer[11].DimRuntime,43,		57,		7, 		0		},
//-----------------------------------------------------------------------------------------------------------
//	50	MANUAL CONTROL - CHANNELLS										pData		x		y		digits	dimm
//-----------------------------------------------------------------------------------------------------------
	{DIT_BAR_CHART_38,			&DmxDisplay[0],							0,			3,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[1],							0,			8,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[2],							0,			13,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[3],							0,			18,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[4],							0,			23,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[5],							0,			28,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[6],							0,			33,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[7],							0,			38,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[8],							0,			43,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[9],							0,			48,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[10],						0,			53,		59,		2,		0		},
	{DIT_BAR_CHART_38,			&DmxDisplay[11],						0,			58,		59,		2,		0		},
	{DIT_SELECT_FRAME,			&MenuEnterButton_SelectedChannel,		0,			0,		0,		0,		0		},
	{FDIT_P_SM_CHAR,			&FlashWritingProgress,					0,			60,		3,		2,		7		},
};

const TDisplayItemPointers DisplayRefreshItemsPtr[DISP_MAX_SCREEN_NR+1] = {
		{REF_SCR_00_IDX,	REF_SCR_00_LEN},
		{REF_SCR_01_IDX,	REF_SCR_01_LEN},
		{REF_SCR_02_IDX,	REF_SCR_02_LEN},
		{REF_SCR_03_IDX,	REF_SCR_03_LEN},
		{REF_SCR_04_IDX,	REF_SCR_04_LEN},
		{REF_SCR_05_IDX,	REF_SCR_05_LEN},
		{REF_SCR_06_IDX,	REF_SCR_06_LEN},
		{REF_SCR_07_IDX,	REF_SCR_07_LEN},
		{REF_SCR_08_IDX,	REF_SCR_08_LEN},
		{REF_SCR_09_IDX,	REF_SCR_09_LEN},
		{REF_SCR_10_IDX,	REF_SCR_10_LEN},
		{REF_SCR_11_IDX,	REF_SCR_11_LEN},
		{REF_SCR_12_IDX,	REF_SCR_12_LEN},
		{REF_SCR_13_IDX,	REF_SCR_13_LEN},
		{REF_SCR_14_IDX,	REF_SCR_14_LEN},
		{REF_SCR_15_IDX,	REF_SCR_15_LEN},
		{REF_SCR_16_IDX,	REF_SCR_16_LEN},
		{REF_SCR_17_IDX,	REF_SCR_17_LEN},
		{REF_SCR_18_IDX,	REF_SCR_18_LEN},
		{REF_SCR_19_IDX,	REF_SCR_19_LEN},
		{REF_SCR_20_IDX,	REF_SCR_20_LEN},
		{REF_SCR_21_IDX,	REF_SCR_21_LEN},
		{REF_SCR_22_IDX,	REF_SCR_22_LEN},
		{REF_SCR_23_IDX,	REF_SCR_23_LEN},
		{REF_SCR_24_IDX,	REF_SCR_24_LEN},
		{REF_SCR_25_IDX,	REF_SCR_25_LEN},
		{REF_SCR_26_IDX,	REF_SCR_26_LEN},
		{REF_SCR_27_IDX,	REF_SCR_27_LEN},
		{REF_SCR_28_IDX,	REF_SCR_28_LEN},
		{REF_SCR_29_IDX,	REF_SCR_29_LEN},
		{REF_SCR_30_IDX,	REF_SCR_30_LEN},
		{REF_SCR_31_IDX,	REF_SCR_31_LEN},
		{REF_SCR_32_IDX,	REF_SCR_32_LEN},
		{REF_SCR_33_IDX,	REF_SCR_33_LEN},
		{REF_SCR_34_IDX,	REF_SCR_34_LEN},
		{REF_SCR_35_IDX,	REF_SCR_35_LEN},
		{REF_SCR_36_IDX,	REF_SCR_36_LEN},
		{REF_SCR_37_IDX,	REF_SCR_37_LEN},
		{REF_SCR_38_IDX,	REF_SCR_38_LEN},
		{REF_SCR_39_IDX,	REF_SCR_39_LEN},
		{REF_SCR_40_IDX,	REF_SCR_40_LEN},
		{REF_SCR_41_IDX,	REF_SCR_41_LEN},
		{REF_SCR_42_IDX,	REF_SCR_42_LEN},
		{REF_SCR_43_IDX,	REF_SCR_43_LEN},
		{REF_SCR_44_IDX,	REF_SCR_44_LEN},
		{REF_SCR_45_IDX,	REF_SCR_45_LEN},
		{REF_SCR_46_IDX,	REF_SCR_46_LEN},
		{REF_SCR_47_IDX,	REF_SCR_47_LEN},
		{REF_SCR_48_IDX,	REF_SCR_48_LEN},
		{REF_SCR_49_IDX,	REF_SCR_49_LEN},
		{REF_SCR_50_IDX,	REF_SCR_50_LEN},
		{REF_SCR_51_IDX,	REF_SCR_51_LEN},
		{REF_SCR_52_IDX,	REF_SCR_52_LEN},
		{REF_SCR_53_IDX,	REF_SCR_53_LEN},
		{REF_SCR_54_IDX,	REF_SCR_54_LEN},
		{REF_SCR_55_IDX,	REF_SCR_55_LEN},
		{REF_SCR_56_IDX,	REF_SCR_56_LEN},
		{REF_SCR_57_IDX,	REF_SCR_57_LEN},
		{REF_SCR_58_IDX,	REF_SCR_58_LEN},
		{REF_SCR_59_IDX,	REF_SCR_59_LEN},
		{REF_SCR_60_IDX,	REF_SCR_60_LEN},
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DYNAMIC DISPLAY ITEMS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

const TDisplayItemStruct DisplayDynamicItems[DYNAMIC_DISPLAY_ITEMS_COUNT] =
{
//--------------------------------------------------------------------------------------------
// 00 dynamic display CRMX LINKED								pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_SM_CHAR,			{"PRESS"},							{0},		3,		48,		{0},	{5}},
	{FDIT_BUTTON,			BUTTON_DOWN_ICON,					0,			14,		46,		0, 		5},
	{FDIT_SM_CHAR,			"TO UNLINK DEVICE FROM",			0,			20,		48,		0, 		5},
	{FDIT_SM_CHAR,			"TRANSMITTER",						0,			21,		56,		0, 		5},
//--------------------------------------------------------------------------------------------
// 01 dynamic display CRMX MODULE IS NOT PRESENT				pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_SM_CHAR,			"CRMX MODULE IS NOT PRESENT",		0,			6,		51,		0, 		5},
//--------------------------------------------------------------------------------------------
// 02 dynamic display UNLINKED CRMX MODULE						pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_SM_CHAR,			"DEVICE IS NOT LINKED",				0,			12,		48,		0, 		5},
	{FDIT_SM_CHAR,			"LINK DEVICE BY TRANSMITTER",		0,			6,		56,		0, 		5},
//--------------------------------------------------------------------------------------------
// 03 dynamic display SAVE CALIBRATION DATA						pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_SM_CHAR,			"PRESS",							0,			4,		48,		0, 		5},
	{FDIT_BUTTON,			BUTTON_UP_ICON,						0,			15,		46,		0, 		5},
	{FDIT_SM_CHAR,			"TO SAVE CALIBRATION",				0,			21,		48,		0, 		5},
	{FDIT_SM_CHAR,			"DATA TO MEMORY",					0,			19,		56,		0, 		5},
//--------------------------------------------------------------------------------------------
// 04 dynamic display NO DMX FOR SAVE CALIBRATION DATA			pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_SM_CHAR,			"DMX SIGNAL MUST BE PRESENT",		0,			6,		48,		0, 		5},
	{FDIT_SM_CHAR,			"FOR SAVING CALIBRATION DATA",		0,			5,		56,		0, 		5},
//--------------------------------------------------------------------------------------------
// 05 dynamic display CALIBRATION DATA SAVED					pData		x		y		w 		h
//--------------------------------------------------------------------------------------------
	{FDIT_SM_CHAR,			"CALIBRATION DATA SAVED",			0,			10,		52,		0, 		5},
};

const TDisplayItemPointers	DisplayDynamicItemsPtr[DISP_DYNAMIC_SCREEN_COUNT] = {
		{DISP_DYN_IDX_00_,		DISP_DYN_LEN_00_},
		{DISP_DYN_IDX_01_,		DISP_DYN_LEN_01_},
		{DISP_DYN_IDX_02_,		DISP_DYN_LEN_02_},
		{DISP_DYN_IDX_03_,		DISP_DYN_LEN_03_},
		{DISP_DYN_IDX_04_,		DISP_DYN_LEN_04_},
		{DISP_DYN_IDX_05_,		DISP_DYN_LEN_05_},
};

// ---
// Akce pro tlacitko (vetsinou) Enter
// U polozky TDisplayButtonsFnc.Enter je uvedena adresa prvni polozky. Po stlaceni
// tlacitka se vykona akce udana polozkou na kterou to ukazuje + se pricita EditableItemValue,
// takze se vykonava akce dle aktivni polozky menu.
// ---
const TButtonEnterAction ButtonEnterAction[31] =
{
// 03 PATCH					- index 0
		{BEA_INCREMENT_VARIABLE + 3,			{&GroupOf12ChannelsIndex}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_POPUP_PATCH_STRT_ADR}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_PATCH_INDIVIDUAL_DIMMERS}},
// 05 UNIT SETTINGS			- index 3
		{BEA_ON_OFF_VARIABLE,					{&UnitSettings.WorklightEn}},
		{BEA_ON_OFF_VARIABLE,					{&UnitSettings.HoldLast}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_POPUP_DISPLAY_OFF_TIME}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_POPUP_FACTORY_SETTINGS}},
		{BEA_ON_OFF_VARIABLE,					{&UnitSettings.EthernetToXLR}},
// 06 DMX INPUT CONTROL 	- index 8
		{BEA_ON_SINGLE_BIT_OR_OFF+2,			{&Eth_Disp_ETH_Enabled}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_SACN_UNIVERSE}},
		{BEA_ON_SINGLE_BIT_OR_OFF+1,			{&Eth_Disp_ETH_Enabled}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_ARTNET_NSNU}},
// 09 SERVICE			 	- index 12
		{BEA_ON_OFF_BIT + 0,					{&UnitSettings.ManualControl}},	// 0=bit number
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_MANUAL_CONTROL_GRP}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_SERVICE_DMX_VALUES}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_ADVANCED}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_SERVICE_PLUGIN_ETHERNET}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_SERVICE_ABOUT}},
// 12 ONE DIMMER SETUP	 	- index 18
		{BEA_INCREMENT_VARIABLE + 5,			{&DispDimmSetup.Curve}},		// 6 kurves
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_POPUP_DIMM_MINIMUM}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_POPUP_DIMM_MAXIMUM}},
		{BEA_INCREMENT_VARIABLE + 2,			{&DispDimmSetup.Response}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_1_ADVANCED_DIMMER_SETUP}},
// 39 ADVANCED MENU		 	- index 23
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_SET_DIGITAL_INPUTS}},
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_DIA_LEVELS}},
		{BEA_ON_SINGLE_BIT_OR_OFF + 1,			{&UnitSettings.AuditoriumDisabledIfDmx}},	// 0=bit number};
		{BEA_DISPLAY_SCREEN,					{.ScrNr=DISP_NR_SERVICE_DEVICE_DATA}},
// 47 ADVANCED_ONE DIMMER_SETUP - index 27
		{BEA_ON_SINGLE_BIT_OR_OFF + 1,			{&DispDimmSetWZIR[0]}},	// 0=bit number};
		{BEA_ON_SINGLE_BIT_OR_OFF + 1,			{&DispDimmSetWZIR[1]}},	// 0=bit number};
		{BEA_ON_SINGLE_BIT_OR_OFF + 1,			{&DispDimmSetWZIR[2]}},	// 0=bit number};
		{BEA_ON_SINGLE_BIT_OR_OFF + 1,			{&DispDimmSetWZIR[3]}},	// 0=bit number};
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DISPLAY BUTTON FUNCTIONS DEFINES
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define TOT(t)		(t/8)
const TDisplayButtonsFnc		DisplayButtonsFunc[DISP_MAX_SCREEN_NR+1] =
{
//-------------------------------------------------------------------------------------------------
//			Timeout				EditVar		Type		Escape					Down					Up						Enter
//-------------------------------------------------------------------------------------------------
// 00 LOGO ----------------------------------------------------
	{
/* Timeout  */	TOT(1000),	//
/* Edit Var */	0,
/* Type     */	0,
/* Escape	*/	DISP_NR_MAIN_SCREEN,
/* Down		*/	0,
/* Up		*/	0,
/* Enter	*/	0
	},
// 01 MAIN SCREEN ---------------------------------------------
	{
/* Timeout  */	TOT(0),
/* Edit Var */	(int*)&GroupOf12ChannelsIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_INCREMENT_VARIABLE | DTP_ALWAYS_STORE_VARIABLE,
/* Escape	*/	DISP_NR_MAIN_MENU, //DISP_NR_MAIN_SCR_HELP,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	3
	},
// 02 MAIN MENU
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	&MainMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DYNAMIC_DISP | DTP_ALWAYS_STORE_VARIABLE | BTN_UP_DN_REVERSE_FUNC,
/* Escape	*/	DISP_NR_MAIN_SCREEN,
/* Down		*/	0,
/* Up		*/	6,
/* Enter	*/	DISP_NR_PATCH	// Vysledny display je 3 + MainMenuItemIndex
	},
// 03 PATCH
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	&PatchMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_MENU_REFERENCE_ADR | DTP_ALWAYS_STORE_VARIABLE | BTN_UP_DN_REVERSE_FUNC,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	0,
/* Up		*/	2,
/* Enter	*/	.EnterAction=&ButtonEnterAction[0] // Ukazatel na prvni akci pri EditableItemValue=0.
	},
// 04 DIMMER SETUP
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	&DimmerSetupDimmerIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DISPLAY | DTP_ALWAYS_STORE_VARIABLE | DTP_ACTION_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	0,
/* Up		*/	NUM_OF_CHANNELS,
/* Enter	*/	DISP_NR_1_DIMMER_SETUP
	},
// 05 UNIT SETTINGS
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	&UnitSettingsMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_MENU_REFERENCE_ADR | BTN_UP_DN_REVERSE_FUNC | DTP_SAVE_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	0,
/* Up		*/	4,
/* Enter	*/	.EnterAction=&ButtonEnterAction[3] // Ukazatel na prvni akci pri EditableItemValue=0.
	},
// 06 DMX INPUT CONTROL
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	&InputCtrlMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_MENU_REFERENCE_ADR | BTN_UP_DN_REVERSE_FUNC,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	.EnterAction=&ButtonEnterAction[8] // Ukazatel na prvni akci pri EditableItemValue=0.
	},
// 07 DMX SOURCE STATUS 1/3 - XLR
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	DISP_NR_DMX_INP_STATUS_ETHERNET,
/* Up		*/	DISP_NR_DMX_INP_STATUS_ETHERNET,
/* Enter	*/	DISP_NR_DMX_INP_STATUS_ETHERNET
	},
// 08 INFO
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_MAIN_SCREEN,
/* Down		*/	DISP_NR_INFO_2_DIMMERS,
/* Up		*/	DISP_NR_INFO_2_DIMMERS,
/* Enter	*/	DISP_NR_INFO_2_DIMMERS
	},
// 09 SERVICE
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	&ServiceMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_MENU_REFERENCE_ADR | BTN_UP_DN_REVERSE_FUNC | DTP_SAVE_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	0,
/* Up		*/	5,
/* Enter	*/	.EnterAction=&ButtonEnterAction[12] // Ukazatel na prvni akci pri EditableItemValue=0.
	},
// 10 popup display START ADDRESS					----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&DmxStartAddress,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_PATCH_FROM_START_ADR | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_PATCH,
/* Down		*/	1,
/* Up		*/	465,
/* Enter	*/	DISP_NR_PATCH
	},
// 11 INDIVIDUAL PATCH
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	&MenuUpDown_EditedValue,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_SELECTED_CHAN | DTP_ACTION_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_PATCH,
/* Down		*/	1,
/* Up		*/	512,
/* Enter	*/	0
	},
// 12 ONE DIMMER SETUP MENU
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	&OneDimmerSetupMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_MENU_REFERENCE_ADR | BTN_UP_DN_REVERSE_FUNC | DTP_SAVE_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_DIMMERS_SETUP,
/* Down		*/	0,
/* Up		*/	4,
/* Enter	*/	.EnterAction=&ButtonEnterAction[18] // Ukazatel na prvni akci pri EditableItemValue=0.
	},
// 13 popup display DIMMER MINIMUM					----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&DispDimmSetup.Minimum,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_1_DIMMER_SETUP,
/* Down		*/	0,
/* Up		*/	250,
/* Enter	*/	DISP_NR_1_DIMMER_SETUP
	},
// 14 popup display DIMMER MAXIMUM					----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&DispDimmSetup.Maximum,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_1_DIMMER_SETUP,
/* Down		*/	500,
/* Up		*/	1000,
/* Enter	*/	DISP_NR_1_DIMMER_SETUP
	},
// 15 popup display UNIT SETTINGS: DISPLAY OFF TIME	----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&UnitSettings.DisplayOffTime,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_UNIT_SETTINGS,
/* Down		*/	0,
/* Up		*/	90,
/* Enter	*/	DISP_NR_UNIT_SETTINGS
	},
// 16 SERVICE - DIMMERCONTROLLER
	{
/* Timeout  */	TOT(500000), // 500 sekund
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_ADVANCED,
/* Down		*/	DISP_NR_ADVANCED,
/* Up		*/	DISP_NR_ADVANCED,
/* Enter	*/	DISP_NR_ADVANCED
	},
// 17 SERVICE - DMX VALUES
	{
/* Timeout  */	TOT(120000), // 20 sekund
/* Edit Var */	(int*)&GroupOf12ChannelsIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_INCREMENT_VARIABLE | DTP_ALWAYS_STORE_VARIABLE,
/* Escape	*/	DISP_NR_SERVICE_MENU,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	3
	},
// 18 SERVICE - ABOUT
	{
/* Timeout  */	TOT(120000), // 20 sekund
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_SERVICE_MENU,
/* Down		*/	DISP_NR_SERVICE_ABOUT_2_DIMMERS,
/* Up		*/	DISP_NR_SERVICE_ABOUT_2_DIMMERS,
/* Enter	*/	DISP_NR_SERVICE_ABOUT_2_DIMMERS
	},
// 19 SERVICE - PLUGIN CRMX - NEPOUZITO!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
/* Timeout  */	TOT(120000), // 20 sekund
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_SERVICE_MENU,
/* Down		*/	DISP_NR_POPUP_UNLINK_CRMX_CONFIRM,
/* Up		*/	DISP_NR_SERVICE_MENU,
/* Enter	*/	DISP_NR_SERVICE_PLUGIN_ETHERNET
	},
// 20 DMX SOURCE STATUS 2/3 - CRMX - NEPOUZITO!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	DISP_NR_DMX_INP_STATUS_XLR,
/* Up		*/	DISP_NR_DMX_INP_STATUS_ETHERNET,
/* Enter	*/	DISP_NR_DMX_INP_STATUS_ETHERNET
	},
// 21 DMX SOURCE STATUS 2/2 - ETH
	{
/* Timeout  */	TOT(600000), // 10 minut
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_MAIN_MENU,
/* Down		*/	DISP_NR_DMX_INP_STATUS_XLR,
/* Up		*/	DISP_NR_DMX_INP_STATUS_XLR,
/* Enter	*/	DISP_NR_DMX_INP_STATUS_XLR
	},
// 22 SACN UNIVERSE
	{
/* Timeout  */	TOT(60000), // 60 sekund
/* Edit Var */	&SAcnUniverseItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DYNAMIC_DISP | DTP_ALWAYS_STORE_VARIABLE | BTN_UP_DN_REVERSE_FUNC,
/* Escape	*/	DISP_NR_INPUT_CONTROL,
/* Down		*/	0,
/* Up		*/	2,
/* Enter	*/	DISP_NR_POPUP_SACN_UNIV100S	// vysledny display je 23 + ItemIndex
	},
// 23 popup display SACN UNIVERSE NUMBER 100S		----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&Eth_Disp_sACN_Universe,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARBYVAL | BTN_UP_CHANGE_VARBYVAL | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_SACN_UNIVERSE,
/* Down		*/	1,
/* Up		*/	63999,
/* Enter	*/	DISP_NR_SACN_UNIVERSE,
/* IncVal	*/	100
	},
// 24 popup display SACN UNIVERSE NUMBER 1S			----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&Eth_Disp_sACN_Universe,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_SACN_UNIVERSE,
/* Down		*/	1,
/* Up		*/	63999,
/* Enter	*/	DISP_NR_SACN_UNIVERSE
	},
// 25 popup display SACN UNIVERSE NUMBER			----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&Eth_Disp_sACN_Universe,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_SACN_UNIVERSE,
/* Down		*/	1,
/* Up		*/	63999,
/* Enter	*/	DISP_NR_SACN_UNIVERSE
	},
// 26 ARTNET UNIVERSE
	{
/* Timeout  */	TOT(60000), // 60 sekund
/* Edit Var */	&ArtNetUnivItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DYNAMIC_DISP | DTP_ALWAYS_STORE_VARIABLE | BTN_UP_DN_REVERSE_FUNC,
/* Escape	*/	DISP_NR_INPUT_CONTROL,
/* Down		*/	0,
/* Up		*/	2,
/* Enter	*/	DISP_NR_POPUP_ARTNET_NET	// vysledny display je 27 + ItemIndex
	},
// 27 popup display ARTNET UNIVERSE NUMBER - NET	----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&Eth_Disp_ArtNet_Universe,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARBYVAL | BTN_UP_CHANGE_VARBYVAL | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_ARTNET_NSNU,
/* Down		*/	0,
/* Up		*/	32767,
/* Enter	*/	DISP_NR_ARTNET_NSNU,
/* IncVal	*/	0x100
	},
// 28 popup display ARTNET UNIVERSE NUMBER - SUBNET	----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&Eth_Disp_ArtNet_Universe,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARBYVAL | BTN_UP_CHANGE_VARBYVAL | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_ARTNET_NSNU,
/* Down		*/	0,
/* Up		*/	32767,
/* Enter	*/	DISP_NR_ARTNET_NSNU,
/* IncVal	*/	0x010
	},
// 29 popup display ARTNET UNIVERSE NUMBER - UNIVERSE	----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int *)&Eth_Disp_ArtNet_Universe,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_ARTNET_NSNU,
/* Down		*/	0,
/* Up		*/	32767,
/* Enter	*/	DISP_NR_ARTNET_NSNU,
	},
// 30 SERVICE - PLUGIN ETHERNET (2/2)
	{
/* Timeout  */	TOT(120000), // 20 sekund
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_SERVICE_MENU,
/* Down		*/	DISP_NR_SERVICE_MENU,
/* Up		*/	DISP_NR_IP_ADDRESS_CHANGING,
/* Enter	*/	DISP_NR_SERVICE_MENU
	},
// 31 IP ADDRESS CHANGING
	{
/* Timeout  */	TOT(0),
/* Edit Var */	&EthIpChangingMenuIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DYNAMIC_DISP | DTP_ALWAYS_STORE_VARIABLE | BTN_UP_DN_REVERSE_FUNC,
/* Escape	*/	DISP_NR_SERVICE_PLUGIN_ETHERNET,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	DISP_NR_IP_ADDRESS_A_CHANGING,
	},
// 32 popup display CHANGE IP ADDRESS A		----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&Eth_DisplayIpAddress[0],
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_IP_ADDRESS_CHANGING,
/* Down		*/	0,
/* Up		*/	255,
/* Enter	*/	DISP_NR_IP_ADDRESS_CHANGING
	},
// 33 popup display CHANGE IP ADDRESS B		----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&Eth_DisplayIpAddress[1],
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_IP_ADDRESS_CHANGING,
/* Down		*/	0,
/* Up		*/	255,
/* Enter	*/	DISP_NR_IP_ADDRESS_CHANGING
	},
// 34 popup display CHANGE IP ADDRESS C		----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&Eth_DisplayIpAddress[2],
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_IP_ADDRESS_CHANGING,
/* Down		*/	0,
/* Up		*/	255,
/* Enter	*/	DISP_NR_IP_ADDRESS_CHANGING
	},
// 35 popup display CHANGE IP ADDRESS D		----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&Eth_DisplayIpAddress[3],
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_IP_ADDRESS_CHANGING,
/* Down		*/	0,
/* Up		*/	255,
/* Enter	*/	DISP_NR_IP_ADDRESS_CHANGING
	},
// 36 MANUAL CONTROL CHANNELL
	{
/* Timeout  */	TOT(200000), // 20 sekund
/* Edit Var */	(int*)&GroupOf12ChannelsIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DISPLAY | DTP_ALWAYS_STORE_VARIABLE,
/* Escape	*/	DISP_NR_SERVICE_MENU,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	DISP_NR_MANUAL_CONTROL_CH
	},
// 37 popup display CONFIRM FACTORY SETTINGS
	{
/* Timeout  */	TOT(5000),
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_WRITE_FACTORY_SETTS | BTN_ENT_CHANGE_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_UNIT_SETTINGS,
/* Down		*/	DISP_NR_UNIT_SETTINGS,
/* Up		*/	0,
/* Enter	*/	DISP_NR_UNIT_SETTINGS,
	},
// 38 popup display CONFIRM CRMX UNLINK - NOT USED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
/* Timeout  */	TOT(20000),
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_SET_BIT_VARIABLE | BTN_ENT_CHANGE_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_SERVICE_PLUGIN_CRMX,
/* Down		*/	DISP_NR_SERVICE_PLUGIN_CRMX,
/* Up		*/	0, //CRMX_STS_UNLINK_DEVICE,
/* Enter	*/	DISP_NR_SERVICE_PLUGIN_CRMX,
	},
// 39 ADVANCED
	{
/* Timeout  */	TOT(20000),
/* Edit Var */	&AdvancedMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_MENU_REFERENCE_ADR | BTN_UP_DN_REVERSE_FUNC | DTP_SAVE_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_SERVICE_MENU,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	.EnterAction=&ButtonEnterAction[23] // Ukazatel na prvni akci pri EditableItemValue=0.
	},
// 40 SET DIGITAL INPUTS
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	&MenuUpDown_EditedValue,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_SELECTED_CHAN | DTP_ACTION_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_ADVANCED,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	0
	},
// 41 DIA LEVELS
	{
/* Timeout  */	TOT(20000),
/* Edit Var */	&DiaLevelsItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DYNAMIC_DISP | DTP_ALWAYS_STORE_VARIABLE | BTN_UP_DN_REVERSE_FUNC,
/* Escape	*/	DISP_NR_ADVANCED,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	DISP_NR_FADE_TIME		// vysledny displej je DISP_NR_FADE_TIME+DiaLevelsItemIndex
	},
// 42 popup display FADE TIME					----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&UnitSettings.AuditoriumFadeTime,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_DIA_LEVELS,
/* Down		*/	1,
/* Up		*/	120,
/* Enter	*/	DISP_NR_DIA_LEVELS
	},
// 43 popup display DIA LEVEL 1					----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&UnitSettings.DiaLevel[0],
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_DIA_LEVELS,
/* Down		*/	1,
/* Up		*/	99,
/* Enter	*/	DISP_NR_DIA_LEVELS
	},
// 44 popup display DIA LEVEL 2					----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&UnitSettings.DiaLevel[1],
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_DIA_LEVELS,
/* Down		*/	1,
/* Up		*/	99,
/* Enter	*/	DISP_NR_DIA_LEVELS
	},
// 45 popup display DIA LEVEL 3					----- POP-UP -----
	{
/* Timeout  */	TOT(20000), // 20 sekund
/* Edit Var */	(int*)&UnitSettings.DiaLevel[2],
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_SAVE_AND_DISPLAY | DTP_NO_CLEAR_BKGD,
/* Escape	*/	DISP_NR_DIA_LEVELS,
/* Down		*/	1,
/* Up		*/	99,
/* Enter	*/	DISP_NR_DIA_LEVELS
	},
// 46 SERVICE ABOUT 2 DIMMERS
	{
/* Timeout  */	TOT(120000), // 120 sekund
/* Edit Var */	(int*)&GroupOf12ChannelsIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DISPLAY | DTP_ALWAYS_STORE_VARIABLE,
/* Escape	*/	DISP_NR_SERVICE_MENU,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	DISP_NR_SERVICE_ABOUT
	},
// 47 ONE ADVANCED DIMMER SETUP MENU
	{
/* Timeout  */	TOT(30000), // 30 sekund
/* Edit Var */	&OneAdvDimmerSetupMenuItemIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_MENU_REFERENCE_ADR | BTN_UP_DN_REVERSE_FUNC | DTP_SAVE_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_1_DIMMER_SETUP,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	.EnterAction=&ButtonEnterAction[27] // Ukazatel na prvni akci pri EditableItemValue=0.
	},
// 48 INFO 2 DIMMERS
	{
/* Timeout  */	TOT(600000), // 120 sekund
/* Edit Var */	(int*)&GroupOf12ChannelsIndex,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_DISPLAY | DTP_ALWAYS_STORE_VARIABLE,
/* Escape	*/	DISP_NR_MAIN_SCREEN,
/* Down		*/	0,
/* Up		*/	3,
/* Enter	*/	DISP_NR_INFO_1
	},
// 49 INFO 3 DIMMERS / Not used so far
	{
/* Timeout  */	TOT(600000), // 120 sekund
/* Edit Var */	0,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_DISPLAY | BTN_UP_CHANGE_DISPLAY | BTN_ENT_CHANGE_DISPLAY,
/* Escape	*/	DISP_NR_MAIN_SCREEN,
/* Down		*/	DISP_NR_INFO_2_DIMMERS,
/* Up		*/	DISP_NR_INFO_1,
/* Enter	*/	DISP_NR_INFO_1
	},
// 50 MANUAL CONTROL CHANNELL
	{
/* Timeout  */	TOT(200000), // 20 sekund
/* Edit Var */	(int*)&MenuUpDown_EditedValue,
/* Type     */	BTN_ESC_CHANGE_DISPLAY | BTN_DN_CHANGE_VARIABLE | BTN_UP_CHANGE_VARIABLE | BTN_ENT_CHANGE_SELECTED_CHAN | DTP_ACTION_ON_CHANGE_VARIABLE,
/* Escape	*/	DISP_NR_MANUAL_CONTROL_GRP,
/* Down		*/	0,
/* Up		*/	100,
/* Enter	*/	0
	},
};
#pragma GCC diagnostic pop


