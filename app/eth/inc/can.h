////////////////////////////////////////////////////////////////////////////////
#ifndef __CAN_H_
#define __CAN_H_
////////////////////////////////////////////////////////////////////////////////

#include "hal_can.h"

////////////////////////////////////////////////////////////////////////////////
#ifdef _CAN_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL CanPeliTxMsg gPeliTxMessage;
GLOBAL CanPeliRxMsg gPeliRxMessage;

GLOBAL u8 canTx;
GLOBAL u8 canSel;


#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

void initGPIO_CAN();
void NVIC_CAN();
void initCAN();
void CAN_SendPacket();;
void BSP_CAN_Configure();

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////