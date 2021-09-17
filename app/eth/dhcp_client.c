////////////////////////////////////////////////////////////////////////////////
/// @file     dhcp_client.c
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  v2.0.0
/// @date     2019-08-12
/// @brief    THIS FILE PROVIDES ALL THE dhcp_client.c EXAMPLE.
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
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

#define _DHCP_CLIENT_C_
#include <string.h>
#include "mm32_types.h"
#include "lwip/dhcp.h"

#include "main.h"
#include "eth.h"
#include "dhcp_client.h"

void dhcp_pre_check(struct netif* netif)
{
#if LWIP_DHCP
    if (netif_is_up(netif)) {
        dhcp_status = DHCP_START;
    }
    else {
        dhcp_status = DHCP_CLOSE;
    }
#endif
}

void dhcp_process(struct netif* netif)
{
#if LWIP_DHCP
    ip_addr_t    ipaddr;
    ip_addr_t    netmask;
    ip_addr_t    gw;
    struct dhcp* dhcp;

    switch (dhcp_status) {
        case DHCP_START:
            dhcp_release_and_stop(netif);
            ip_addr_set_zero_ip4(&netif->ip_addr);
            ip_addr_set_zero_ip4(&netif->netmask);
            ip_addr_set_zero_ip4(&netif->gw);
            dhcp_status = DHCP_PROCESSING;
            dhcp_start(netif);
            break;

        case DHCP_PROCESSING:
            if (dhcp_supplied_address(netif)) {
                dhcp_status = DHCP_SUCCESS;
                gGetIPFlag  = 1;
            }
            else {
                dhcp = (struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

                if (dhcp->tries > DHCP_TRIES_MAX) {  // Retry times > 4

                    dhcp_status = DHCP_FAILED;

                    dhcp_stop(netif);

                    IP4_ADDR(&ipaddr, gEthernetConfig.ip[0], gEthernetConfig.ip[1], gEthernetConfig.ip[2], gEthernetConfig.ip[3]);
                    IP4_ADDR(&netmask, gEthernetConfig.mask[0], gEthernetConfig.mask[1], gEthernetConfig.mask[2],
                             gEthernetConfig.mask[3]);
                    IP4_ADDR(&gw, gEthernetConfig.gw[0], gEthernetConfig.gw[1], gEthernetConfig.gw[2], gEthernetConfig.gw[3]);

                    netif_set_addr(netif, &ipaddr, &netmask, &gw);
                }
            }
            break;

        case DHCP_CLOSE:
            dhcp_stop(netif);
            dhcp_status = DHCP_OFF;
            break;

        default:  // SUCCESS, FAILED, OFF
            break;
    }
#endif
}

void dhcp_tick_check(struct netif* netif, uint32_t cnt)
{
#if LWIP_DHCP
    if (cnt - dhcp_user_fine_tmr > DHCP_FINE_TIMER_MSECS) {
        dhcp_user_fine_tmr = cnt;
        dhcp_process(netif);
    }
    if (gGetIPFlag) {
        rf.eth     = true;
        gGetIPFlag = 0;
        gIPAddr    = getIpAddress();
    }
#endif
}