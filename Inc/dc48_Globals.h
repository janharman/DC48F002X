/*
 * dc48_Globals.h
 *
 *  Created on: Jan 29, 2026
 *      Author: harma
 */

#ifndef INC_DC48_GLOBALS_H_
#define INC_DC48_GLOBALS_H_

#define FIRMWARE_VERSION_HEX			0x0001
#define FIRMWARE_VERSION				"F001"
//#define APPLICATION_FW_VERSION			FIRMWARE_VERSION
#define FIRMWARE_DATE					0x260310
#define FIRMWARE_DATE_TXT				"260310"

#define CONFIGURATION_STRING			"CD02T"

#define HARDWARE_VERSION_H00A

//---------------------------------------------------
#ifdef HARDWARE_VERSION_H00A
#define HARDWARE_VERSION				"H00A"
#define HARDWARE_VERSION_HEX			0x0000
#endif

#define CORE_FREQUENCY					200000000
#define SYS_TICK_FREQUENCY				10000
#define TASK_HANDLE_FREQUENCY(hz)		(SYS_TICK_FREQUENCY/hz)

// Debug LEDs
#define LED_DEBUG_BLUE_ON				(GPIOB->BSRR = GPIO_BSRR_BR4)
#define LED_DEBUG_BLUE_OFF				(GPIOB->BSRR = GPIO_BSRR_BS4)
#define LED_DEBUG_BLUE_TGL
//#define LED_DEBUG_BLUE_TGL			(GPIOB->ODR ^= GPIO_ODR_OD4)
#define LED_DEBUG_RED_ON				(GPIOB->BSRR = GPIO_BSRR_BR5)
#define LED_DEBUG_RED_OFF				(GPIOB->BSRR = GPIO_BSRR_BS5)
//#define LED_DEBUG_RED_TGL				(GPIOB->ODR ^= GPIO_ODR_OD5)

#define OLED_E_1						GPIOC->BSRR = GPIO_BSRR_BS3
#define OLED_E_0						GPIOC->BSRR = GPIO_BSRR_BR3
#define OLED_RW_1						GPIOC->BSRR = GPIO_BSRR_BS2
#define OLED_RW_0						GPIOC->BSRR = GPIO_BSRR_BR2
#define OLED_D_1						GPIOC->BSRR = GPIO_BSRR_BS1
#define OLED_D_0						GPIOC->BSRR = GPIO_BSRR_BR1
#define OLED_RES_1						GPIOC->BSRR = GPIO_BSRR_BS0
#define OLED_RES_0						GPIOC->BSRR = GPIO_BSRR_BR0
#define OLED_CS_1						GPIOH->BSRR = GPIO_BSRR_BS1
#define OLED_CS_0						GPIOH->BSRR = GPIO_BSRR_BR1

#define LED_DMX_ON						(GPIOC->BSRR = GPIO_BSRR_BR4)
#define LED_DMX_OFF						(GPIOC->BSRR = GPIO_BSRR_BS4)
//#define LED_DMX_TGL					(GPIOA->ODR ^= GPIO_ODR_6)
#define LED_ERR_ON						(GPIOC->BSRR = GPIO_BSRR_BR9)
#define LED_ERR_OFF						(GPIOC->BSRR = GPIO_BSRR_BS9)
#define DMX_XLR_DIRECTION_IS_TX			(GPIOA->ODR & GPIO_ODR_OD8)
#define DMX_XLR_SET_DIR_TO_RECEIVE		(GPIOA->BSRR = GPIO_BSRR_BR8)
#define DMX_XLR_SET_DIR_TO_TRANSMIT		(GPIOA->BSRR = GPIO_BSRR_BS8)
#define DMX_XLR_SET_TRANSMIT_TX_LINE_1	(GPIOA->BSRR = GPIO_BSRR_BS9)
#define DMX_XLR_SET_TRANSMIT_TX_LINE_0	(GPIOA->BSRR = GPIO_BSRR_BR9)
#define FAN_ROTATION_IN					(GPIOB->IDR & GPIO_IDR_ID3)

#define REC_DMX_BUFFER_SIZE				513
#define MAX_SETUPABLE_DMX_ADDRESS		511
#define NUM_OF_DISP_CHANNS				12
#define NUM_OF_CHANNELS					48

#define DMX_PR_XLR				1
#define DMX_PR_ETH				2
#define DMX_PR_RDM				4
#define DMX_PR_FROM_STRT 		0x80
#define DMX_PR_ANY				(DMX_PR_XLR | DMX_PR_ETH)

#define WRITE_TO_FLASH__PATCH_DIMMERS		0x08040000
#define WRITE_TO_FLASH__UNIT_SETTINGS		0x08042000
#define WRITE_TO_FLASH__WHRS_SETTINGS		0x08044000
#define WRITE_TO_FLASH__DIMMERS_SETTS		0x08046000
#define WRITE_TO_FLASH__RDM_SETTINGS		0x08048000

