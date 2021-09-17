////////////////////////////////////////////////////////////////////////////////
/// @file     RTC.C
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
#define _RTC_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"
#include "main.h"

#include "datetime.h"
#include "rtc.h"

#include "hal_nvic.h"
#include "hal_rtc.h"
#include "hal_pwr.h"
#include "hal_bkp.h"
#include "hal_rcc.h"

////////////////////////////////////////////////////////////////////////////////
void NVIC_RTC()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
        RTC_ClearITPendingBit(RTC_IT_SEC);
    }
}

////////////////////////////////////////////////////////////////////////////////
void initRTC()
{
    PWR_BackupAccessCmd(ENABLE);
    RCC->BDCR |= 1 << 24;
    RCC->BDCR |= 1 << 0;

    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {};
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    RTC_WaitForLastTask();
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
    RTC_WaitForLastTask();
}

////////////////////////////////////////////////////////////////////////////////
void checkRTC()
{
    for (u32 i = 0; i < 1000; i++)
        ;
    COMMON_EnableIpClock(emCLOCK_PWR);
    COMMON_EnableIpClock(emCLOCK_BKP);

    PWR_BackupAccessCmd(DISABLE);
    PWR_BackupAccessCmd(ENABLE);
    for (u32 i = 0; i < 1000; i++)
        ;
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) {
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
        PWR_BackupAccessCmd(DISABLE);
        initRTC();
    }
    else {
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
            RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
    BKP_TamperPinCmd(DISABLE);
    BKP_ITConfig(DISABLE);

    // BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
    RCC_ClearFlag();

    timeSel = 0;
}

////////////////////////////////////////////////////////////////////////////////
