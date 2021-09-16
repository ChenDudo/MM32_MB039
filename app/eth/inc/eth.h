////////////////////////////////////////////////////////////////////////////////
/// @file     ETH.H
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  1.0.0
/// @date     2021-04-21
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE ETH
///           EXAMPLES.
////////////////////////////////////////////////////////////////////////////////
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

// Define to prevent recursive inclusion  --------------------------------------
#ifndef __ETH_H
#define __ETH_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Application_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Application_Ethernet
/// @brief Ethernet Application modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Ethernet_Exported_Constants
/// @{


typedef struct {
    uint8_t ip[4];
    uint8_t mask[4];
    uint8_t gw[4];
    uint8_t mac[6];
} InitEthernet_Typedef;


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Ethernet_Exported_Variables
/// @{

#ifdef _ETH_C_
#define GLOBAL

GLOBAL uint32_t gLwipCounter = 0;

#else
#define GLOBAL extern
#endif

GLOBAL struct netif gnetif;
GLOBAL uint32_t gLwipCounter;
GLOBAL InitEthernet_Typedef gEthernetConfig;
GLOBAL uint32_t gIPAddr;
GLOBAL char enableETH, enableOK, retryEth;
#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup Ethernet_Exported_Functions
/// @{

void InitEthernet(InitEthernet_Typedef config);
uint32_t getIpAddress(void);
void getMacAddress(uint8_t *mac);
void BSP_HTTP_Configure(void);

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __ETH_H */
////////////////////////////////////////////////////////////////////////////////
