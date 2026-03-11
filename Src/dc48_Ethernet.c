/*
 * dc48_Ethernet.c
 *
 *  Created on: 5. 2. 2026
 *      Author: harma
 */

#include <stdint.h>
#include <string.h>
#include "stm32h523xx.h"
#include "dc48_Globals.h"
#include "dc48_Ethernet.h"
#include "dc48_Flash.h"
#include "dc48_Uart_Ethernet.h"

#define NO_VERSION		"----"

// Global Variables
unsigned int	 		Eth_ModuleStatusBits;			// Status of the module read from the module by SPI
int 					Eth_EthernetStatus;				// Total Status

//char const * 			DispEthernetActiveTxt;		// Text co je aktivni jesli sACN, nebo ArtNet
//int					Eth_StreamACN_Universe;		// Streaming ACN Universe number
//int					Eth_ArtNet_Universe;		// ArtNet Universe number (Net:SubNet:Universe)
unsigned int			Eth_IP_Address;					// Ip adresa ETH modulu
unsigned int			Eth_MAC_Address[2];				// MAC adresa ETH modulu
char 					Eth_Mod_HwVerTxt[8] = NO_VERSION;
char 					Eth_Mod_FwVerTxt[8] = NO_VERSION;
unsigned int			Eth_ReceivedFPS;				// Received Frames Per Second
unsigned int			Eth_AcceptedFPS;				// Accepted Frames Per Second
unsigned int			Eth_DisplayIpAddress[4];		// Ip adresa pro display a zmenu
unsigned int			Eth_ETH_Enabled;				// sACN[1] or ArtNet[0] Enabled set in module
unsigned int			Eth_sACN_Universe;
unsigned int			Eth_ArtNet_Universe;			// ArtNet Universe set in module
unsigned int			Eth_IP_Address_ToChange;		// IP address for save to ETH module
unsigned int			Eth_Universes_ToChange;			// Both Universes for save to ETH module
unsigned int			Eth_Enabled_ToChange;			// Both Enabled for save to ETH module
unsigned int			Eth_Disp_ETH_Enabled;			// sACN[1] or ArtNet[0] Enabled set in module
unsigned int			Eth_Disp_sACN_Universe;			// sACN Universe set in module
unsigned int			Eth_Disp_ArtNet_Universe;		// ArtNet Universe set in module
unsigned int			Eth_Universes_ToChange;			// Universes or Enabled changed - write to module

// Local Variables
unsigned int			eth_CharsToTransmit;		// num of chars to transmit
unsigned char			eth_RxDataBuff[16];			// registers returned from Ethernet Module
int						eth_RxDataCnt;				// number of rx data in buffer
unsigned char			eth_TxDataBuff[16];			// registers sent to Ethernet Module
unsigned short *		eth_TxDataBuffPtr;			// pointer to data buffer
int						eth_NeedToProcessRecData;	// after finish sending packet
int 					eth_ModulePresentTimer;		// delay off module
unsigned int			eth_IssuedCommand;			// issued command (only MSB)
unsigned int			eth_LiveDataOutCtc;			// Helper for switching live data Fuse/Bulb
unsigned int			eth_IssuedSetup;			// Circular setup is sent to EthSpi module

unsigned int			WriteToFlash_UserEthernet;		// Writing to FLASH activated by user on DC12

// Constants
const char TextSm_sACN[] = 		"\x65""AC\x64";
const char TextSm_ArtNet[] =	"A\x66\x67\x64";
const char TextSm_NoNet[] =		"    ";
const char TextDeviceName[] = 	"DC12";
const char _0F_chars[20] = 		"0123456789ABCDEF";


#define ETHERNET_STS_CLEAR_BITS_NO_MODULE		(ETHERNET_STS_PLUGIN_MODULE | ETHERNET_STS_LINK | ETHERNET_STS_PROTOCOL | ETHERNET_STS_DMX_OK)
#define ETHERNET_ENABLED						(EthernetStatus | (ETHERNET_STS_ENABLED_SACN | ETHERNET_STS_ENABLED_ARTNET))
#define ETHERNET_FRAME_SIZE						16

