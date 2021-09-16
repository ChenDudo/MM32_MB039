////////////////////////////////////////////////////////////////////////////////
/// @file     tcp_server.h
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  v2.0.0
/// @date     2019-08-01
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE tcp_server.h EXAMPLES.
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

#ifndef __UDP_SERVER_H
#define __UDP_SERVER_H

#define UDP_SERVER_PORT                 6001

#ifdef _UDP_SERVER_C_
#define GLOBAL

#else
#define GLOBAL extern
#endif

#undef GLOBAL

void InitUdpServer(void);

#endif
