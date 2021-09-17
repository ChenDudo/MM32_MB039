////////////////////////////////////////////////////////////////////////////////
/// @file     udp_server.c
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  1.0.0
/// @date     2021-04-21
/// @brief    THIS FILE PROVIDES ALL THE udp_server EXAMPLE.
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
#define _UDP_SERVER_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sys.h"

#include "eth.h"
#include "udp_server.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Appliaction_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Appliaction_UDP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UDP_Server_Functions
/// @{

static void udp_find_receive(struct netconn* conn, struct netbuf* buf)
{
    uint32_t ip = getIpAddress();
    uint8_t  mac[6];

    char* buffer = NULL;
    u16_t buflen = 0;

    struct netbuf* newbuf;

    char respstring[128] = {0};
    char macstring[25]   = {0};

    getMacAddress(mac);
    sprintf(macstring, " MAC: %02X-%02X-%02X-%02X-%02X-%02X ", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    netbuf_data(buf, (void**)&buffer, &buflen);

    if (buflen > 0) {
        if (strcmp(buffer, UDP_FIND_STRING) == 0) {
            strcpy(respstring, inet_ntoa(ip));
            strcat(respstring, macstring);
            strcat(respstring, DEVICE_INFO);

            newbuf = netbuf_new();

            if (newbuf == NULL) {
                printf("[Error] newbuf alloc new netbuf failed!\n");
                return;
            }

            netbuf_alloc(newbuf, strlen(respstring));
            netbuf_data(newbuf, (void**)&buffer, &buflen);
            memcpy(buffer, respstring, buflen);
            newbuf->addr.addr = buf->addr.addr;
            newbuf->port      = buf->port;
            netconn_send(conn, newbuf);
            netbuf_delete(newbuf);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void udp_find_server_thread(void* arg)
{
    struct netconn* conn;
    struct netbuf*  buf;

    LWIP_UNUSED_ARG(arg);

    conn = netconn_new(NETCONN_UDP);  // Create a new UDP connection handle.

    if (conn != NULL) {
        if (netconn_bind(conn, IP_ADDR_ANY, UDP_FIND_SERVER_PORT) == ERR_OK) {
            while (1) {
                if (netconn_recv(conn, &buf) == ERR_OK) {
                    udp_find_receive(conn, buf);
                    netbuf_delete(buf);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void udp_server_thread(void* arg)
{
    struct netconn* conn;
    struct netbuf*  buf;

    LWIP_UNUSED_ARG(arg);

    conn = netconn_new(NETCONN_UDP);  // Create a new UDP connection handle.

    if (conn != NULL) {
        if (netconn_bind(conn, IP_ADDR_ANY, UDP_SERVER_PORT) == ERR_OK) {
            while (1) {
                if (netconn_recv(conn, &buf) == ERR_OK) {
                    netconn_send(conn, buf);
                    netbuf_delete(buf);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void udp_server_init(void)
{
    sys_thread_new("UDP_SERVER", udp_server_thread, NULL, 128, 2);
    sys_thread_new("UDP_FIND_SERVER", udp_find_server_thread, NULL, 128, 2);
}

/// @}

/// @}

/// @}