#define ETH_COMM_READ_CONFIGURATION_02			0x02
#define ETH_COMM_READ_CONFIGURATION_03			0x03
#define ETH_COMM_READ_CONFIGURATION_04			0x04
#define ETH_COMM_WRITE_CONFIGURATION_82			0x82
#define ETH_COMM_WRITE_CONFIGURATION_83			0x83
#define ETH_COMM_WRITE_CONFIGURATION_84			0x84
#define ETH_COMM_WRITE_CONFIGURATION_88			0x88	// Host Name
#define ETH_COMM_WRITE_CONFIGURATION_89			0x89	// Host Version
#define ETH_COMM_WRITE_LIVE_OUT_DATA_A0			0xA0	// LIVE OUTPUT DATA + Fuse/Bulb Status
#define ETH_COMM_WRITE_LIVE_STS_DATA_A1			0xA1	// LIVE DC12 STATUS
#define ETH_COMM_WRITE_OUT_SETUP_DMX_B0			0xB0	// SETUP DMX
#define ETH_COMM_WRITE_OUT_SETUP_MIN_B1			0xB1	// SETUP MIN
#define ETH_COMM_WRITE_OUT_SETUP_MAX_B2			0xB2	// SETUP MAX
#define ETH_COMM_WRITE_OUT_SETUP_CRV_B3			0xB3	// SETUP CRV = Curves
#define ETH_COMM_WRITE_OUT_SETUP_ADV_B4			0xB4	// SETUP ADV
#define ETH_COMM_WRITE_OUT_SETUP_UNT_B5			0xB5	// SETUP UNT
#define ETH_COMM_WRITE_OUT_SETUP_HWV_B6			0xB6	// SETUP DIMMER HW VERSION
#define ETH_COMM_WRITE_OUT_SETUP_FWV_B7			0xB7	// SETUP DIMMER HW VERSION

// Ethernet Module Status Bits
#define EMSB_PHY_LINK							0x04
#define EMSB_PHY_SPEED							0x08
#define EMSB_PHY_DUPLEX							0x10
#define EMSB_UDP_PRESENT						0x40
#define EMSB_PROTOCOL_PRESENT					0x80
#define EMSB_LIVE_DATA_NEEDED					0x8000

// Control PINs
#define ETHERNET_IRQ_PIN						PORTC_PCR15
#define ENABLE_ETHERNET_INTERRUPT				ETHERNET_IRQ_PIN = PORT_GPIO_ENABLE_INTERRUPT_F
#define DISABLE_ETHERNET_INTERRUPT				ETHERNET_IRQ_PIN = PORT_GPIO_DISABLE_INTERRUPT

void SetVariablesForDisplay(void)
{
	if (eth_ModulePresentTimer) eth_ModulePresentTimer--;

	Eth_EthernetStatus = Eth_ETH_Enabled;
	if (Eth_ETH_Enabled & ETHERNET_STS_ENABLED_SACN)
			DispEthernetActiveTxt = TextSm_sACN;
	else
	if (Eth_ETH_Enabled & ETHERNET_STS_ENABLED_ARTNET)
			DispEthernetActiveTxt = TextSm_ArtNet;
	else	DispEthernetActiveTxt = TextSm_NoNet;

	if (!eth_ModulePresentTimer)
	{		// Module is not present action
			Eth_EthernetStatus &= ~ETHERNET_STS_CLEAR_BITS_NO_MODULE;
			strcpy(Eth_Mod_HwVerTxt, NO_VERSION);
			strcpy(Eth_Mod_FwVerTxt, NO_VERSION);
			eth_IssuedCommand = ETH_COMM_READ_CONFIGURATION_04;
	}
	else	// Module is present -> check other properties
	{
			Eth_EthernetStatus |= ETHERNET_STS_PLUGIN_MODULE;
			if (Eth_ModuleStatusBits & EMSB_PHY_LINK) Eth_EthernetStatus |= ETHERNET_STS_LINK;
			if (Eth_ModuleStatusBits & EMSB_PROTOCOL_PRESENT) Eth_EthernetStatus |= ETHERNET_STS_PROTOCOL;
			if ((Eth_ModuleStatusBits >> 8) & Eth_EthernetStatus)
			{
				Eth_EthernetStatus |= ETHERNET_STS_DMX_OK;
			}
	}

	// To enable/disable UART
	if (Eth_EthernetStatus & ETHERNET_STS_DMX_OK)
	{
			if (!Uart_ETHERNET_Enabled())
					Initialization_Uart_ETHETNET();
	}
	else
	{
			if (Uart_ETHERNET_Enabled())
					Disable_Uart_ETHERNET();
	}
}

/* DIMMER OUTPUTS
 * FUSE STATUS
 * BULB STATUS
 * PWR DIMMER OK
 */

