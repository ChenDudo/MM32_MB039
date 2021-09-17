////////////////////////////////////////////////////////////////////////////////
/// @file     SDIO.C
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
#define _SDIO_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"
#include "sdio.h"
#include "main.h"

#include "bsp_sdio.h"

#include "hal_spi.h"
#include "hal_gpio.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{

#define SD_DET  (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))


////////////////////////////////////////////////////////////////////////////////
void sdio_task()
{
#if defined(__USE_SDIO)
    static bool reflag = true;
    if (SD_DET){
        if (cardFirst){
            cardFirst = false;
            cardInitFlag = true;
        }
        reflag = true;
    }
    else {
        if (reflag){
            reflag = false;
            rf.sdio = 1;
        }
        error.sdio = 1;
        SDErroCode = SD_NoInsert;
        cardFirst = true;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void sdio_tick()
{
    if (cardInitFlag){
        cardInitFlag = false;
        initSDIO();
    }

}

////////////////////////////////////////////////////////////////////////////////
void initSDIO()
{
    SDErroCode = SD_Init();
    if (SDErroCode != SD_OK){
        error.sdio = 1;
    }
    else {
        SD_ReadDisk(&sdCardbuf[0], 0, 1);
        error.sdio = 0;
    }
    rf.sdio = 1;
}


/// @}

/// @}

/// @}
