/*
 * a2Rdm.c
 *
 *  Created on: 11. 1. 2016
 *      Author: J�n Harman
 */

/* Project includes */
#include "stm32h5xx.h"				// only for debug
#include <string.h>
#include <stdio.h>
#include "dc48_Globals.h"
#include "dc48_RdmDef.h"
#include "dc48_Rdm.h"
#include "dc48_DisplayConst.h"
#include "dc48_Ethernet.h"
//#include "dc5_Flash.h"
#include "rdm.h"
#include "dc48_Boot_Glob.h"
//#include "dc5_Display.h"
#include "dc48_Tools.h"

/* Global variables */
unsigned int			RDM_QueuedMessages[QUEUED_MESSAGES_BUFF_MAX];	// Message Queue
unsigned int			RDM_QueuedMsgPtr;			// Pointer to first free message record. If zero, no message is in the queue
int						RDM_WriteCalibrationDataConfirm = 0;
int						RDM_WriteDataError = 0;

#define MR_MESSAGE_IS_NOT_VALID_RDM_PACKET							0
#define MR_MESSAGE_IS_VALID_RDM_PACKET_BUT_NOT_FOR_THIS_DEVICE		1
#define MR_MESSAGE_IS_BROADCAST_TYPE								2
#define MR_MESSAGE_IS_FOR_THIS_DEVICE								3

#define ALL_MANUFACTURERS_UID		0xFFFF
#define ALL_DEVICES_UID				0xFFFFFFFF

#define	DI_RDM_PROTOCOL_VERSION			0x0100
#define	DI_PRODUCT_CATEGORY				E120_PRODUCT_CATEGORY_POWER_CONTROL
#define DI_SOFTWARE_VERSION				((*(int*)FIRMWARE_VERSION)-(*(int*)"0000"))
#define	RDM_WRITE_SN_CC					0x84
#define RDM_WRITE_SN_TRAIL_CODE			0x3B

unsigned short			ManufacturerUID;		// Manufacturer UID
unsigned int			DeviceUID;				// Device UID
unsigned short			ManufacturerUID_SWP;	// Manufacturer UID
unsigned int			DeviceUID_SWP;			// Device UID
int						DeviceIsMuted;			// Muting of the device itself
unsigned short			Remote_ManufactUID;		// Controller Manufacturer UID
unsigned int			Remote_DeviceUID;		// Controller Device UID
unsigned int *			GoToBootloaderCode;		// Code for go to bootloader after restart

//extern void Bytes_To_Hexadecimal(unsigned char * data, unsigned char * arr, unsigned char count, unsigned char byte_delimiter);
void Execute_GET_Command_Class(unsigned char * d, int msg_type);
//extern float eeCalibrationConstants[NUM_OF_CHROMACITIES][NUM_OF_STUDIO_CHANNELS];

int __changeEndian(unsigned int num)
{
	return __builtin_bswap32(num);
//	register int ret = num;
//	__asm(" rev r0, r0");
//	__asm(" bx lr");
//	return ret;
}

unsigned short __swapShort(unsigned short s)
{
	return (s>>8) + (s<<8);
}

int Count_Packet_CRC(char * d, int count)
{
	int crc = 0;
	while (count--) crc += *d++;
	return crc;
}

void writeAndSetDeviceLabel(int cnt, unsigned char * data)
{
	char * label = RdmSetup.DeviceLabel;
	while (cnt--)
		*label++ = *data++;
	*label = 0;
//	SaveSetupParametersToFLASH(WRITE_TO_FLASH__RDM_SETTINGS, 0);
}

int UID_in_range(unsigned char * d)
{
	// Low range
	unsigned short mid;
	unsigned int uid;

	mid = __swapShort(*(unsigned short*)d);
	if (mid > ManufacturerUID) return 0;
	if (mid == ManufacturerUID)
	{
			uid = __changeEndian(*(unsigned int*)&d[2]);
			if (uid > DeviceUID) return 0;
	}
	// High range
	mid = __swapShort(*(unsigned short*)&d[6]);
	if (mid < ManufacturerUID) return 0;
	if (mid == ManufacturerUID)
	{
			uid = __changeEndian(*(unsigned int*)&d[8]);
			if (uid < DeviceUID) return 0;
	}
	return 1;
}