#ifdef NO_ETHERNET_FUNCTIONS_SO_FAR
unsigned char __fill_dimmers_live_data_A0(void)
{
	unsigned char * tx = (unsigned char *)&eth_TxDataBuff[1];
	unsigned int bits23 = 0;
	unsigned int bits21 = 0;
	unsigned int mask = 0x000001;
	unsigned char v;
	for (int i=0; i<SERIAL_DIMMERS_CNT; i++)
	{
		if (SerialDimmer[i].DimRuntime[0] & SDS_STATUS_WORD_OK) bits21 |= mask;
		if (SerialDimmer[i].DimRuntime[1] & SDS_BALLAST_PRESENT) bits23 |= mask;
		v = DmxValue[i].OutputFinish*100;
		if (v > 100) v = 100;
		if (SerialDimmer[i].DimRuntime[1] & SDS_LINE_FUSE_OK) v |= 0x80;
		if (!(i&1)) tx[1] = v; else tx[-1] = v;
		tx++;
		mask <<= 1;
	}
	eth_TxDataBuff[0] = bits23 + ((bits21 & 0xF00) << 4);
	return bits21 & 0xFF;
}

/* DC12 DMX REC STATUS
 *
 */

unsigned char __fill_dimmers_status_data_A1(void)
{
	eth_TxDataBuff[0] = 0;
	eth_TxDataBuff[1] = GlobalDmxPresent;
	return 0;
}

/* setup data
 *
 * 0xB0 = dimmers dmx channels
 * 0xB1 = dimmers minimum
 * 0xB2 = dimmers maximum
 * 0xB3 = dimmers curve
 * 0xB4 = dimmers advanced
 * 0xB5 = unit settings
 *
 * returns 1. byte in header
 */

unsigned char __fill_dimmers_setup_data_Bx(void)
{
	unsigned char * tx = (unsigned char *)&eth_TxDataBuff[1];
	unsigned short bits8 = 0;
	unsigned short bits9 = 0;
	unsigned int bit_mask = 1;
	unsigned int v;

	for (int i=0; i<SERIAL_DIMMERS_CNT; i++)
	{
		switch (eth_IssuedSetup)
		{
		case 0:	// ----> DMX patch setup
				v = Flash_Patch[i]-1;
				break;
		case 1:	// ----> Dimmers Minimum
				v = Flash_DimmerSettings[i].Minimum;
				break;
		case 2:	// ----> Dimmers Maximum
				v = Flash_DimmerSettings[i].Maximum-500;
				break;
		case 3:	// ----> Dimmers Curve
				v = Flash_DimmerSettings[i].Curve & 0xFF;
				break;
		case 4:	// ----> Dimmers Advanced
				v = Flash_DimmerSettings[i].WorkL_ZiZ + (Flash_DimmerSettings[i].Ts_Ru << 2);
				break;
		case 5:	// ----> Unit Settings
				switch (i){
				case 0 : v = Flash_UnitSettings.WorklightEnabled + (Flash_UnitSettings.HoldLast << 1); break;
				default : v = 0;
				}
				break;
		case 6:	// ----> Dimmers HW Version
				v = SerialDimmer[i].Hw;
				break;
		case 7:	// ----> Dimmers FW Version
				v = SerialDimmer[i].Fw;
				break;
		}
		if (!(i&1)) tx[1] = v & 0xFF; else tx[-1] = v & 0xFF;
		tx++;
		if (v & 0x100) bits8 |= bit_mask;
		if (v & 0x200) bits9 |= bit_mask;
		bit_mask <<= 1;
	}

	eth_TxDataBuff[0] = bits8 + ((bits9 & 0xF00) << 4);;
	eth_IssuedCommand = eth_IssuedCommand  + (eth_IssuedSetup << 8);

	// at next send, different data
	if (++eth_IssuedSetup >= 8) eth_IssuedSetup = 0;
	return bits9 & 0xFF;
}

/* Last communication RX frame
 *
 */

const char _0F_chars[20] = "0123456789ABCDEF";

#endif

