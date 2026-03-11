/*
 * dc4_RdmDef.c
 *
 *  Created on: Apr 9, 2024
 *      Author: harma
 */

#include "rdm.h"
#include "dc48_Globals.h"
#include "dc48_RdmDef.h"

tRdmSetup					RdmSetup;					// RDM setup parameters rewritable by user
TRMD_Packet 				RDM_Packet;				// Data pro vysilani RDM
const unsigned int			cThisDeviceManufUID = THIS_DEVICE_MANUFACTURER_UID;

const tDmxPersonality		DmxSubDevPersonality[DMX_NUBER_OF_SUBDEV_MODES] =
{
		// Text			ChNr	SlotInfo.LabelID		SlotInfo.Name
		{"Always OFF",	0,	{{SD_INTENSITY_MASTER,	"---"}}},
		{"Linear",		1,	{{SD_INTENSITY_MASTER,	"Intensity Ch: "}}},
		{"Square",		1,	{{SD_INTENSITY_MASTER,	"Intensity Ch: "}}},
		{"S-Curve",		1,	{{SD_INTENSITY_MASTER,	"Intensity Ch: "}}},
		{"No-Dimm",		1,	{{SD_INTENSITY_MASTER,	"Intensity Ch: "}}},
		{"Always ON",	0,	{{SD_INTENSITY_MASTER,	"---"}}},
};

const char SensorDefinition[RDM_SENSOR_COUNT][30]=
{
		{0,		0,		1,		1,		0, 0,	3, 0xE8,		0, 0,		3, 0x52,		0, 	'C', 'h', 'i', 'p', ' ', 'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e'},
		{0,		0,		1,		1,		0, 0,	3, 0xE8,		0, 0,		3, 0x52,		0, 	'E', 'x', 't', '.', ' ', 'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e'},
};

const short SupportedParameterList[NUM_OF_ALL_SUPPORTED_IDS] =
{
		SWS__(E120_QUEUED_MESSAGE),					// 0020
		SWS__(E120_STATUS_MESSAGES),				// 0030
		SWS__(E120_STATUS_ID_DESCRIPTION),			// 0031
		SWS__(E120_DEVICE_MODEL_DESCRIPTION),		// 0080
		SWS__(E120_MANUFACTURER_LABEL),				// 0081
		SWS__(E120_DEVICE_LABEL),					// 0082
		SWS__(E120_DMX_PERSONALITY_DESCRIPTION),	// 00E1
		SWS__(E120_DMX_PERSONALITY),				// 00E0
		SWS__(E120_SLOT_INFO),						// 0120
		SWS__(E120_SLOT_DESCRIPTION),				// 0121
		SWS__(E120_SENSOR_DEFINITION),				// 0200
		SWS__(E120_SENSOR_VALUE),					// 0201
		SWS__(E120_DEVICE_HOURS),					// 0400
		SWS__(E120_DEVICE_POWER_CYCLES),			// 0405
		/* Manufacturer specific PIDs */
		SWS__(ALP_PID_WORKLIGHT_ENABLED),			// 8000
		SWS__(ALP_PID_HOLD_LAST_DMX_FAIL),			// 8100
		SWS__(ALP_PID_ETHERNET_SACN_ENABLED),		// 8120
		SWS__(ALP_PID_ETHERNET_SACN_UNIVERSE),		// 8121
		SWS__(ALP_PID_ETHERNET_ARTNET_ENABLED),		// 8130
		SWS__(ALP_PID_ETHERNET_ARTNET_UNIVERSE),	// 8131
};

const TParameterDescription ParamDescriptor[NUM_OF_MANUFACTURER_SPECIFIC_IDS] =
{
		/* 				PID							PDL 		Data Type		Command Class		Type					Unit				Prefix	Min Value	Max Value	Default V.	Description			*/
		{SWS__(ALP_PID_WORKLIGHT_ENABLED),			1,	E120_DS_UNSIGNED_BYTE,	E120_CC_GET_SET,	0x80,					0,					0,		0x00000000,	0x01000000,	0x00000000,	"Worklight Enabled"				},
		{SWS__(ALP_PID_HOLD_LAST_DMX_FAIL),			1,	E120_DS_UNSIGNED_BYTE,	E120_CC_GET_SET,	0x80,					0,					0,		0x00000000,	0x01000000,	0x01000000,	"Hold Last after DMX Fail"		},
		{SWS__(ALP_PID_ETHERNET_SACN_ENABLED),		1,	E120_DS_UNSIGNED_BYTE,	E120_CC_GET,		0x80,					0,					0,		0x00000000,	0x01000000,	0x00000000,	"ETHERNET sACN Enabled"			},
		{SWS__(ALP_PID_ETHERNET_SACN_UNIVERSE),		2,	E120_DS_UNSIGNED_WORD,	E120_CC_GET,		0x80,					0,					0,		0x00000000,	0x01000000,	0x00000000,	"ETHERNET sACN Universe"		},
		{SWS__(ALP_PID_ETHERNET_ARTNET_ENABLED),	1,	E120_DS_UNSIGNED_BYTE,	E120_CC_GET,		0x80,					0,					0,		0x00000000,	0x01000000,	0x00000000,	"ETHERNET ArtNet Enabled"		},
		{SWS__(ALP_PID_ETHERNET_ARTNET_UNIVERSE),	9,	E120_DS_ASCII,			E120_CC_GET,		0x80,					0,					0,		0,			0,			0,			"ETHERNET ArtNet Universe"		},
};

__attribute__((section(".flash_Rdm")))
const tRdmSetup 				FLASH_RdmSetup =
{
	RDM_DEVICE_DEFAULT_SERIAL_NR,		// DeviceSerialNr
	RDM_DEVICE_DEFAULT_LABEL
};

// FLASH_PAGE --> FLASH_PRGADR
/* Start of this segment is loaded by addresses directly in *.ld file. */
__attribute__((section(".flash_PrgAdr")))
const tFirmwareInfo				FLASH_FirmwareInfo =
{
	/* Model ID			*/		RDM_DEVICE_MODEL_ID,
	/* Fw Version		*/		FIRMWARE_VERSION_HEX,
	/* Hw Version		*/		HARDWARE_VERSION_HEX,
	/* Fw Date			*/		FIRMWARE_DATE,
	/* Configuration	*/		CONFIGURATION_STRING,
	/* FwTxt			*/		FIRMWARE_VERSION,
	/* HwTxt			*/		HARDWARE_VERSION,
	/* DtTxt			*/		FIRMWARE_DATE_TXT
};


