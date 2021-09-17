////////////////////////////////////////////////////////////////////////////////
/// @file     ETH.C
/// @author   D CHEN
/// @version  v1.0.0
/// @date     2021-04-21
/// @brief    THIS FILE PROVIDES ALL THE ETH EXAMPLE.
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
#define _ETH_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"
#include "common.h"

#include "netif/ethernet.h"
#include "lwip/arch.h"
#include "lwip/dhcp.h"
#include "lwip/opt.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/apps/httpd.h"

#include "ethernetif.h"

#include "main.h"
#include "eth.h"
#include "tcp_server.h"
#include "http_server.h"
#include "dhcp_client.h"
#include "hal_gpio.h"
#include "hal_eth.h"

#include "bsp_eth.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Application_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Application_Ethernet
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Ethernet_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Ethernet receive callback function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ETH_RxCpltCallback(void)
{
    ethernetif_input(&gnetif);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Ethernet interrupt request handler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ETH_IRQHandler(void)
{
    if (ETH_GetDMAITStatus(ETH_DMA_IT_R)) {
        ETH_RxCpltCallback();
        ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
        error.eth = false;
		rfOverCnt.eth = 0;
    } else if (ETH_GetDMAITStatus(ETH_DMA_IT_T)) {
        ETH_DMAClearITPendingBit(ETH_DMA_IT_T);
    } else if (ETH_GetDMAITStatus(ETH_DMA_IT_TBU)) {
        ETH_DMAClearITPendingBit(ETH_DMA_IT_TBU);
    }

    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

    if (ETH_GetDMAITStatus(ETH_DMA_FLAG_AIS)) {
        ETH_DMAClearITPendingBit(ETH_DMA_FLAG_AIS);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Ethernet init function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void InitEthernet(InitEthernet_Typedef config)
{
    memcpy(&gEthernetConfig, &config, sizeof(InitEthernet_Typedef));

    ip4_addr_t ipaddr;
    ip4_addr_t netmask;
    ip4_addr_t gw;

    // IP addresses initialization
#if LWIP_DHCP
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
#else
    IP4_ADDR(&ipaddr, config.ip[0], config.ip[1], config.ip[2], config.ip[3]);
    IP4_ADDR(&netmask, config.mask[0], config.mask[1], config.mask[2], config.mask[3]);
    IP4_ADDR(&gw, config.gw[0], config.gw[1], config.gw[2], config.gw[3]);
#endif  // USE_DHCP

    memcpy(gMac, config.mac, 6);

    lwip_init();

    // Initilialize the LwIP stack without RTOS add the network interface (IPv4/IPv6) without RTOS
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    netif_set_default(&gnetif);  // Registers the default network interface

    if (netif_is_link_up(&gnetif)) {
        netif_set_up(&gnetif);  // When the netif is fully configured this
        // function must be called
    } else {
        netif_set_down(&gnetif);  // When the netif link is down this function
        // must be called
    }
}

////////////////////////////////////////////////////////////////////////////////
uint32_t getIpAddress(void)
{
    return gnetif.ip_addr.addr;
}

void getMacAddress(uint8_t * mac)
{
    memcpy(mac, gnetif.hwaddr, sizeof(uint8_t) * 6);
}

u32_t sys_now(void)
{
    return gLwipCounter;
}

////////////////////////////////////////////////////////////////////////////////
void BSP_HTTP_Configure(void)
{
    rf.eth = true;

    // Set random seed by Device ID.
    srand(*(unsigned int *)(0x1FFFF7E8));

    // Initialization Ethernet.
    InitEthernet_Typedef ethConfig = {
        .ip     = { 192,  168,    1,     1},
        .gw     = { 192,  168,    1,     1},
        .mask   = { 255,  255,  255,     0},
        .mac    = {(uint16_t)rand(), (uint16_t)rand(), (uint16_t)rand(),
        (uint16_t)rand(), (uint16_t)rand(), (uint16_t)rand()}
    };

    // Mac multicast bit must be 0.
    ethConfig.mac[0] &= 0xFE;

    BSP_ETH_Configure();

    if(EthErr < 0)
        return;

    InitEthernet(ethConfig);

#if LWIP_DHCP
    dhcp_pre_check(&gnetif);
#endif

    // Initialization TCP Server (port 5001).
    InitTcpServer(5001);

    // Initialization HTTP Server (port 80).
    httpd_init();

    enableOK = true;
}


/// @}

/// @}

/// @}
