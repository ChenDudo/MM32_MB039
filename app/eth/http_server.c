////////////////////////////////////////////////////////////////////////////////
/// @file     http_server.c
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  1.0.0
/// @date     2021-04-21
/// @brief    THIS FILE PROVIDES ALL THE http_server.c EXAMPLE.
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

// Define to prevent recursive inclusion  --------------------------------------
#define _HTTP_SERVER_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include <stdio.h>

#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "lwip/arch.h"
#include "lwip/opt.h"
#include "lwip/tcpbase.h"
#include "lwip/tcp.h"

#include "http_server.h"
#include "lwip/sys.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Appliaction_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Appliaction_Http
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Appliaction_Http_Data
/// @{

#if defined(LWIP_NETCONN) && (LWIP_NETCONN == 1)

char indexHtml[] = "\
<!DOCTYPE html>\
    <html lang=\"en\">\
        <head>\
            <meta charset=\"UTF-8\">\
                <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
                    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\
                        <title>MM32 lwIP Http Server</title>\
                            </head>\
                                <body>\
                                    <h1>Hello World!</h1>\
                                        <p >This is MM32 Ethernet HTTP Server DEMO!</p>\
                                            <button onclick=\"addAction()\">Add</button>\
                                                <button onclick=\"clearAction()\">Clear</button>\
                                                    <div id=\"t1\"></div>\
                                                        <script type=\"text/javascript\" src=\"main.js\"></script>\
                                                            </body>\
                                                                </html>\
                                                                    ";

char mainJs[] = "\
                                                                            let t1 = document.getElementById('t1');\
                                                                                function addAction() {\
                                                                                    t1.innerHTML += '<div>test!!!!</div>';\
                                                                                }\
                                                                                    function clearAction() {\
                                                                                        t1.innerHTML = '';\
                                                                                    }";

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Http_Server_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Http server task processing function.
/// @param  arg: Task parameters.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void http_server_process(struct netconn* conn)
{
    struct netbuf* inbuf;
    char*          buf;
    u16_t          buflen;

    if (netconn_recv(conn, &inbuf) == ERR_OK) {  // Receive connection data.

        if (netconn_err(conn) == ERR_OK) {  // Get and reset pending error on the connection.

            netbuf_data(inbuf, (void**)&buf, &buflen);  // Get data and data length.

            if ((buflen >= 5) &&
                (strncmp(buf, "GET /", 5) == 0)) {  // Check whether the data is the request header(Begin with "GET /").
                if (strncmp(buf + 5, "main.js", 7) == 0) {
                    netconn_write(conn, mainJs, sizeof(mainJs) - 1, NETCONN_NOCOPY);
                }
                else if (strncmp(buf + 5, " ", 1) == 0) {
                    netconn_write(conn, indexHtml, sizeof(indexHtml) - 1, NETCONN_NOCOPY);
                }
                else {
                    netconn_write(conn, " ", 1, NETCONN_NOCOPY);
                }
            }
        }
    }

    // Close the connection (server closes in HTTP)
    netconn_close(conn);

    // Delete the buffer (netconn_recv gives us ownership,
    // so we have to make sure to deallocate the buffer)
    netbuf_delete(inbuf);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Http server thread function.
/// @param  arg: Task parameters.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void http_server_thread(void* arg)
{
    struct netconn *conn, *newconn;

    LWIP_UNUSED_ARG(arg);

    conn = netconn_new(NETCONN_TCP);  // Create a new TCP connection handle.

    if (conn != NULL) {
        if (netconn_bind(conn, NULL, HTTP_SERVER_PORT) == ERR_OK) {  // Bind to port 80 (HTTP) with default IP address

            netconn_listen(conn);  // Start listening port.

            while (1) {
                if (netconn_accept(conn, &newconn) == ERR_OK) {  // Accept a new connection on a TCP listening connection.
                    http_server_process(newconn);                // Processing connection
                    netconn_delete(newconn);                     // Close the connection
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Http server task creation function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void http_server_init(void)
{
    sys_thread_new("HTTP", http_server_thread, NULL, 500, 2);
}

#endif

/// @}

/// @}

/// @}
