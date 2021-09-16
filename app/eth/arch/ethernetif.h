////////////////////////////////////////////////////////////////////////////////
/// @file   ethernetif.h
/// @author AE TEAM
/// @brief  THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE
///         ethernetif.h EXAMPLES.
/// ////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2021 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

#ifndef __ETHERNETIF_H
#define __ETHERNETIF_H

#include "lwip/err.h"

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Application_Layer
/// @brief MM32 Application Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Application_Ethernet_Interface
/// @brief Application Ethernet Interface Module
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Ethernet_Interface_Exported_Constants
/// @{

///  Network Interface MTU
#define NETIF_MTU                       1500

/// Network Interface Name
#define IFNAME0                         'm'
#define IFNAME1                         'm'







/// Static IP ADDRESS: 10.3.1.222
#define IP_ADDR0                        10
#define IP_ADDR1                        3
#define IP_ADDR2                        1
#define IP_ADDR3                        222

/// NETMASK: 
#define NETMASK_ADDR0                   255
#define NETMASK_ADDR1                   255
#define NETMASK_ADDR2                   254
#define NETMASK_ADDR3                   0

/// Gateway Address
#define GW_ADDR0                        10
#define GW_ADDR1                        3
#define GW_ADDR2                        0
#define GW_ADDR3                        1

/// MAC Address
#define MAC_ADDR0                       2
#define MAC_ADDR1                       0
#define MAC_ADDR2                       0
#define MAC_ADDR3                       0
#define MAC_ADDR4                       2
#define MAC_ADDR5                       2










/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Ethernet_Interface_Exported_Enumeration
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Ethernet_Interface_Exported_Variables
/// @{

#ifdef _ETHERNETIF_C_
#define GLOBAL

GLOBAL uint8_t gMac[6] = {02, 00, 00, 00, 00, 02};

#else
#define GLOBAL extern

GLOBAL uint8_t gMac[6];

#endif

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Ethernet_Interface_Exported_Functions
/// @{

err_t ethernetif_init(struct netif* netif);

void ethernetif_input(struct netif *netif);
void ethernetif_output( void *argument );
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
