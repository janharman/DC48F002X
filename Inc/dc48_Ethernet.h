/*
 * dc48_Ethernet.h
 *
 *  Created on: 5. 2. 2026
 *      Author: harma
 */

#ifndef INC_DC48_ETHERNET_H_
#define INC_DC48_ETHERNET_H_

#define ETHERNET_STS_ENABLED_ARTNET		1
#define ETHERNET_STS_ENABLED_SACN		2
#define ETHERNET_STS_PLUGIN_MODULE		4
#define ETHERNET_STS_LINK				8
#define ETHERNET_STS_PROTOCOL			16
#define ETHERNET_STS_DMX_OK				32
#define ETHERNET_STS_ENABLED			(ETHERNET_STS_ENABLED_SACN | ETHERNET_STS_ENABLED_ARTNET)

extern unsigned int 		Eth_ModuleStatusBits;			// Status of the module read from the module by SPI
extern int 					Eth_EthernetStatus;				// Total Status
extern unsigned int			Eth_IP_Address;					// Ip adresa ETH module
extern unsigned int			Eth_MAC_Address[2];				// MAC address ETH module
extern char					Eth_Mod_HwVerTxt[8];
extern char					Eth_Mod_FwVerTxt[8];
extern unsigned int			Eth_ReceivedFPS;				// Received Frames Per Second
extern unsigned int			Eth_AcceptedFPS;				// Accepted Frames Per Second
extern unsigned int			Eth_DisplayIpAddress[4];		// Ip adresa pro display a zmenu
extern unsigned int			Eth_ETH_Enabled;				// sACN[1] or ArtNet[0] Enabled set in module
extern unsigned int			Eth_sACN_Universe;				// sACN Universe set in module
extern unsigned int			Eth_ArtNet_Universe;			// ArtNet Universe set in module
extern unsigned int			Eth_IP_Address_ToChange;		// IP address for save to ETH module
extern unsigned int			Eth_Disp_ETH_Enabled;			// sACN[1] or ArtNet[0] Enabled set in module
extern unsigned int			Eth_Disp_sACN_Universe;			// sACN Universe set in module
extern unsigned int			Eth_Disp_ArtNet_Universe;		// ArtNet Universe set in module
extern unsigned int			Eth_Universes_ToChange;			// Universes or Enabled changed - write to module

void PluginModuleHandle_ETHERNET(void);
void Initialization_ETHERNET_SSI(void);

#endif /* INC_DC48_ETHERNET_H_ */
