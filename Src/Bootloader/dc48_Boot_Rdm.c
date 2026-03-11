/*
 * dc4_Boot_Rdm.c
 *
 *  Created on: Apr 9, 2024
 *      Author: harma
 */

#include <dc48_Boot_Disp.h>
#include <dc48_Boot_Flash.h>
#include <dc48_Boot_Glob.h>
#include "stm32h5xx.h"
#include "string.h"
#include "rdm.h"
#include "dc48_Globals.h"
#include "dc48_RdmDef.h"

unsigned short 				blRemote_ManufactUID;
unsigned int				blRemote_DeviceUID;
unsigned short				blManufacturerUID_swp;
unsigned int				blDeviceUID_swp;
unsigned int				blFwActualSectorAdr;		// Sector start address
unsigned int				blFwActualSectorOfs;		// Sector offset
unsigned int				blFwActualSectorUse;		// Sector using bits -> each 256 bytes is one bit. If whole sector is filled it is 0xFF

unsigned short _bl__swapShort(unsigned short s)
{
	return (s>>8) + (s<<8);
}

int bl_Count_Packet_CRC(char * d, int count)
{
	int crc = 0;
	while (count--) crc += *d++;
	return crc;
}

void _bl_Fill_Default_RDM_DataMessagePlusCRC(int PDL)
{
	unsigned short crc;

	RDM_Packet.StartCode = E120_SC_RDM;
	RDM_Packet.SubStartCode = E120_SC_SUB_MESSAGE;
	RDM_Packet.MessageLength = EMPTY_MESSAGE_LENGTH_WO_CRC + RDM_Packet.ParameterDataLength;
	RDM_Packet.DestinUID.ManufacturerUID = blRemote_ManufactUID;
	RDM_Packet.DestinUID.DeviceUID = blRemote_DeviceUID;
	RDM_Packet.SourceUID.ManufacturerUID = blManufacturerUID_swp;
	RDM_Packet.SourceUID.DeviceUID = blDeviceUID_swp;
	RDM_Packet.PortID = 0;
	RDM_Packet.MessageCount = 0;
	RDM_Packet.SubDevice = 0;
	crc = bl_Count_Packet_CRC((char*)&RDM_Packet, RDM_Packet.MessageLength);
	RDM_Packet.Data[RDM_Packet.ParameterDataLength+0] = crc >> 8;
	RDM_Packet.Data[RDM_Packet.ParameterDataLength+1] = crc;
}

int _bl_Validate_RDM_Data_Packet(TRMD_Packet * rdm, int packet_size)
{
	unsigned int crc, rcrc, uid, crc_offset;

	if (packet_size < 26) return 0;
	if (rdm->SubStartCode != 1) return 0;
	if (rdm->MessageLength+2 > packet_size) return 0;
	packet_size = rdm->MessageLength;
	crc_offset = packet_size-24;
	crc = bl_Count_Packet_CRC((char *)rdm, packet_size);
	rcrc = (rdm->Data[crc_offset] << 8) + rdm->Data[crc_offset+1];
	if (crc != rcrc) return 0;

	// Check if packet is for this device
	uid = _bl__swapShort(rdm->DestinUID.ManufacturerUID);
	if (uid == 0xFFFF) return 0;								// ALL_MANUFACTURERS_UID
	if (uid == THIS_DEVICE_MANUFACTURER_UID)
	{
			uid = __builtin_bswap32(rdm->DestinUID.DeviceUID);
			if (uid == 0xFFFFFFFF) return 0;	// ALL_DEVICES_UID
			if (uid == blDeviceUID)
			{
					blRemote_ManufactUID = rdm->SourceUID.ManufacturerUID;
					blRemote_DeviceUID = rdm->SourceUID.DeviceUID;
					return 1;
			}
	}
	return 0;
}

//void bl_ForceResponseHwFwInfo(void)
//{
//	TRMD_Packet * rdm = (TRMD_Packet*)blRxBuffer;
//	rdm->StartCode = E120_SC_RDM;
//	rdm->SubStartCode = 1;
//	rdm->MessageLength = EMPTY_MESSAGE_LENGTH_WO_CRC;
//	rdm->DestinUID.ManufacturerUID = blManufacturerUID_swp;
//	rdm->DestinUID.DeviceUID = blDeviceUID_swp;
//	rdm->CommandClass = E120_GET_COMMAND;
//	rdm->ParameterID = SWS__(DC4_PID_GET__HW_FW_INFO);
//	unsigned short crc = bl_Count_Packet_CRC(blRxBuffer, rdm->MessageLength);
//	rdm->Data[0] = crc >> 8;
//	rdm->Data[1] = crc;
//	blRxProcessFrame = 26;	// minimum number of recived characters
//}

void _bl_memcpy(char * dst, char * src, int cnt)
{
	while (cnt--) *dst++ = *src++;
}