void CommunicationRxManager(void)
{
	if (eth_RxDataCnt < ETHERNET_FRAME_SIZE) return;
	if ((eth_RxDataBuff[1] & 0x0003) != 1) return;

	eth_ModulePresentTimer = 100;	// 40ms interval = 1 sec.
	Eth_ModuleStatusBits = eth_RxDataBuff[1] + (eth_RxDataBuff[0] << 8);

	switch (eth_IssuedCommand)
	{
	case ETH_COMM_READ_CONFIGURATION_02:	// Enabled + Universes
			Eth_ETH_Enabled = eth_RxDataBuff[2] & 3;
			Eth_ArtNet_Universe = (eth_RxDataBuff[4] << 8) + eth_RxDataBuff[5];
			Eth_sACN_Universe = (eth_RxDataBuff[6] << 8) + eth_RxDataBuff[7];
			eth_IssuedCommand = ETH_COMM_WRITE_LIVE_OUT_DATA_A0;
			Eth_Universes_ToChange = 0;
			break;
	case ETH_COMM_READ_CONFIGURATION_03:	// IP Address
			Eth_IP_Address = __REV(*(unsigned int *)&eth_RxDataBuff[2]);
			eth_IssuedCommand = ETH_COMM_READ_CONFIGURATION_02;
			break;
	case ETH_COMM_READ_CONFIGURATION_04:	// MAC, HW, FW spi modulu
			Eth_MAC_Address[0] = *(unsigned int *)&eth_RxDataBuff[2];
			Eth_MAC_Address[1] = *(unsigned short *)&eth_RxDataBuff[6];
			Eth_Mod_HwVerTxt[0] = 'H';
			Eth_Mod_HwVerTxt[1] = '0'+ ((eth_RxDataBuff[11] & 0x00f0) >> 4);
			Eth_Mod_HwVerTxt[2] = '0'+ ((eth_RxDataBuff[11] & 0x000f) >> 0);
			Eth_Mod_HwVerTxt[3] = 'A'+ (eth_RxDataBuff[10]);
			Eth_Mod_HwVerTxt[4] = 0;
			Eth_Mod_FwVerTxt[0] = 'F';
			Eth_Mod_FwVerTxt[1] = _0F_chars[(eth_RxDataBuff[13] & 0x000F)];
			Eth_Mod_FwVerTxt[2] = _0F_chars[(eth_RxDataBuff[12] & 0xf0)>>4];
			Eth_Mod_FwVerTxt[3] = _0F_chars[(eth_RxDataBuff[12] & 0x0f)];
			Eth_Mod_FwVerTxt[4] = 0;
			eth_IssuedCommand = ETH_COMM_READ_CONFIGURATION_03;
			break;
	case ETH_COMM_WRITE_CONFIGURATION_82:	// Enabled + Universes
			eth_IssuedCommand = ETH_COMM_READ_CONFIGURATION_02;
			Eth_Universes_ToChange = 0;
			break;
	case ETH_COMM_WRITE_CONFIGURATION_83:	// IP Address
			eth_IssuedCommand = ETH_COMM_READ_CONFIGURATION_03;
			Eth_IP_Address_ToChange = 0;
			break;
	case ETH_COMM_WRITE_CONFIGURATION_88:	// HOST NAME
			eth_IssuedCommand = ETH_COMM_READ_CONFIGURATION_04;
			break;
	case ETH_COMM_WRITE_CONFIGURATION_89:	// HW + FW DC12
			eth_IssuedCommand = ETH_COMM_WRITE_CONFIGURATION_88;
			break;
#ifdef NO_ETHERNET_FUNCTIONS_SO_FAR
	case ETH_COMM_WRITE_LIVE_OUT_DATA_A0:
	case ETH_COMM_WRITE_LIVE_STS_DATA_A1:
			Eth_ReceivedFPS = eth_RxDataBuff[1];
			Eth_AcceptedFPS = eth_RxDataBuff[2];
			eth_IssuedCommand = ETH_COMM_WRITE_LIVE_OUT_DATA_A0;
			if (!(Eth_ModuleStatusBits & EMSB_LIVE_DATA_NEEDED))
				eth_IssuedCommand = ETH_COMM_WRITE_CONFIGURATION_89;
			else if (!(eth_LiveDataOutCtc % 6))
				eth_IssuedCommand = ETH_COMM_WRITE_LIVE_STS_DATA_A1;
			else if (!(eth_LiveDataOutCtc % 20))
				eth_IssuedCommand = ETH_COMM_WRITE_OUT_SETUP_DMX_B0;
			break;
	case ETH_COMM_WRITE_OUT_SETUP_DMX_B0:
	case ETH_COMM_WRITE_OUT_SETUP_MIN_B1:
	case ETH_COMM_WRITE_OUT_SETUP_MAX_B2:
	case ETH_COMM_WRITE_OUT_SETUP_CRV_B3:
	case ETH_COMM_WRITE_OUT_SETUP_ADV_B4:
	case ETH_COMM_WRITE_OUT_SETUP_UNT_B5:
	case ETH_COMM_WRITE_OUT_SETUP_HWV_B6:
	case ETH_COMM_WRITE_OUT_SETUP_FWV_B7:
			eth_IssuedCommand = ETH_COMM_WRITE_LIVE_OUT_DATA_A0;
			break;
	default:
			eth_IssuedCommand = ETH_COMM_WRITE_LIVE_OUT_DATA_A0;
#endif
	}
	if (Eth_IP_Address_ToChange)
	{
		eth_IssuedCommand = ETH_COMM_WRITE_CONFIGURATION_83;
	}
	if (Eth_Universes_ToChange)
	{
		eth_IssuedCommand = ETH_COMM_WRITE_CONFIGURATION_82;
	}
}