void Fill_Default_RDM_DataMessagePlusCRC(int subdev)
{
	unsigned short crc;

	RDM_Packet.StartCode = E120_SC_RDM;
	RDM_Packet.SubStartCode = E120_SC_SUB_MESSAGE;
	RDM_Packet.MessageLength = EMPTY_MESSAGE_LENGTH_WO_CRC + RDM_Packet.ParameterDataLength;
	RDM_Packet.DestinUID.ManufacturerUID = Remote_ManufactUID;
	RDM_Packet.DestinUID.DeviceUID = Remote_DeviceUID;
	RDM_Packet.SourceUID.ManufacturerUID = ManufacturerUID_SWP;
	RDM_Packet.SourceUID.DeviceUID = DeviceUID_SWP;
	RDM_Packet.PortID = 0;
	RDM_Packet.MessageCount = RDM_QueuedMsgPtr;
	RDM_Packet.SubDevice = __swapShort(subdev);
	crc = Count_Packet_CRC((char*)&RDM_Packet, RDM_Packet.MessageLength);
	RDM_Packet.Data[RDM_Packet.ParameterDataLength+0] = crc >> 8;
	RDM_Packet.Data[RDM_Packet.ParameterDataLength+1] = crc;
	DmxRS485_RDM_DiscoveryResponse = RDM_Packet.MessageLength+2;
}

int Validate_RDM_Data_Packet(TRMD_Packet * rdm, int packet_size)
{
	unsigned int crc, rcrc, uid, crc_offset, uidok, uiddt, uidsn;

	if (packet_size < 26) return MR_MESSAGE_IS_NOT_VALID_RDM_PACKET;
	if (rdm->SubStartCode != E120_SC_SUB_MESSAGE) return MR_MESSAGE_IS_NOT_VALID_RDM_PACKET;
	if (rdm->MessageLength+2 > packet_size) return MR_MESSAGE_IS_NOT_VALID_RDM_PACKET;
	packet_size = rdm->MessageLength;
	crc_offset = packet_size-EMPTY_MESSAGE_LENGTH_WO_CRC;
	crc = Count_Packet_CRC((char *)rdm, packet_size);
	rcrc = (rdm->Data[crc_offset] << 8) + rdm->Data[crc_offset+1];
	if (crc != rcrc) return MR_MESSAGE_IS_NOT_VALID_RDM_PACKET;

	// Check if packet is for this device
	uid = __swapShort(rdm->DestinUID.ManufacturerUID);
	if (uid == ALL_MANUFACTURERS_UID) return MR_MESSAGE_IS_BROADCAST_TYPE;
	if (uid == THIS_DEVICE_MANUFACTURER_UID)
	{
			uid = __changeEndian(rdm->DestinUID.DeviceUID);
			if (uid == ALL_DEVICES_UID) return MR_MESSAGE_IS_BROADCAST_TYPE;
			// this device regular UID
			uidok = uid == DeviceUID;
			if (!uidok)
			{
				if (rdm->ParameterID == SWS__(DC4_PID_GET__HW_FW_INFO))
				{
					// ONLY for DC4_PID_GET__HW_FW_INFO
					// zero serial number -> valid only for one device on line
					// this device type must be same
					uiddt = uid & 0xFFFF0000;
					if ((uiddt && (uiddt == RDM_DEVICE_HIGH_SERIAL_NR)) || (!uiddt))
					{
						uidsn = uid & 0xFFFF;
						if (!uidsn || (uidsn == (DeviceUID & 0xFFFF)))
							uidok = 1;
					}
				}
			}
			if (uidok)
			{
					Remote_ManufactUID = rdm->SourceUID.ManufacturerUID;
					Remote_DeviceUID = rdm->SourceUID.DeviceUID;
					return MR_MESSAGE_IS_FOR_THIS_DEVICE;
			}
	}
	return MR_MESSAGE_IS_VALID_RDM_PACKET_BUT_NOT_FOR_THIS_DEVICE;
}

void Fill_ParameterDescription(unsigned short PID)
{
	int i = 0;
	RDM_Packet.DataS[0] = PID;
	RDM_Packet.ParameterDataLength = sizeof(TParameterDescription);
	while (i < NUM_OF_MANUFACTURER_SPECIFIC_IDS)
	{
		if (ParamDescriptor[i].PID == PID)
		{
			memcpy(RDM_Packet.Data, &ParamDescriptor[i], RDM_Packet.ParameterDataLength);
			return;
		}
		i++;
	}
	RDM_Packet.ParameterDataLength = 0x2;
}

void Fill_SlotInfo_Record(int subdev)
{
	const tDmxPersonality * per = &DmxSubDevPersonality[FLASH_DimmerSetup[subdev-1].Curve];
	int sc = per->ChNr;	//  0 or 1
	RDM_Packet.ParameterDataLength = sc * 5;
	char * pd = RDM_Packet.Data;
	if (sc) // 0 or 1
	{
		const tSlotInfo * si = per->Slot;
		*pd++ = 0;			// index high = 0
		*pd++ = 0;			// index low = 0	only one slot exists, so always 0
		*pd++ = 0;			// type = 0
		*pd++ = si->LabelId >> 8;
		*pd++ = si->LabelId & 0xFF;
	}
}

