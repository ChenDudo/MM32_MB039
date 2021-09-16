////////////////////////////////////////////////////////////////////////////////
/// @file     UID.C
/// @author   D CHEN
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UID EXAMPLE.
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
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _CAN_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"
#include "main.h" 
#include "hal_can.h"
#include "hal_gpio.h"	
#include "hal_nvic.h"	

#include "main.h"
#include "can.h"

////////////////////////////////////////////////////////////////////////////////
void initGPIO_CAN()
{
    GPIO_InitTypeDef	GPIO_InitStructure;
    COMMON_EnableIpClock(emCLOCK_GPIOB);
    COMMON_EnableIpClock(emCLOCK_EXTI);
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   		//can rx  pb8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_FLOATING
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_9);
    
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   		//can tx  pb9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_9);
}

////////////////////////////////////////////////////////////////////////////////
void NVIC_CAN()
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void initCAN()
{
	CAN_Peli_InitTypeDef  CAN_Peli_InitStruct;
	
	COMMON_EnableIpClock(emCLOCK_CAN); 
	
	CAN_Mode_Cmd(CAN1, CAN_PELIMode);
	CAN_ResetMode_Cmd(CAN1,ENABLE);

    CAN_AutoCfg_BaudParam(&CAN_Peli_InitStruct,  36000000, 100000 );  //100K baud
	/* Initialize the SAM member(where can be set (SET or RESET)) */
	CAN_Peli_InitStruct.SAM = RESET;
	/* Initialize the LOM member*/
	CAN_Peli_InitStruct.LOM = DISABLE;
	/* Initialize the STM member*/

	if (canSel == 0)
		CAN_Peli_InitStruct.STM = ENABLE;
	else if (canSel == 1)
		CAN_Peli_InitStruct.STM = DISABLE;

//	CAN_Peli_InitStruct.SRR = ENABLE; 
	CAN_Peli_InitStruct.SRR = DISABLE;
	
	/* Initialize the SM member*/
	CAN_Peli_InitStruct.SM = DISABLE;
	
	CAN_Peli_InitStruct.EWLR = 0x96;
	CAN_Peli_Init(&CAN_Peli_InitStruct);

///////	
	CAN_Peli_FilterInitTypeDef CAN_Peli_FilterInitStruct;

	CAN_Peli_FilterInitStruct.AFM = CAN_FilterMode_Singal;
	CAN_Peli_FilterInitStruct.CAN_FilterId0 = 0x00;
	CAN_Peli_FilterInitStruct.CAN_FilterId1 = 0x00;
	CAN_Peli_FilterInitStruct.CAN_FilterId2 = 0x00;
	CAN_Peli_FilterInitStruct.CAN_FilterId3 = 0x00;

	CAN_Peli_FilterInitStruct.CAN_FilterMaskId0 = 0xff;
	CAN_Peli_FilterInitStruct.CAN_FilterMaskId1 = 0xff;
	CAN_Peli_FilterInitStruct.CAN_FilterMaskId2 = 0xff;
	CAN_Peli_FilterInitStruct.CAN_FilterMaskId3 = 0xff;

	CAN_Peli_FilterInit( &CAN_Peli_FilterInitStruct );
	CAN_ResetMode_Cmd(CAN1,DISABLE);
    
    CAN_Peli_ITConfig(CAN_IT_RI , ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void CAN1_RX_IRQHandler()
{
    if(CAN_Peli_GetITStatus(CAN_IT_RI)){
        CAN_Peli_Receive(&gPeliRxMessage);
		rf.can = true;
		error.can = false;
		rfOverCnt.can = 0;

    }
}

////////////////////////////////////////////////////////////////////////////////
void CAN_SendPacket()
{
	u16 addr;
    gPeliTxMessage.FF = 0;
    gPeliTxMessage.RTR = 0;

	addr =  0x123;
	gPeliTxMessage.IDHH = (addr >> 3) & 0xff;
	gPeliTxMessage.IDHL = (addr) & 0x7;
    gPeliTxMessage.DLC = 8;

	for (u8 i = 0; i < 8; i++){
		gPeliTxMessage.Data[i] = i + canTx;
	}
	canTx++ ;

	CAN_Peli_Transmit(&gPeliTxMessage);
}

////////////////////////////////////////////////////////////////////////////////
void BSP_CAN_Configure()
{
    initGPIO_CAN();
    initCAN();
    NVIC_CAN();
}