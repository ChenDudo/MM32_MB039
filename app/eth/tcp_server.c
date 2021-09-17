////////////////////////////////////////////////////////////////////////////////
/// @file     tcp_server.c
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  1.0.0
/// @date     2021-04-21
/// @brief    THIS FILE PROVIDES ALL THE tcp_server EXAMPLE.
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
#define _TCP_SERVER_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"

#include "eth.h"
#include "tcp_server.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Appliaction_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Appliaction_TCP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TCP_Server_Functions
/// @{

void SendTcpPackage(char* data, uint16_t length)
{
    extern struct tcp_pcb* tcp_active_pcbs;
    tcp_write(tcp_active_pcbs, data, length, TCP_WRITE_FLAG_COPY);
    memset(data, 0, length);
}

static err_t ReceivedTcpPcbCallback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err)
{
    if (p != NULL) {
        // Received data
        tcp_recved(tpcb, p->tot_len);

        memset(p->payload, 0, p->tot_len);

        pbuf_free(p);
    }
    else {
        return tcp_close(tpcb);
    }

    return ERR_OK;
}

static err_t AccpectNewLinkCallback(void* arg, struct tcp_pcb* new_pcb, err_t err)
{
    tcp_recv(new_pcb, ReceivedTcpPcbCallback);

    return ERR_OK;
}

void InitTcpServer(uint16_t port)
{
    struct tcp_pcb* pcb = NULL;

    // New TCP protocol control block
    pcb = tcp_new();

    if (pcb != NULL) {
        // Bind TCP to 0.0.0.0:port
        tcp_bind(pcb, IP_ADDR_ANY, port);

        // Listen start
        pcb = tcp_listen(pcb);

        // Process link
        tcp_accept(pcb, AccpectNewLinkCallback);
    }
}

/// @}

/// @}

/// @}
