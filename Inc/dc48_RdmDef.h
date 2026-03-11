/*
 * dc4_RdmDef.h
 *
 *  Created on: Apr 9, 2024
 *      Author: harma
 */

#ifndef DC4_RDMDEF_H_
#define DC4_RDMDEF_H_

#define STR_HELPER(x) 					#x
#define STR(x) 							STR_HELPER(x)

#define RDM_DEVICE_DEFAULT_SERIAL_NR	1739


#define RDM_DEVICE_HIGH_SERIAL_NR		(RDM_DEVICE_MODEL_ID << 16)
#define THIS_DEVICE_MANUFACTURER_UID	0x01A2		// Assigned by ESTA

#define	EMPTY_MESSAGE_LENGTH_WO_CRC		24
#define	RDM_PROTOCOL_VERSION			"R025"

#define SWS__(a)			((((a<<8)+(a>>8))&0xffff))

/* RDM packet fields */
#define	RPF_START_CODE				0
#define RPF_SUB_START_CODE			1
#define RPF_MESSAGE_LENGTH			2
#define RPF_DESTINATION_UID			3
#define RPF_SOURCE_UID				9
#define RPF_TRANSACTION_NR			15
#define RPF_PORT_ID					16
#define RPF_MESSAGE_COUNT			17
#define RPF_SUBDEVICE				18
#define RPF_COMMAND_CLASS			20
#define RPF_PARAMETER_ID			21
#define RPF_PARAMETER_DATA_LEN		23
#define RPF_PARAMETER_DATA			24
#define RPF_CHECKSUM_H				24
#define RPF_CHECKSUM_L				25

typedef struct
{
	unsigned short	LabelId;
	char			Name[24];
}	tSlotInfo;

typedef struct
{
	char						Text[24];
	int							ChNr;
	tSlotInfo					Slot[1];
}	tDmxPersonality;

typedef struct __attribute__((__packed__))
{
		unsigned short		PID;
		unsigned char		PDL_Size;
		unsigned char		DataType;
		unsigned char		CC;
		unsigned char		Type;
		unsigned char		Unit;
		unsigned char		Prefix;
		unsigned int		Min;
		unsigned int		Max;
		unsigned int		Def;
		unsigned char		Description[32];
} TParameterDescription;

#define RDM_DEVICE_MODEL_DESCRIPTION		"DC48"
#define RDM_DEVICE_DEFAULT_LABEL			"Fixture-" RDM_DEVICE_MODEL_DESCRIPTION "-" STR(RDM_DEVICE_DEFAULT_SERIAL_NR)
#define RDM_ROOT_DEV_PERSONALITY			"Dimmer 48-Channels"
#define	RDM_DEVICE_MODEL_ID					0xDC48			// as it is DC48
#define	DMX_MODE_NUMBER_OF_MODES			1
#define	DMX_MODE							1
#define DMX_NUBER_OF_SUBDEV_MODES			6
#define RDM_SENSOR_COUNT					2
#define RDM_DMX512_FOOTPRINT				0
#define RDM_SUB_DEVICES_COUNT				48
#define NUM_OF_MANUFACTURER_SPECIFIC_IDS	6
#define NUM_OF_ALL_SUPPORTED_IDS			14+NUM_OF_MANUFACTURER_SPECIFIC_IDS
extern const short 							SupportedParameterList[NUM_OF_ALL_SUPPORTED_IDS];
extern const TParameterDescription 			ParamDescriptor[NUM_OF_MANUFACTURER_SPECIFIC_IDS];

extern const tDmxPersonality				DmxSubDevPersonality[DMX_NUBER_OF_SUBDEV_MODES];
extern const char 							SensorDefinition[RDM_SENSOR_COUNT][30];

typedef struct
{
	unsigned int			DeviceSerialNr;
	char					DeviceLabel[36];
}	tRdmSetup;

typedef union __attribute__((__packed__))
{
	char	DestinationUID[6];
	struct __attribute__((__packed__))
	{
		short ManufacturerUID;
		unsigned int DeviceUID;
	};
} UUID;

#define RDM_PACKET_DATA_ARRAY_SIZE		256

typedef struct __attribute__((__packed__))
{
	char 		StartCode;
	char		SubStartCode;
	char		MessageLength;	// 24 without data and CRC
	UUID		DestinUID;
	UUID		SourceUID;
	char		TransactionNr;
	char		PortID;
	char		MessageCount;
	short		SubDevice;
	char		CommandClass;
	short		ParameterID;
	char		ParameterDataLength;
	union
	{
		char			Data[RDM_PACKET_DATA_ARRAY_SIZE];		// Message Data Block - empty if PDL = 0
		short			DataS[RDM_PACKET_DATA_ARRAY_SIZE/2];
		unsigned int	DataI[RDM_PACKET_DATA_ARRAY_SIZE/4];
	};
} TRMD_Packet;

#define RDM_MANUFACTURER_LABEL			"Art Lighting"

extern tRdmSetup				RdmSetup;
extern const tRdmSetup			FLASH_RdmSetup;
extern TRMD_Packet 				RDM_Packet;				// Data pro vysilani RDM
extern const unsigned int		cThisDeviceManufUID;

typedef struct
{
	unsigned int			ModelId;
	unsigned int			Version;
	unsigned int			HwVer;
	unsigned int			FWDate;
	char					Config[8];
	char					FwTxt[8];
	char					HwTxt[8];
	char					DtTxt[8];
} tFirmwareInfo;

extern const tFirmwareInfo		FLASH_FirmwareInfo;

#endif /* DC4_RDMDEF_H_ */