void Fill_Slot_Description_Data(int subdev, unsigned short slot)
{
	const tDmxPersonality * per = &DmxSubDevPersonality[FLASH_DimmerSetup[subdev-1].Curve];
	RDM_Packet.Data[0] = 0;
	RDM_Packet.Data[1] = slot;
	char * c = &RDM_Packet.Data[2];
	strcpy(c, per->Slot[0].Name);	// toto je uprava pro DC48, abych nemusel definovat 48 stejnych slotu
	RDM_Packet.ParameterDataLength = strlen(&RDM_Packet.Data[2])+2;
}

void Fill_SensorValue(int sns_nr)
{
	int value = 0;
	RDM_Packet.Data[0] = sns_nr;
	switch (sns_nr)
	{
	case 0: value = CpuTemperature; break;
	case 1: value = ExtTemperature; break;
	}
	*(unsigned short *)&RDM_Packet.Data[1] = __swapShort(value);
	*(unsigned short *)&RDM_Packet.Data[3] = 0;
	*(unsigned short *)&RDM_Packet.Data[5] = 0;
	*(unsigned short *)&RDM_Packet.Data[7] = 0;
	RDM_Packet.ParameterDataLength = 9;
}

void Write_PID_To_MessageQueue(unsigned short pid)
{
	if (RDM_QueuedMsgPtr >= QUEUED_MESSAGES_BUFF_MAX) RDM_QueuedMsgPtr = 0;
	RDM_QueuedMessages[RDM_QueuedMsgPtr++] = pid;
}

typedef struct
{
	unsigned int *	Adr;
	unsigned int 	Pid;
} tsAdrToPid;


#define NUM_OF_PARAMETERS_TO_QUEUE		6

const tsAdrToPid AdrPid[NUM_OF_PARAMETERS_TO_QUEUE] = {
		{&UnitSettings.DisplayOffTime,	SWS__(E120_DMX_START_ADDRESS)},
		{&UnitSettings.WorklightEn,		SWS__(ALP_PID_WORKLIGHT_ENABLED)},
		{&UnitSettings.EthernetToXLR,	SWS__(ALP_PID_WORKLIGHT_LEVEL)},
		{&UnitSettings.HoldLast,		SWS__(ALP_PID_HOLD_LAST_DMX_FAIL)},
		{&Eth_ETH_Enabled,				SWS__(ALP_PID_ETHERNET_SACN_ENABLED)},
		{&Eth_ETH_Enabled,				SWS__(ALP_PID_ETHERNET_ARTNET_ENABLED)},
};

void Write_To_MessageQueue_by_VarAddress(unsigned int * varaddr)
{
	for (int i=0; i<NUM_OF_PARAMETERS_TO_QUEUE; i++)
		if (AdrPid[i].Adr == varaddr)
		{
			Write_PID_To_MessageQueue(AdrPid[i].Pid);
			break;
		}
}

void Process_Queued_Message(void)
{
	unsigned int msg = 0;
	if (!RDM_QueuedMsgPtr)
		msg = SWS__(E120_STATUS_MESSAGES);
	else
	{
		RDM_QueuedMsgPtr--;
		msg = RDM_QueuedMessages[RDM_QueuedMsgPtr];
	}
	Execute_GET_Command_Class((unsigned char *)&msg, 0);
}

void Fill_ArtNet_Universe_ToDataField(void)
{
	char * c = RDM_Packet.Data;
	c += __byteToStr(Eth_ArtNet_Universe >> 8, c);
	*c++ = ':';
	c += __byteToStr((Eth_ArtNet_Universe >> 4) & 0x0f, c);
	*c++ = ':';
	c += __byteToStr(Eth_ArtNet_Universe & 0x0f, c);
//	c += sprintf(c, "%d:", EthernetSettings.ArtNet_Universe >> 8);
//	c += sprintf(c, "%d:", (EthernetSettings.ArtNet_Universe >> 4) & 0xF);
//	c += sprintf(c, "%d:", EthernetSettings.ArtNet_Universe & 0xF);
	RDM_Packet.ParameterDataLength = strlen(RDM_Packet.Data);
}

