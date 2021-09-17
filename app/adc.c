////////////////////////////////////////////////////////////////////////////////
/// @file     ADC.C
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
#define _ADC_C_
// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"
#include "adc.h"
#include "main.h"

#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
void adc_Tick()
{
    // 25deg temp reference value at 0x1FFF_F7F4
    static float temp;

    u16* p;
    p = (u16*)0x1FFFF7F4;

    temp = ((*p == 0xffff) || (*p == 0x0000) || (*p < 1700) || (*p > 1900)) ? 1800 : *p;

    adcVol += adcValue[0];
    adcTmp += adcValue[3];

    if (rfCnt.adc++ >= 40) {
        rfCnt.adc = 0;
        adcVolt   = ((adcVol / 40) * 30 + adcVolt * 70) / 100;
        if (adcVolt > 4096)
            adcVolt = 4096;
        adcVol = 0;
        rf.adc = true;
    }

    if (rfCnt.temp++ >= 400) {
        rfCnt.temp = 0;
        temp       = ((adcTmp / 400) * 10 + temp * 90) / 100;
        adcTemp    = (10 * (temp - 1800) / 43) + 25;
        adcTmp     = 0;
        rf.temp    = true;
    }
    if (++adcCnt > 5) {
        adcCnt = 0;
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        if (ADC_GetFlagStatus(ADC1, ADC_IT_EOC)) {
            adcValue[0] = ADC1->CH5DR;
            adcValue[1] = ADC1->CH4DR;
            adcValue[2] = ADC1->CH1DR;
            adcValue[3] = ADC1->CH14DR;
        }
        for (u8 i = 0; i < 4; i++) {
            rv[i] = (u16)((float)(rv[i] * 8 + adcValue[i] * 2) / 10);
        }
        adcFlag = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BSP_ADC_Configure()
{
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    COMMON_EnableIpClock(emCLOCK_ADC1);
    COMMON_EnableIpClock(emCLOCK_DMA1);
    COMMON_EnableIpClock(emCLOCK_GPIOA);

    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution       = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE         = ADC_PCLK2_PRESCARE_16;
    ADC_InitStructure.ADC_Mode             = ADC_Mode_Continue;
    ADC_InitStructure.ADC_DataAlign        = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC1_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_ANY_Cmd(ADC1, DISABLE);
    ADC_ANY_NUM_Config(ADC1, 4);
    ADC_ANY_CH_Config(ADC1, 0, ADC_Channel_5);
    ADC_ANY_CH_Config(ADC1, 1, ADC_Channel_4);
    ADC_ANY_CH_Config(ADC1, 2, ADC_Channel_1);
    ADC_ANY_CH_Config(ADC1, 3, ADC_Channel_TempSensor);
    ADC_ANY_Cmd(ADC1, ENABLE);

    ADC_TempSensorVrefintCmd(ENABLE);
    ADC_Cmd(ADC1, ENABLE);
}

/// @}

/// @}

/// @}