//unsigned short __swap_short(unsigned short s)
//{
//	return (s>>8) + (s<<8);
//}

//void __fill_tx_buffer(const char * d)
//{
//	int i;
//
//	eth_TxDataBuff[0] = 0;
//	eth_TxDataBuff[1] = 0;
//	eth_TxDataBuff[2] = 0;
//	eth_TxDataBuff[3] = 0;
//	eth_TxDataBuff[4] = 0;
//	eth_TxDataBuff[5] = 0;
//	eth_TxDataBuff[6] = 0;
//	strcpy((char *)eth_TxDataBuff, d);
//	for (i=0; i<7; i++)
//		eth_TxDataBuff[i] = __swap_short(eth_TxDataBuff[i]);
//}

void SPI2_IRQHandler(void)
{
	SPI2->IFCR = SPI_IFCR_EOTC;
	int l = 16;
	unsigned char * rx = eth_RxDataBuff;
	eth_RxDataCnt = 0;
	while ((SPI2->SR & SPI_SR_RXP) && (l--))
	{
		*rx++ = *(unsigned char*)&SPI2->RXDR;
		eth_RxDataCnt++;
	}
}

#define SPI2__STOP								SPI2->CR1 = 0;
#define SPI2__START								SPI2->CR1 = SPI_CR1_SPE;
#define SPI2__CSTART							SPI2->CR1 = SPI_CR1_SPE + SPI_CR1_CSTART;

void __start_spi_ethernet_transmision(void)
{
	SPI2__STOP;
	SPI2->CR2 = ETHERNET_FRAME_SIZE;
	SPI2->IFCR = SPI_IFCR_TXTFC | SPI_IFCR_EOTC;
	SPI2__START;
	SPI2__CSTART;
	for (int i=0; i<16; i++)
		*(char *)&SPI2->TXDR = eth_TxDataBuff[i];
	SPI2->IER = SPI_IER_EOTIE;// | SPI_IER_RXPIE;
}

void CommunicationTxManager(void)
{
	switch (eth_IssuedCommand)
	{
	case ETH_COMM_READ_CONFIGURATION_02:
	case ETH_COMM_READ_CONFIGURATION_03:
	case ETH_COMM_READ_CONFIGURATION_04:
			memset(eth_TxDataBuff, 1, sizeof(eth_TxDataBuff));
			break;
	case ETH_COMM_WRITE_CONFIGURATION_82:
			if (Eth_Disp_ETH_Enabled >= 3) Eth_Disp_ETH_Enabled = ETHERNET_STS_ENABLED_ARTNET;
			eth_TxDataBuff[2] = Eth_Disp_ETH_Enabled;
			eth_TxDataBuff[3] = 0;
			*(unsigned short*)&eth_TxDataBuff[4] = __REV16(Eth_Disp_ArtNet_Universe);
			*(unsigned short*)&eth_TxDataBuff[6] = __REV16(Eth_Disp_sACN_Universe);
			eth_TxDataBuff[8] = 0;
			break;
	case ETH_COMM_WRITE_CONFIGURATION_83:
			*(unsigned int *)&eth_TxDataBuff[2] = __REV(Eth_IP_Address_ToChange);
			eth_TxDataBuff[6] = 0;
			break;
	case ETH_COMM_WRITE_CONFIGURATION_88:
//			__fill_tx_buffer(TextDeviceName);
			break;
	case ETH_COMM_WRITE_CONFIGURATION_89:
//			__fill_tx_buffer(HARDWARE_VERSION FIRMWARE_VERSION);
			break;
	case ETH_COMM_WRITE_LIVE_OUT_DATA_A0:
			eth_LiveDataOutCtc++;
//			first_byte = __fill_dimmers_live_data_A0();
			break;
	case ETH_COMM_WRITE_LIVE_STS_DATA_A1:
			eth_LiveDataOutCtc++;
//			first_byte = __fill_dimmers_status_data_A1();
			break;
	case ETH_COMM_WRITE_OUT_SETUP_DMX_B0:
	case ETH_COMM_WRITE_OUT_SETUP_MIN_B1:
	case ETH_COMM_WRITE_OUT_SETUP_MAX_B2:
	case ETH_COMM_WRITE_OUT_SETUP_CRV_B3:
	case ETH_COMM_WRITE_OUT_SETUP_ADV_B4:
	case ETH_COMM_WRITE_OUT_SETUP_UNT_B5:
	case ETH_COMM_WRITE_OUT_SETUP_HWV_B6:
	case ETH_COMM_WRITE_OUT_SETUP_FWV_B7:
//			first_byte = __fill_dimmers_setup_data_Bx();
			break;
	default:
			eth_IssuedCommand = ETH_COMM_WRITE_LIVE_STS_DATA_A1;
//			first_byte = __fill_dimmers_status_data_A1();
	}
	eth_TxDataBuff[0] = eth_IssuedCommand;
	eth_TxDataBuff[1] = 0x00;
	__start_spi_ethernet_transmision();
}

