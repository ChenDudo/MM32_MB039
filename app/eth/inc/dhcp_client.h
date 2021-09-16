////////////////////////////////////////////////////////////////////////////////
/// @file     dhcp_client.h
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  v2.0.0
/// @date     2019-08-12
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE dhcp_client.h EXAMPLES.
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

#ifndef __DHCP_CLIENT_H
#define __DHCP_CLIENT_H

typedef enum {
    DHCP_OFF = 0,
    DHCP_START,
    DHCP_PROCESSING,
    DHCP_SUCCESS,
    DHCP_FAILED,
    DHCP_CLOSE
} DHCP_STATUS;

#define DHCP_TRIES_MAX  4   // retry times

#ifdef _DHCP_CLIENT_C_
#define GLOBAL

#else
#define GLOBAL extern
#endif

GLOBAL DHCP_STATUS dhcp_status;
GLOBAL uint32_t dhcp_user_fine_tmr;
GLOBAL uint8_t gGetIPFlag;


#undef GLOBAL

void dhcp_pre_check(struct netif *netif);
void dhcp_tick_check(struct netif *netif, uint32_t cnt);

#endif
