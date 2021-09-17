////////////////////////////////////////////////////////////////////////////////
/// @file     SPI.C
/// @author   D CHEN
/// @version  v1.0.0
/// @date     2021-03-13
/// @brief    THIS FILE PROVIDES ALL THE EVBOARD EXAMPLE.
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
#define _SPI_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"
#include "main.h"
#include "hal_spi.h"
#include "hal_gpio.h"
#include "spi.h"

extern void DataCompare(u8 *p1, u8 *p2,u16 count);

void SPIM_CSLow(SPI_TypeDef* SPIx)
{
	if(SPIx==SPI1) 	{
		GPIO_ResetBits( GPIOA, GPIO_Pin_4 );
	}
	else {
		GPIO_ResetBits( GPIOB, GPIO_Pin_12 );
	}
}

void SPIM_CSHigh(SPI_TypeDef* SPIx)
{
	if(SPIx==SPI1){
		GPIO_SetBits( GPIOA, GPIO_Pin_4 );
	}
	else{
		GPIO_SetBits( GPIOB, GPIO_Pin_12 );
	}
}

void SPIM_TXEn(SPI_TypeDef* SPIx)
{
	SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

void SPIM_TXDisable(SPI_TypeDef* SPIx)
{
	SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Tx);
}

void SPIM_RXEn(SPI_TypeDef* SPIx)
{
	SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

void SPIM_RXDisable(SPI_TypeDef* SPIx)
{
	SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Rx);
}

u16 SPIMReadWriteByte(SPI_TypeDef* SPIx,u8 tx_data)
{
    u16 overTime = 10000;
	SPI_SendData(SPIx, tx_data);
	while (1){
		if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL))	{
			return SPI_ReceiveData(SPIx);
		}
        if (overTime-- == 0)
            return 0;
	}

}

void SPIM_Init(SPI_TypeDef* SPIx,unsigned short spi_baud_div)
{
	SPI_InitTypeDef SPI_InitStructure;

	COMMON_EnableIpClock(emCLOCK_SPI2);

	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_DataWidth = 8;                    //SPI_DataWidth_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPIx, &SPI_InitStructure);
    if (SPI_InitStructure.SPI_BaudRatePrescaler <= 8) {
        SPI_DataEdgeAdjust(SPI2, SPI_DataEdgeAdjust_FAST);
    }
    SPIM_TXEn(SPIx);
	SPIM_RXEn(SPIx);
	SPI_Cmd(SPIx, ENABLE);

}

void SPIM_ReadID(SPI_TypeDef* SPIx, u8* id)
{
	u16 i;

	SPIM_CSLow(SPIx);								//Spi cs assign to this pin,select
	SPIMReadWriteByte( SPIx,RDID);

	for(i=0;i<3;i++){
		*(id + i) = SPIMReadWriteByte( SPIx,0x01);
	}

	SPIM_CSHigh(SPIx);   							//Spi cs release
}

void SPIM_WriteEnable(SPI_TypeDef* SPIx)
{
	SPIM_CSLow(SPIx); 								//Spi cs assign to this pin,select
	SPIMReadWriteByte( SPIx,WREN);
	SPIM_CSHigh(SPIx); 								//Spi cs release
}


////////////////////////////////////////////////////////////////////////////////
void SPIM_checkStatus(SPI_TypeDef* SPIx)
{
	u8 temp;
	SPIM_CSLow(SPIx); 								//Spi cs assign to this pin,select
	SPIMReadWriteByte( SPIx,RDSR);
	while(1){
		temp = SPIMReadWriteByte( SPIx,0x00);
		if((temp&0x01)==0x0)
			break;
	}
	SPIM_CSHigh(SPIx);  							//Spi cs release
}

void SPIM_WriteDisable(SPI_TypeDef* SPIx)
{
	SPIM_CSLow(SPIx);
	SPIMReadWriteByte( SPIx,WRDI);
	SPIM_CSHigh(SPIx);
}

void SPIM_PageRead(SPI_TypeDef* SPIx,u32 address,u8 *p,u16 number)  //page = 256 bytes
{
	u8 addr0,addr1,addr2;
	u16 i;
	address = address&0xffffff00; 					//page address
	addr0 = (u8)(address>>16);
	addr1 = (u8)(address>>8);
	addr2 = (u8)address;

	SPIM_CSLow(SPIx); 								//Spi cs assign to this pin,select

	SPIMReadWriteByte( SPIx,READ);
	SPIMReadWriteByte( SPIx,addr0);
	SPIMReadWriteByte( SPIx,addr1);
	SPIMReadWriteByte( SPIx,addr2);

	for(i=0;i<256;i++) {
		rxtmpdata[i] = SPIMReadWriteByte(SPIx,0x00);
	}

	SPIM_CSHigh(SPIx);  							//Spi cs release
}

void SPIM_PageProgram(SPI_TypeDef* SPIx,u32 address,u8 *p,u16 number)
{
	u16 j;
	u8 addr0,addr1,addr2;
	address = address&0xffffff00; 					//page address
	addr0 = (u8)(address>>16);
	addr1 = (u8)(address>>8);
	addr2 = (u8)address;

	SPIM_WriteEnable(SPIx);
	SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select
	SPIMReadWriteByte(SPIx,PP);
	SPIMReadWriteByte(SPIx,addr0);
	SPIMReadWriteByte(SPIx,addr1);
	SPIMReadWriteByte(SPIx,addr2);
	for(j=0;j<number;j++){SPIMReadWriteByte(SPIx,*(p++));
	}

	SPIM_CSHigh(SPIx);  							//Spi cs release

	SPIM_checkStatus(SPIx);
}

void SPIM_SectorErase(SPI_TypeDef* SPIx,u32 address)
{
	u8 addr0,addr1,addr2;
	address = address & 0xffff0000;
	addr0 = ((u8)(address>>16))&0xff;
	addr1 = ((u8)(address>>8))&0xff;
	addr2 = ((u8)address)&0xff;

	SPIM_WriteEnable(SPIx);

	SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select

	SPIMReadWriteByte(SPIx,SE);
	SPIMReadWriteByte(SPIx,addr0);
	SPIMReadWriteByte(SPIx,addr1);
	SPIMReadWriteByte(SPIx,addr2);
	SPIM_CSHigh(SPIx);  							//Spi cs release

	SPIM_checkStatus(SPIx);
}

void SPIM_BlockErase(SPI_TypeDef* SPIx)
{
	SPIM_WriteEnable(SPIx);

	SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select

	SPIMReadWriteByte( SPIx,BE);

	SPIM_CSHigh(SPIx);  							//Spi cs release

	SPIM_checkStatus(SPIx);
}

void SPIM_Close(SPI_TypeDef* SPIx)
{
	SPIM_CSHigh(SPIx);  							//Spi cs release
	SPIMReadWriteByte(SPIx,0x01);
}

void SPIM_Test(SPI_TypeDef* SPIx)
{
	u16 i;
	u8 id[3];
	for(i = 0; i < 256; i++){
		tmpdata[i] = i*2;
	}

	SPIM_Init(SPIx,0x2);

	SPIM_ReadID(SPIx, &id[0]);

	SPIM_SectorErase(SPIx, 0);

	SPIM_PageProgram(SPIx,256,tmpdata,256);

	for(i = 0; i < 256; i++){
		rxtmpdata[i] = 0x0;
	}

	SPIM_PageRead(SPIx,256,rxtmpdata,256);
}
