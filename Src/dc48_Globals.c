/*
 * dc48_Globals.c
 *
 *  Created on: Jan 29, 2026
 *      Author: harma
 */

#include "dc48_Globals.h"

unsigned int				CircularCounter100us;
int							DmxDisplay[NUM_OF_DISP_CHANNS];	// DMX displayed values on Main Screen
int							CpuTemperature;					// MCU Temperature
int							ExtTemperature;					// External Temperature
unsigned int				NumberOfStarts;
unsigned int				LED_BlueCtc;					// Counter for blinking with blue debug LED
tSerialDimmer				SerialDimmer[NUM_OF_CHANNELS];		// Serial Dimmers data
int							LineVoltage[3];						// Line voltage from dimmers
int							LineFrequency[3];					// Line frequency from dimmers
int							DisplayMcuTemperature;				// Temperature MCU
int							DisplayExtTemperature;				// Temperature External
unsigned int				Display_fan_output;
unsigned int				Display_fan_tacho;
int							DisplayFanTemperature;				// Temperature Mcu or Ext for fan ctrl
int							SavingToFlashActive;
/* ETHERNET *********************************************************************************/
//unsigned int				Eth_IpAddressToSave;				// Changed IP address by user via display
/* CHANNELS *********************************************************************************/
float						DimmerInput[NUM_OF_CHANNELS];		// All input sources
float						DimmerOutput[NUM_OF_CHANNELS];		// Output values after curves
unsigned int				RDM_IdentifiingDevice[NUM_OF_CHANNELS];
/* DMX **************************************************************************************/
unsigned int				DMX_to_XLR_Active;
unsigned char				DMX_XLR_RecBuffer[REC_DMX_BUFFER_SIZE+3];
unsigned char				DMX_ETH_RecBuffer[REC_DMX_BUFFER_SIZE+3];
unsigned int				DMX_Is_Present;
unsigned int				DMX_XLR_Value[NUM_OF_CHANNELS];				// Raw Value from DMX XLR
unsigned int				DMX_ETH_Value[NUM_OF_CHANNELS];		// Raw Value from DMX XLR
int							DMX_XLR_Data_Process;				// To know to process received DMX data
int							DMX_ETH_Data_Process;				// To know to process received DMX data
int							DMX_XLR_NumOfRecChans;				// Number of received characters via XLR
unsigned int				DMX_HTP_Value[NUM_OF_CHANNELS];		// HTP Value processed on XLR and ETH
/* RDM **************************************************************************************/
int							DmxRS485_RDM_DataToProcess;
int 						DmxRS485_RDM_DiscoveryResponse;		// Number of bytes to send as discovery response
/* Display **********************************************************************************/
int							Actual_Screen_Number;				// Screen number
int							EditableItemValue;					// Editable value
int							XLR_Display_Status;					// NOTOK, OK, OUTPUT
int							Display_Worklight_Manual_Mode;		// For Main Display Mode indication
int							MainMenuItemIndex;					// main menu item index
char						DispDmxStartAddress[4];				// Text Start Address or Question Mark
char *						FlashWritingProgress;				// sign "FL" or nothing
int							PatchMenuItemIndex;					// patch menu item index
int							DimmerSetupDimmerIndex;				// dimmer setup dimmer index
TDimmerSetup				DispDimmSetup;						// Nastaveni jednoho dimmeru pro display
unsigned char				DispDimmSetupWZIR[4];				// Dimmer setup for Worklight; ZeroIsZero; IGBT Style; RelayUse;
unsigned int				DispDimmSetWZIR[4];					// Temporary Dimmer setup for Worklight; ZeroIsZero; IGBT Style; RelayUse;
int							UnitSettingsMenuItemIndex;			// unit settings item index
int							InputCtrlMenuItemIndex;				// input ctrl item index
int							ServiceMenuItemIndex;				// service item index
unsigned int				DmxStartAddress;					// Start address filled at entering menu PATCH
int							MenuUpDown_EditedValue;				// individual patch menu = channel
unsigned int				MenuEnterButton_SelectedChannel;	// Selected channell [1..12] at individual patch
char *						FlashWritingProgress;				// sign "FL" or nothing
unsigned int				GroupOf12ChannelsIndex;			// Group 0=1-12, 1=13-24, 2=25-36, 3=37-48
unsigned int				DispPatch[NUM_OF_DISP_CHANNS];		// Patch for display. There is possible to edit only 12 channels
int							OneDimmerSetupMenuItemIndex;		// prechod mezi polozkami nastaveni dimeru
int							OneAdvDimmSetupMenuItemIndex;		// prechod mezi polozkami nastaveni dimeru
int							DispDmxValue[NUM_OF_DISP_CHANNS];	// Pouzito v SERVICE / DMX VALUES pro zobrazeni Raw DMX hodnot
int							ArtNetUnivItemIndex;				// ArtNet universe item index menu
int							SAcnUniverseItemIndex;				// sACN universe item index in menu
int							EthIpChangingMenuIndex;				// Ethernet IP address changing menu item index
int							AdvancedMenuItemIndex;				// Advanced Menu Items Index
int							DiaLevelsItemIndex;					// Auditorium Dia Levels menu item index
tSerialDimmer				DispSerialDimm[NUM_OF_DISP_CHANNS];	// Serial Dimmers displayed data
int							OneAdvDimmerSetupMenuItemIndex;		// Advanced Dimmer Setup WZIR item index
char const *				DispEthernetActiveTxt;				// toto prijde presunout do Ethernet unit
/* FLASH --> RAM ***************************************************************************/
unsigned int				Patch[NUM_OF_CHANNELS];				// Individual Patch
TUnitSettings				UnitSettings;						// RAM Unit Settings
unsigned int				WorkHours10min;
TDimmerSetup				DimmerSetup[NUM_OF_CHANNELS];
/* FLASH ***********************************************************************************/
__FLASH_SET_PATCH
const unsigned int			FLASH_Patch[NUM_OF_CHANNELS] =		// Individual Patch
							{100, 2, 3, 40, 15, 16, 77, 158, 9, 10, 11, 12, 13, 512, 511, 16, 17, 18, 19, 20, 21, 22, 23, 10,
							25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48};