#define __FLASH_PRG_ADR			__attribute__ ((section(".flash_PrgAdr")))
#define __FLASH_PRG_JUMP		__attribute__ ((section(".flash_jump_to_prg")))
#define __FLASH_PRG_CRC			__attribute__ ((section(".flash_PrgCrc")))
#define __FLASH_SET_PATCH		__attribute__ ((section(".flash_Patch")))
#define __FLASH_SET_UNIT		__attribute__ ((section(".flash_Unit")))
#define __FLASH_WORKHOURS		__attribute__ ((section(".flash_Whrs")))
#define __FLASH_SET_DIMMERS		__attribute__ ((section(".flash_Dimmers")))

#define XLR_MODE_NO_DMX_SIGNAL		0
#define XLR_MODE_INPUT_SIGNAL		1
#define XLR_MODE_OUTPUT_SIGNAL		2

typedef struct
{
	unsigned int	WorklightEn;
	unsigned int	HoldLast;
	unsigned int	DisplayOffTime;
	unsigned int	ManualControl;
	unsigned int	EthernetToXLR;
	unsigned int	ManualChannels[NUM_OF_CHANNELS];
	unsigned int	DiaLevel[3];
	unsigned int	AuditoriumDisabledIfDmx;
	unsigned int	AuditoriumFadeTime;
}
	TUnitSettings;

typedef struct
{
	unsigned int			XLR;
	unsigned int			Ethernet;
	unsigned int			Wifi;
	unsigned int			BarChInOut;
	unsigned int			AfterHTP;
	unsigned short *		pFTM_CV;
	float					Input;
	float					Output;
	float					OutputFinish;
}
	TDmxValues;

#define DIMMER_OPT__TRIAC_STYLE			1
#define DIMMER_OPT__RELAY_USING			2

typedef struct
{
	unsigned int Curve;
	unsigned int Minimum;
	unsigned int Maximum;
	unsigned int Response;
	unsigned int WorkL_ZiZ;
	unsigned int Ts_Ru;				// Triac Style; Relay Using;
	unsigned int SvetloDiaTma;
	unsigned char * ResponseText;
}
	TDimmerSetup;

#define DIMSET_WZ__WORKLIGHT_ON				1
#define DIMSET_WZ__ZERO_IS_ZERO				2
#define DIMSET_TR__TRIAC_STYLE				1
#define DIMSET_TR__RELAY_USING				2

#define	DIMM_CURVE_OFF			0
#define DIMM_CURVE_LINEAR		1
#define DIMM_CURVE_SQUARE		2
#define DIMM_CURVE_SCURVE		3
#define DIMM_CURVE_NODIMM		4
#define DIMM_CURVE_ALW_ON		5
#define DIMM_CURVES_COUNT		6

#define DIMM_RESPONSE_FAST		0
#define DIMM_RESPONSE_NORMAL	1
#define DIMM_RESPONSE_SLOW		2
#define DIMM_RESPONSE_VERY_SLOW	3

#define DIMM_DEFAULT_SETUP {DIMM_CURVE_LINEAR, 0, 1000, 1, 0, 0, 0}

typedef struct
{
	unsigned int			HwFwSt;
	unsigned int			Live;
	unsigned int			Runtime;	// Fuse+Bulb
	unsigned int			LineVoltage;
	unsigned int			LineFrequency;
	float					FreqMulFactor;
}	tSerialDimmer;

// SERIAL DIMMER STATUS BITS
#define SDS_LINE_FUSE_OK		1
#define SDS_BALLAST_PRESENT		2
#define SDS_STATUS_WORD_OK		0x80000000