void Fill_Personality(int subdev)
{
	if (!subdev)
	{
		RDM_Packet.Data[0] = DMX_MODE;
		RDM_Packet.Data[1] = DMX_MODE_NUMBER_OF_MODES;
	}
	else
	{
		RDM_Packet.Data[0] = FLASH_DimmerSetup[subdev-1].Curve+1;
		RDM_Packet.Data[1] = DMX_NUBER_OF_SUBDEV_MODES;
	}
	RDM_Packet.ParameterDataLength = 2;
}

void Fill_PersonalityDescription(int subdev, unsigned int personality)
{
	if (!subdev) // root device
	{
		RDM_Packet.Data[0] = 1;
		RDM_Packet.Data[1] = 0;
		RDM_Packet.Data[2] = 0;
		strcpy(&RDM_Packet.Data[3], RDM_ROOT_DEV_PERSONALITY);
		RDM_Packet.ParameterDataLength = 3 + strlen(RDM_ROOT_DEV_PERSONALITY);
	}
	else
	{
		unsigned int pers = personality - 1;
		if (pers >= DMX_NUBER_OF_SUBDEV_MODES) pers = 0;
		RDM_Packet.Data[0] = personality;
		RDM_Packet.Data[1] = 0;
		const tDmxPersonality * per = &DmxSubDevPersonality[pers];
		RDM_Packet.Data[2] = per->ChNr;
		strcpy(&RDM_Packet.Data[3], per->Text);
		RDM_Packet.ParameterDataLength = 3 + strlen(per->Text);
	}
}

void Execute_DISCOVERY_Command_Class(unsigned char * d, int msg_type)
{
	unsigned short crc;
	char * disc_resp = (char *)&RDM_Packet;

	if (*d++) return;	// High byte must be zero!
	switch (*d) // Parameter ID LOW BYTE
	{
	case E120_DISC_UNIQUE_BRANCH:
			if (DeviceIsMuted) return;
			if (d[1] != 0x0C) return;
			if (!UID_in_range(&d[2])) return;
			DmxRS485_RDM_DiscoveryResponse = 24;
			// Tady se TRMD_Packet naplni bezhlave polem char. Protoze tato odpoved
			// (DISC_UNIQUE_BRANCH_RESPONSE) ma uplne jinou strukturu nez normalni RDM packet.
			*(int *)&disc_resp[0] = 0xFEFEFEFEUL;
			*(int *)&disc_resp[4] = 0xAAFEFEFEUL;
			disc_resp[8] = (ManufacturerUID >> 8) | 0xAA;
			disc_resp[9] = (ManufacturerUID >> 8) | 0x55;
			disc_resp[10] = ManufacturerUID | 0xAA;
			disc_resp[11] = ManufacturerUID | 0x55;
			disc_resp[12] = (DeviceUID >> 24) | 0xAA;
			disc_resp[13] = (DeviceUID >> 24) | 0x55;
			disc_resp[14] = (DeviceUID >> 16) | 0xAA;
			disc_resp[15] = (DeviceUID >> 16) | 0x55;
			disc_resp[16] = (DeviceUID >> 8) | 0xAA;
			disc_resp[17] = (DeviceUID >> 8) | 0x55;
			disc_resp[18] = DeviceUID | 0xAA;
			disc_resp[19] = DeviceUID | 0x55;
			crc = Count_Packet_CRC(&disc_resp[8], 12);
			disc_resp[20] = (crc >> 8) | 0xAA;
			disc_resp[21] = (crc >> 8) | 0x55;
			disc_resp[22] = crc | 0xAA;
			disc_resp[23] = crc | 0x55;
			break;
	case E120_DISC_MUTE:
			if (DeviceIsMuted) return;
			DeviceIsMuted = 1;
			if (msg_type == MR_MESSAGE_IS_BROADCAST_TYPE) return;
 			RDM_Packet.CommandClass = E120_DISCOVERY_COMMAND_RESPONSE;
			RDM_Packet.ParameterID = SWS__(E120_DISC_MUTE);
			RDM_Packet.ParameterDataLength = 2;
			RDM_Packet.DataS[0] = 0;
			RDM_Packet.TransactionNr = d[-7];
			Fill_Default_RDM_DataMessagePlusCRC(2);
			break;
	case E120_DISC_UN_MUTE:
			DeviceIsMuted = 0;
			if (msg_type == MR_MESSAGE_IS_BROADCAST_TYPE) return;
			RDM_Packet.CommandClass = E120_DISCOVERY_COMMAND_RESPONSE;
			RDM_Packet.ParameterID = SWS__(E120_DISC_UN_MUTE);
			RDM_Packet.ParameterDataLength = 2;
			RDM_Packet.DataS[0] = 0;
			RDM_Packet.TransactionNr = d[-7];
			Fill_Default_RDM_DataMessagePlusCRC(2);
			break;
	}
}