__FLASH_SET_UNIT
const TUnitSettings			FLASH_UnitSettings = {				// FLASH Unit Settings
		/* WorklightEn */		1,
		/* HoldLast */			1,
		/* DisplayOffTime */	10,
		/* ManualControl */		0,
		/* EthernetToXLR */		0,
		/* ManualChannels */	{0},
		/* DiaLevel */			{50},
		/* sACN_Enabled */		0,
		/* ArtNet_Enabled */	1,
};
const unsigned int			FLASH_WorkHours10min;
__FLASH_SET_DIMMERS
const TDimmerSetup			FLASH_DimmerSetup[NUM_OF_CHANNELS] = {
		DIMM_DEFAULT_SETUP,
		{DIMM_CURVE_OFF, 10, 800, 1, 2, 1, 0},
		DIMM_DEFAULT_SETUP,
		{DIMM_CURVE_ALW_ON, 10, 800, 1, 1, 0, 0},
		{DIMM_CURVE_NODIMM, 15, 821, 1, 1, 0, 0},
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
		DIMM_DEFAULT_SETUP,
};

// FLASH_PAGE --> FLASH_WHRS
__attribute__((section(".flash_Whrs")))	// may be sometime in future
const unsigned int 				FLASH_WorkHours10min;

// FLASH - UPDATE HISTORY
__attribute__((section(".flash_History")))	// may be sometime in future
const unsigned int				FLASH_Update_History[16] = {FIRMWARE_DATE, FIRMWARE_VERSION_HEX};

// FLASH Write Buffer - common for normal FW and bootloader
__attribute__((section(".ram_flash_write_buf")))
unsigned int					FlashWrBuf[0x2000>>2];	// 8 kB / 4 = 2 kB of int