extern unsigned int				CircularCounter100us;
extern int						DmxDisplay[NUM_OF_DISP_CHANNS];	// DMX displayed values on Main Screen
extern int						CpuTemperature;					// MCU Temperature
extern int						ExtTemperature;					// External Temperature
extern unsigned int				NumberOfStarts;
extern unsigned int				LED_BlueCtc;					// Counter for blinking with blue debug LED
extern tSerialDimmer			SerialDimmer[NUM_OF_CHANNELS];		// Serial Dimmers data
extern int						LineVoltage[3];						// Line voltage from dimmers
extern int						LineFrequency[3];					// Line frequency from dimmers
extern int						DisplayMcuTemperature;				// Temperature MCU
extern int						DisplayExtTemperature;				// Temperature External
extern unsigned int				Display_fan_output;
extern unsigned int				Display_fan_tacho;
extern int						DisplayFanTemperature;				// Temperature Mcu or Ext for fan ctrl
extern int						SavingToFlashActive;
/* ETHERNET *********************************************************************************/
//extern unsigned int				Eth_IpAddressToSave;				// Changed IP address by user via display
/* CHANNELS *********************************************************************************/
extern float					DimmerInput[NUM_OF_CHANNELS];		// All input sources
extern float					DimmerOutput[NUM_OF_CHANNELS];		// Output values after curves
extern unsigned int				RDM_IdentifiingDevice[NUM_OF_CHANNELS];
/* DMX **************************************************************************************/
extern unsigned int				DMX_to_XLR_Active;
extern unsigned char			DMX_XLR_RecBuffer[REC_DMX_BUFFER_SIZE+3];
extern unsigned char			DMX_ETH_RecBuffer[REC_DMX_BUFFER_SIZE+3];
extern unsigned int				DMX_Is_Present;
extern unsigned int				DMX_XLR_Value[NUM_OF_CHANNELS];		// Raw Value from DMX XLR
extern unsigned int				DMX_ETH_Value[NUM_OF_CHANNELS];		// Raw Value from DMX XLR
extern int						DMX_XLR_Data_Process;				// To know to process received DMX data
extern int						DMX_ETH_Data_Process;				// To know to process received DMX data
extern int						DMX_XLR_NumOfRecChans;				// Number of received characters via XLR
extern unsigned int				DMX_HTP_Value[NUM_OF_CHANNELS];		// HTP Value processed on XLR and ETH
/* RDM **************************************************************************************/
extern int						DmxRS485_RDM_DataToProcess;
extern int 						DmxRS485_RDM_DiscoveryResponse;		// Number of bytes to send as discovery response
/* Display **********************************************************************************/
extern int						Actual_Screen_Number;				// Screen number
extern int						EditableItemValue;					// Editable value
extern int						XLR_Display_Status;					// NOTOK, OK, OUTPUT
extern int						Display_Worklight_Manual_Mode;		// For Main Display Mode indication
extern int						MainMenuItemIndex;					// main menu item index
extern char						DispDmxStartAddress[4];				// Text Start Address or Question Mark
extern char *					FlashWritingProgress;				// sign "FL" or nothing
extern int						PatchMenuItemIndex;					// patch menu item index
extern int						DimmerSetupDimmerIndex;				// dimmer setup dimmer index
extern TDimmerSetup				DispDimmSetup;						// Nastaveni jednoho dimmeru pro display
extern unsigned char			DispDimmSetupWZIR[4];				// Dimmer setup for Worklight; ZeroIsZero; IGBT Style; RelayUse;
extern unsigned int				DispDimmSetWZIR[4];					// Temporary Dimmer setup for Worklight; ZeroIsZero; IGBT Style; RelayUse;
extern int						UnitSettingsMenuItemIndex;			// unit settings item index
extern int						InputCtrlMenuItemIndex;				// input ctrl item index
extern int						ServiceMenuItemIndex;				// service item index
extern unsigned int				DmxStartAddress;					// Start address filled at entering menu PATCH
extern int						MenuUpDown_EditedValue;				// individual patch menu = channel
extern unsigned int				MenuEnterButton_SelectedChannel;	// Selected channell [1..12] at individual patch
extern char *					FlashWritingProgress;				// sign "FL" or nothing
extern unsigned int				GroupOf12ChannelsIndex;				// Group 0=1-12, 1=13-24, 2=25-36, 3=37-48
extern unsigned int				DispPatch[NUM_OF_DISP_CHANNS];		// Patch for display. There is possible to edit only 12 channels
extern int						OneDimmerSetupMenuItemIndex;		// prechod mezi polozkami nastaveni dimeru
extern int						OneAdvDimmSetupMenuItemIndex;		// prechod mezi polozkami nastaveni dimeru
extern int						DispDmxValue[NUM_OF_DISP_CHANNS];	// Pouzito v SERVICE / DMX VALUES pro zobrazeni Raw DMX hodnot
extern int						ArtNetUnivItemIndex;				// ArtNet universe item index menu
extern int						SAcnUniverseItemIndex;				// sACN universe item index in menu
extern int						EthIpChangingMenuIndex;				// Ethernet IP address changing menu item index
extern int						AdvancedMenuItemIndex;				// Advanced Menu Items Index
extern int						DiaLevelsItemIndex;					// Auditorium Dia Levels menu item index
extern tSerialDimmer			DispSerialDimm[NUM_OF_DISP_CHANNS];	// Serial Dimmers displayed data
extern int						OneAdvDimmerSetupMenuItemIndex;		// Advanced Dimmer Setup WZIR item index
extern char const *				DispEthernetActiveTxt;				// toto prijde presunout do Ethernet unit
/* FLASH --> RAM ***************************************************************************/
extern unsigned int				Patch[NUM_OF_CHANNELS];				// Individual Patch
extern TUnitSettings			UnitSettings;						// RAM Unit Settings
extern unsigned int				WorkHours10min;
extern TDimmerSetup				DimmerSetup[NUM_OF_CHANNELS];
/* FLASH ***********************************************************************************/
extern const TDimmerSetup		DefaultDimmerSetup;
extern const unsigned int		FLASH_Patch[NUM_OF_CHANNELS];		// Individual Patch
extern const TUnitSettings		FLASH_UnitSettings;					// FLASH Unit Settings
extern const unsigned int		FLASH_WorkHours10min;
extern const TDimmerSetup		FLASH_DimmerSetup[NUM_OF_CHANNELS];

// FLASH - UPDATE HISTORY
extern const unsigned int		FLASH_Update_History[16];

// FLASH Write Buffer - common for normal FW and bootloader
extern unsigned int				FlashWrBuf[0x2000>>2];


#endif /* INC_DC48_GLOBALS_H_ */