void Execute_GET_Command_Class(unsigned char * d, int msg_type)
{
	int i, x, y;
	char * c;
	unsigned short pid = *(unsigned short *)d;
	unsigned short sub = d[-2];	// Sub-Device, fetch only byte, because we have only 48 sub-devices
	if (msg_type == MR_MESSAGE_IS_BROADCAST_TYPE) return;
	RDM_Packet.CommandClass = E120_GET_COMMAND_RESPONSE;
	RDM_Packet.ParameterID = pid;
	RDM_Packet.TransactionNr = d[-6];
	switch (pid)
	{
	case SWS__(E120_QUEUED_MESSAGE):
			Process_Queued_Message();
			RDM_Packet.TransactionNr = d[-6];
			break;
	case SWS__(E120_STATUS_MESSAGES):
			// toto nefunguje na DMX CAT
			RDM_Packet.ParameterDataLength = 0; //9;
			RDM_Packet.DataS[0] = 0; // SubDevice ID -> 0 = root device
			RDM_Packet.Data[2] = E120_STATUS_ERROR;
			RDM_Packet.Data[3] = STS_OVERTEMP >> 8;
			RDM_Packet.Data[4] = STS_OVERTEMP & 0xFF;
			RDM_Packet.Data[5] = 0;
			RDM_Packet.Data[6] = 0;
			RDM_Packet.Data[7] = 2;
			RDM_Packet.Data[8] = 45;
			break;
	case SWS__(E120_STATUS_ID_DESCRIPTION):
			// toto nefunguje na DMX CAT
			RDM_Packet.ParameterDataLength = 0; //9;
			i = (d[3] << 8) + d[4];
			if (i == STS_OVERTEMP)
			{
				RDM_Packet.ParameterDataLength = 32;
				memcpy(RDM_Packet.Data, STS_MSG_DEFINITION__OVERTEMP, 32);
			}
			break;
	case SWS__(E120_SUPPORTED_PARAMETERS):
			RDM_Packet.ParameterDataLength = sizeof(SupportedParameterList);
			memcpy(RDM_Packet.DataS, SupportedParameterList, RDM_Packet.ParameterDataLength);
			break;
	case SWS__(E120_PARAMETER_DESCRIPTION):
			Fill_ParameterDescription(*(unsigned short *)&d[3]);
			break;
	case SWS__(E120_DEVICE_INFO):
			RDM_Packet.ParameterDataLength = 0x13;
			RDM_Packet.DataS[0] = SWS__(DI_RDM_PROTOCOL_VERSION);
			RDM_Packet.DataS[1] = SWS__(RDM_DEVICE_MODEL_ID);
			RDM_Packet.DataS[2] = SWS__(DI_PRODUCT_CATEGORY);
			RDM_Packet.DataS[3] = DI_SOFTWARE_VERSION;
			RDM_Packet.DataS[4] = (DI_SOFTWARE_VERSION)>>16;
			RDM_Packet.Data[10] = 0;	// DMX Footprint High byte
			if (!sub) // root device
			{
				RDM_Packet.Data[11] = RDM_DMX512_FOOTPRINT; // DMX Footprint is zero for root device
				RDM_Packet.Data[12] = DMX_MODE;		// DMX Personality
				RDM_Packet.Data[13] = DMX_MODE_NUMBER_OF_MODES;
				RDM_Packet.DataS[7] = 0xFFFF;						// Start Address is -1 because of sub-devices
			}
			else
			{
				i = sub-1;
				x = FLASH_DimmerSetup[i].Curve;
				y = 1;
				if ((x == DIMM_CURVE_OFF) || (x == DIMM_CURVE_ALW_ON)) y = 0;
				RDM_Packet.Data[11] = y; // DMX Footprint is 1 for sub-device if no OFF or ON
				RDM_Packet.Data[12] = x+1;	// Dimmer Curve as DMX Personality
				RDM_Packet.Data[13] = DMX_NUBER_OF_SUBDEV_MODES;		// Nuber of Curves
				RDM_Packet.DataS[7] = (y)?__swapShort(Patch[i]):0xFFFF;		// Start Address is Patch
			}
			RDM_Packet.Data[16] = 0; // Sub-devices H
			RDM_Packet.Data[17] = 48; //RDM_SUB_DEVICES_COUNT L;
			RDM_Packet.Data[18] = (!sub)?RDM_SENSOR_COUNT:0;		// Sensor Count
			break;
	case SWS__(E120_DEVICE_MODEL_DESCRIPTION):
			strcpy(RDM_Packet.Data, RDM_DEVICE_MODEL_DESCRIPTION);
			RDM_Packet.ParameterDataLength = strlen(RDM_DEVICE_MODEL_DESCRIPTION);
			break;
	case SWS__(E120_MANUFACTURER_LABEL):
			strcpy(RDM_Packet.Data, RDM_MANUFACTURER_LABEL);
			RDM_Packet.ParameterDataLength = strlen(RDM_MANUFACTURER_LABEL);
			break;
	case SWS__(E120_DEVICE_LABEL):
			if (!sub)
			{
				RdmSetup.DeviceLabel[32] = 0;
				strcpy(RDM_Packet.Data, RdmSetup.DeviceLabel);
			}
			else
			{
				strcpy(RDM_Packet.Data, "Dimmer #");
				c = &RDM_Packet.Data[8];
				c += __byteToStr(sub, &RDM_Packet.Data[8]);
				i = sub-1;
				x = FLASH_DimmerSetup[i].Curve;
				if (x == DIMM_CURVE_OFF) strcpy(c, " - Always OFF");
				else
				if (x == DIMM_CURVE_ALW_ON) strcpy(c, " - Always ON");
			}
			RDM_Packet.ParameterDataLength = strlen(RDM_Packet.Data);
			break;
	case SWS__(E120_BOOT_SOFTWARE_VERSION_ID):
//			RDM_Packet.DataI[0] = BootLdrVer.Version;
			RDM_Packet.ParameterDataLength = 4;
			break;
	case SWS__(E120_BOOT_SOFTWARE_VERSION_LABEL):
//			strcpy(RDM_Packet.Data, BootLdrVer.Text);
//			RDM_Packet.ParameterDataLength = strlen(BootLdrVer.Text);
			break;
	case SWS__(E120_SOFTWARE_VERSION_LABEL):
			strcpy(RDM_Packet.Data, HARDWARE_VERSION "-");
			strcat(RDM_Packet.Data, FIRMWARE_VERSION);
			strcat(RDM_Packet.Data, "-");
			strcat(RDM_Packet.Data, FIRMWARE_DATE_TXT);
			strcat(RDM_Packet.Data, "-");
			strcat(RDM_Packet.Data, RDM_PROTOCOL_VERSION);
			RDM_Packet.ParameterDataLength = strlen(RDM_Packet.Data);
			break;
	case SWS__(E120_DMX_PERSONALITY):
			Fill_Personality(sub);
			break;
	case SWS__(E120_DMX_PERSONALITY_DESCRIPTION):
			Fill_PersonalityDescription(sub, d[3]);
			break;
	case SWS__(E120_DMX_START_ADDRESS):
			if (!sub)
				RDM_Packet.DataS[0] = 0xFFFF;
			else
				RDM_Packet.DataS[0] = __swapShort(Patch[sub-1]);
			RDM_Packet.ParameterDataLength = 2;
			break;
	case SWS__(E120_SLOT_INFO):
			Fill_SlotInfo_Record(sub);
			break;
	case SWS__(E120_SLOT_DESCRIPTION):
			Fill_Slot_Description_Data(sub, __swapShort(*(unsigned short *)&d[3]));
			break;
	case SWS__(E137_DMX_FAIL_MODE):
			RDM_Packet.DataS[0] = 0;
			RDM_Packet.DataS[1] = UnitSettings.HoldLast*0xFFFF;
			RDM_Packet.DataS[2] = 0;
			RDM_Packet.DataS[3] = 0;
			RDM_Packet.ParameterDataLength = 7;
			break;
	case SWS__(E120_SENSOR_DEFINITION):
			memcpy(RDM_Packet.Data, SensorDefinition[d[3]], 30);
			RDM_Packet.ParameterDataLength = 30;
			break;
	case SWS__(E120_SENSOR_VALUE):
			Fill_SensorValue(d[3]);
			break;
	case SWS__(E120_DEVICE_HOURS):
			i = WorkHours10min/6;
			*(int*)RDM_Packet.Data = __changeEndian(i);
			RDM_Packet.ParameterDataLength = 4;
			break;
	case SWS__(E120_DEVICE_POWER_CYCLES):
			*(int*)RDM_Packet.Data = __changeEndian(NumberOfStarts);
			RDM_Packet.ParameterDataLength = 4;
			break;
	case SWS__(E120_IDENTIFY_DEVICE):
			if (sub) x = RDM_IdentifiingDevice[sub-1]; else x = 0;
			RDM_Packet.Data[0] = x;
			RDM_Packet.ParameterDataLength = 1;
			break;
	case SWS__(ALP_PID_WORKLIGHT_ENABLED):
			RDM_Packet.Data[0] = UnitSettings.WorklightEn;
			RDM_Packet.ParameterDataLength = 1;
			break;
	case SWS__(ALP_PID_HOLD_LAST_DMX_FAIL):
			RDM_Packet.Data[0] = UnitSettings.HoldLast;
			RDM_Packet.ParameterDataLength = 1;
			break;
	case SWS__(ALP_PID_ETHERNET_SACN_ENABLED):
			RDM_Packet.Data[0] = !!(Eth_ETH_Enabled & ETHERNET_STS_ENABLED_SACN);
			RDM_Packet.ParameterDataLength = 1;
			break;
	case SWS__(ALP_PID_ETHERNET_SACN_UNIVERSE):
			RDM_Packet.Data[0] = __swapShort(Eth_sACN_Universe);
			RDM_Packet.ParameterDataLength = 2;
			break;
	case SWS__(ALP_PID_ETHERNET_ARTNET_ENABLED):
			RDM_Packet.Data[0] = !!(Eth_ETH_Enabled & ETHERNET_STS_ENABLED_ARTNET);
			RDM_Packet.ParameterDataLength = 1;
			break;
	case SWS__(ALP_PID_ETHERNET_ARTNET_UNIVERSE):
			Fill_ArtNet_Universe_ToDataField();
			break;
	case SWS__(DC4_PID_GET__HW_FW_INFO):
			RDM_Packet.ParameterDataLength = 8*4;
			RDM_Packet.DataI[0] = 888; //DC5_BOOTLOADER_VERSION;
			RDM_Packet.DataI[1] = 888; //DC5_BOOTLOADER_DATE;
			memcpy(&RDM_Packet.DataI[2], &FLASH_FirmwareInfo, 24);
			break;
	default:
			RDM_Packet.ParameterDataLength = 0;
			return;
	}
	if (msg_type) Fill_Default_RDM_DataMessagePlusCRC(sub);
}