void PluginModuleHandle_ETHERNET(void)
{
	CommunicationRxManager();	// Vyhodnoti se prijaty frame z minule komunikace
	SetVariablesForDisplay();
	CommunicationTxManager();	// Vydana nova komunikace pres SPI
}

/* ---------------------------------------------------
 * Nastaveni SPI pro komunikaci s ETHERNETOVYM MODULEM
 * ---------------------------------------------------
 * Ethernetovy modul nepouziva SPI, softwarove pomoci preruseni na CS a SCK portu dela SPI.
 * Proto komunikace neni nijak rychla, nicmene pro tuto aplikaci to bohate staci.
 * Latence od preruseni k reakci je cca 1 us.
 * Zvolil jsem proto hodinovy kmitocet SCK na cca 39062.5 Hz (25.6 us Perioda),
 * zmena (nahoru, nebo dolu) kazdych 12.8 us.
 * CS je rizene modulem SPI a je o 1 SCK pred prvni hranou SCK, to je cca 12.8 us.
 * Hodiny v idle stavu jsou 1.
 * Neaktivni CS je 1
 * Data se meni na sestupne hrane, ctou se na nabezne hrane.
 * CS se ovlada z SPI modulu SPI2.NSS.
 *
 * Vysila/prijima se 16 x 8 bitu.
 * Prvni byt je zadani adresy, druhy nula, a vraci se status byte 0 a 1.
 *
 * Clock:
 * 		- input clock: pll1_q_ck = 10 MHz
 * 		- MBR divisor: 256
 * 		- resulting SPI clock: 39062.5 Hz
 * */
void Initialization_ETHERNET_SSI(void)
{
	RCC->APB1LENR |= RCC_APB1LENR_SPI2EN;
	(void)RCC->APB1LENR;
//	RCC->CCIPR3 &= ~RCC_CCIPR3_SPI2SEL_Msk;
//	RCC->CCIPR3 |= 1 << RCC_CCIPR3_SPI2SEL_Pos;		// pll2_q_ck selected as kernel clock for SPI2
	eth_IssuedSetup = 0;
	eth_ModulePresentTimer = 2;
	memset(eth_RxDataBuff, 0, sizeof(eth_RxDataBuff));
	SPI2->CFG1 = (7 << SPI_CFG1_MBR_Pos) |
				 (7 << SPI_CFG1_DSIZE_Pos) |
				 0;
	SPI2->CFG2 = SPI_CFG2_MASTER |
				 SPI_CFG2_SSOE |
				 SPI_CFG2_CPOL |
				 SPI_CFG2_CPHA |
				 (3 << SPI_CFG2_MIDI_Pos) |	// delay between two bytes
				 0;
	NVIC->ISER[1] |= (1<<(SPI2_IRQn-32));				// Enable INTERRUPT in NVIC
	eth_IssuedCommand = ETH_COMM_READ_CONFIGURATION_04;
	Eth_IP_Address =
	Eth_ETH_Enabled =
	Eth_sACN_Universe =
			Eth_ArtNet_Universe =
	Eth_IP_Address_ToChange =
	Eth_Universes_ToChange = 0;
}

/* End of the file dc48_Ethernet.c */