void _bl_Form_RDM_GET_Response(TRMD_Packet * rdm, int packet_size)
{
	RDM_Packet.CommandClass = E120_GET_COMMAND_RESPONSE;
	RDM_Packet.ParameterID = rdm->ParameterID;
	RDM_Packet.TransactionNr = rdm->TransactionNr;
	switch (rdm->ParameterID)
	{
	case SWS__(DC4_PID_GET__HW_FW_INFO):
			RDM_Packet.ParameterDataLength = 8*4;
			RDM_Packet.DataI[0] = DC48_BOOTLOADER_VERSION + DC48_BOOTLOADER_ID_VER;
			RDM_Packet.DataI[1] = DC48_BOOTLOADER_DATE;
			_bl_memcpy(&RDM_Packet.Data[8], (char *)&FLASH_FirmwareInfo, 24);
			break;
	default: RDM_Packet.CommandClass = 0;
	}
}

void _bl_Check_FW_Data_and_write(char * c)
{
	unsigned int * d = (unsigned int *)c;
	if (d[0] != 0xAAAABBBB) return;
	if (d[BOOT_PROTOCOL_END] != 0xEEEE5555) return;
	unsigned int adr = d[1];
	if (adr < BOOT_FW_START_ADDRESS) return;
	if (adr > BOOT_FW_FW_ADR_LAST_ADDRESS) return;	// jen formalita, neni uplne dobre
	blFlashFrameActual = c[8];
	blFlashFramesTotal = c[9];
	blFlashSectorActual = c[10];
	blFlashSectorsTotal = c[11];

	unsigned int crc = 0;
	for (int i=0; i<=BOOT_PROTOCOL_END; i++)
	{
		crc ^= *d++;
	}
	if (crc != *d) return;
	RDM_Packet.DataI[0] = adr;
	d = (unsigned int *)c;
	int cnt = 13;
	if ((adr & 0x7FF) >= 1872) cnt = 11;
	bl_Flash_Write_to_Flash(adr, &d[3], cnt);
}

void _bl_Form_RDM_SET_Command(TRMD_Packet * rdm, int packet_size)
{
	RDM_Packet.CommandClass = E120_SET_COMMAND_RESPONSE;
	RDM_Packet.ParameterID = rdm->ParameterID;
	RDM_Packet.TransactionNr = rdm->TransactionNr;
	unsigned int c;
	unsigned int * gotoblcode = FlashWrBuf;
	switch (rdm->ParameterID)
	{
	case SWS__(DC4_PID_GO_TO_BOOTLOADER):
			c = GO_TO_BOOTLOADER_CODE;
			for (int i=0; i<4; i++){
				gotoblcode[i] = c;
				c += GO_TO_BOOTLOADER_CODE_ADD;
			}
			// no break
	case SWS__(DC4_PID_RESET_DEVICE):
			NVIC_SystemReset();
			break;
	case SWS__(DC4_PID_LOAD_FIRMWARE):
			RDM_Packet.ParameterDataLength = 4;
			RDM_Packet.DataI[0] = 0;
			if (rdm->ParameterDataLength == 228)
				_bl_Check_FW_Data_and_write(&rdm->Data[0]);
			blFlashingInProgressCtc = 120000;
			break;
	default: RDM_Packet.CommandClass = 0;
	}
}

void bl_ProcessRxData(int size)
{
	if (*blRxBuffer != E120_SC_RDM) return;
	blRdmPresentLED = 10;	// LED DMX/RDM present
	TRMD_Packet * pac = (TRMD_Packet*)blRxBuffer;
	if (!_bl_Validate_RDM_Data_Packet(pac, size)) return;
	blRxFramesCnt++;
	blCommActive = 1;

	// Valid Frame -> form response
	RDM_Packet.CommandClass = 0;		// make response invalid
	switch (pac->CommandClass)
	{
	case E120_GET_COMMAND:
			_bl_Form_RDM_GET_Response(pac, size);
			break;
	case E120_SET_COMMAND:
			_bl_Form_RDM_SET_Command(pac, size);
			break;
	}
	if (RDM_Packet.CommandClass)		// if response is valid ->> transmit
	{
		_bl_Fill_Default_RDM_DataMessagePlusCRC(0);
		blRdmTxFramePrepared = RDM_Packet.MessageLength+2;
	}
}

void bl_InitializationRDM(void)
{
	blManufacturerUID_swp = _bl__swapShort(THIS_DEVICE_MANUFACTURER_UID);
	blDeviceUID = (FLASH_FirmwareInfo.ModelId << 16) + (FLASH_RdmSetup.DeviceSerialNr & 0xFFFF);
//	blDeviceUID = FLASH_RdmSetup.DeviceSerialNr;
	blDeviceUID_swp	= __builtin_bswap32(blDeviceUID);
	blRdmTxFramePrepared = 0;
}