void Execute_SET_Command_Class(unsigned char * d, int msg_type)
{
	unsigned short pid = *(unsigned short *)d;
	unsigned short sub = d[-2] + (d[-3]<<8);	// Sub-Device, fetch only byte, because we have only 48 sub-devices
	unsigned int adr, ui, c;
	int i, x;
#ifdef DEVICE_CAN_BE_CALIBRATED
	unsigned int * cd;
#endif
	if (msg_type == MR_MESSAGE_IS_BROADCAST_TYPE) return;
	RDM_Packet.CommandClass = E120_SET_COMMAND_RESPONSE;
	RDM_Packet.ParameterID = pid;
	RDM_Packet.ParameterDataLength = 0;
	switch (pid)
	{
	case SWS__(E120_DMX_PERSONALITY):
#ifdef COMPILATION_FOR__DSL
			x = d[3]-1;
			if ((x != UnitSettings.StroboEnabled) && (x < DMX_MODE_NUMBER_OF_MODES))
			{
				UnitSettings.StroboEnabled = x;
				SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			}
#endif
#ifdef COMPILATION_FOR__DLL
			x = d[3]-1;
			if ((x != UnitSettings.CtrlChEnabled) && (x < 2))
			{
				UnitSettings.CtrlChEnabled = x;
				SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			}
#endif
			break;
	case SWS__(E120_DMX_START_ADDRESS):
			adr = (d[3] << 8) + d[4];
			if ((!adr) | (adr > MAX_SETUPABLE_DMX_ADDRESS)) return;
			Patch[0] = adr;
//			SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			break;
	case SWS__(E120_DEVICE_LABEL):
			writeAndSetDeviceLabel(d[2], &d[3]);
			break;
	case SWS__(E137_DMX_FAIL_MODE):
			x = (d[5] == 0xFF) && (d[6] == 0xFF);
			if (x != UnitSettings.HoldLast)
			{
				UnitSettings.HoldLast = x;
//				SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			}
			break;
	case SWS__(E120_IDENTIFY_DEVICE):
			if (!sub) break;
			x = d[3]*100;
			if (sub <= NUM_OF_CHANNELS)
				RDM_IdentifiingDevice[sub-1] = x;
			else
			if (sub == 0xFFFF)
				for (i=0; i<NUM_OF_CHANNELS; i++)
					RDM_IdentifiingDevice[i] = x;
			break;
	case SWS__(ALP_PID_WORKLIGHT_ENABLED):
			x = !!d[3];
			if (x != UnitSettings.WorklightEn)
			{
				UnitSettings.WorklightEn = x;
//				SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			}
			break;
	case SWS__(ALP_PID_HOLD_LAST_DMX_FAIL):
			x = !!d[3];
			if (x != UnitSettings.HoldLast)
			{
				UnitSettings.HoldLast = x;
//				SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			}
			break;
	case SWS__(ALP_PID_ETHERNET_SACN_ENABLED):
			ui = !!d[3];
			if (ui != Eth_ETH_Enabled)
			{
				Eth_ETH_Enabled = ui;
//				SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			}
			break;
	case SWS__(ALP_PID_ETHERNET_ARTNET_ENABLED):
			ui = !!d[3];
			if (ui != Eth_ETH_Enabled)
			{
				Eth_ETH_Enabled = ui;
//				SaveSetupParametersToFLASH(WRITE_TO_FLASH__UNIT_SETTINGS, 0);
			}
			break;
	case SWS__(AII_PID_FIXTURE_SERIAL_NUMBER):
			if (d[3] == RDM_WRITE_SN_CC)
				if (d[8] == RDM_WRITE_SN_TRAIL_CODE)
				{
					DeviceUID = (d[6] << 8) + d[7] + RDM_DEVICE_HIGH_SERIAL_NR;
					DeviceUID_SWP = __changeEndian(DeviceUID);
					RdmSetup.DeviceSerialNr = DeviceUID;
//					SaveSetupParametersToFLASH(WRITE_TO_FLASH__RDM_SETTINGS, 0);
				}
			RDM_Packet.ParameterDataLength = 4;
			*(unsigned int *)&RDM_Packet.Data[0] = DeviceUID_SWP;
			break;
	case SWS__(DC4_PID_GO_TO_BOOTLOADER):													// ---- DC4_PID_GO_TO_BOOTLOADER
			GoToBootloaderCode = FlashWrBuf;
			c = GO_TO_BOOTLOADER_CODE;
			for (int i=0; i<4; i++){
				GoToBootloaderCode[i] = c;
				c += GO_TO_BOOTLOADER_CODE_ADD;
			}
			// no break
	case SWS__(DC4_PID_RESET_DEVICE):														// ---- DC4_PID_RESET_DEVICE
			NVIC_SystemReset();
			return;
	}
	Fill_Default_RDM_DataMessagePlusCRC(0);
}

void Process_valid_RDM_message(unsigned char * d, int msg_type)
{
	LED_BlueCtc = 5;
	switch (d[RPF_COMMAND_CLASS])
	{
	case E120_DISCOVERY_COMMAND:
			Execute_DISCOVERY_Command_Class(&d[RPF_PARAMETER_ID], msg_type);
			break;
	case E120_GET_COMMAND:
			Execute_GET_Command_Class(&d[RPF_PARAMETER_ID], msg_type);
			break;
	case E120_SET_COMMAND:
			Execute_SET_Command_Class(&d[RPF_PARAMETER_ID], msg_type);
			break;
	}
//	GlobRdmPresentKillCnt_Xlr = 50;
}

void Process_DMX_RS485_RDM_Data_Packet(void)
{
	int v = Validate_RDM_Data_Packet((TRMD_Packet*)DMX_XLR_RecBuffer, DmxRS485_RDM_DataToProcess);
	DmxRS485_RDM_DataToProcess = 0;
	if (!v) return;
	*DMX_XLR_RecBuffer = 255;	// To know UART that it is processed
	DMX_Is_Present |= DMX_PR_RDM;
//	GlobRdmPresentKillCnt_Xlr = 45;	// 3600 ms --> killing is in a2SignalProcess

	// RDM message is valid -> process message if it is for this device
	if (v > MR_MESSAGE_IS_VALID_RDM_PACKET_BUT_NOT_FOR_THIS_DEVICE)
			Process_valid_RDM_message(DMX_XLR_RecBuffer, v);
}

void Initialization_RDM(void)
{
	DeviceIsMuted =
	DmxRS485_RDM_DiscoveryResponse =
	RDM_QueuedMsgPtr = 0;
	memset(RDM_IdentifiingDevice, 0, sizeof(RDM_IdentifiingDevice));
	ManufacturerUID = THIS_DEVICE_MANUFACTURER_UID;
	ManufacturerUID_SWP = __swapShort(THIS_DEVICE_MANUFACTURER_UID);
	DeviceUID = RdmSetup.DeviceSerialNr + RDM_DEVICE_HIGH_SERIAL_NR;
	DeviceUID_SWP = __changeEndian(DeviceUID);
//	WritingToFlashOK = 0;
//	WritingToFlashError = 0;
}

