////////////////////////////////////////////////////////////////////////////////
/// @file     SPIFLASH.C
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
#define _SPIFLASH_C_
// Includes  -------------------------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mm32_types.h"

#include "common.h"
#include "hal_gpio.h"
#include "hal_rcc.h"

#include "main.h"
#include "spi.h"
#include "spiflash.h"

void BSP_FLASH_Configure()
{
    initGPIO_SPI(SPI2);
	SPIM_Init(SPI2, 0x8);			//div 8
}


u8 checkSPIFlashId()
{
    SPIM_ReadID(SPI2, &spiId[0]);
	return ((spiId[0] != 0x00) | (spiId[1] != 0x00) | (spiId[2] != 0x00))
		&  ((spiId[0] != 0xff) | (spiId[1] != 0xff) | (spiId[2] != 0xff)) ;
}


void initGPIO_SPI(SPI_TypeDef* SPIx)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    COMMON_EnableIpClock(emCLOCK_GPIOB);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5);

    SPIM_CSHigh(SPIx);
    //spi2_cs  pb12//spi2_sck  pb13 //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
