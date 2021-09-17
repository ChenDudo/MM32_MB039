////////////////////////////////////////////////////////////////////////////////
/// @file     ADC.H
/// @author   
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE UID
///           EXAMPLES.
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
#ifndef __ADC_H
#define __ADC_H

// Files includes  -------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_UID
/// @brief UID example modules
/// @{

#if defined(__MM32_MB020)

#define ADCCHx ADC_Channel_1
#define ADCPin GPIO_Pin_1

#endif
#if defined(__MM32_MB021)

#define ADCCHx ADC_Channel_5
#define ADCPin GPIO_Pin_5

#endif

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Variables
/// @{

#ifdef _ADC_C_

#define GLOBAL
#else
#define GLOBAL extern

#endif

GLOBAL bool adcFlag;
GLOBAL u16 adcValue[4], rv[4], adcCnt;


/**/
GLOBAL u32 ADC1Value[2];
GLOBAL float adcTemp;
GLOBAL float adcTmp;

GLOBAL u32 adcVolt;
GLOBAL u32 adcVol;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

/// @}

void adc_Tick(void);
void BSP_ADC_Configure(void);

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __MUSIC_H */
////////////////////////////////////////////////////////////////////////////////
