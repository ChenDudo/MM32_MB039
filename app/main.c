////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN.C
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
#define _MAIN_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"
#include "mm32_system.h"
#include "common.h"
#include "bsp_lcd.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_sdio.h"
#include "hal_uart.h"
#include "main.h"
#include "adc.h"
#include "hci.h"
#include "lcd.h"
#include "datetime.h"
#include "rtc.h"
#include "uart.h"
#include "spiflash.h"
#include "i2c.h"
#include "can.h"
#include "music.h"
#include "spiflash.h"
#include "lwip/timeouts.h"
#include "eth.h"
#include "tcp_server.h"
#include "dhcp_client.h"
#include "httpd.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{

#define USE_BEEP

////////////////////////////////////////////////////////////////////////////////
void initPara()
{
    SystemTick_Count = 0;
    retryEth = true;
    memset(ledStatus, 0x00, sizeof(ledStatus));
    memset(&sPlayMusic, 0x00, sizeof(sPlayMusic));
}

////////////////////////////////////////////////////////////////////////////////
void initPeri()
{
    BSP_KEY_Configure();
    BSP_KeyFuncSet(1, Key1Down, Key1Pressing);
    BSP_KeyFuncSet(2, Key2Down, Key2Pressing);
    BSP_KeyFuncSet(3, Key3Down, Key3Pressing);
    BSP_KeyFuncSet(4, Key4Down, Key4Pressing);
    BSP_LED_Configure();
    BSP_LCD_Configure();
    BSP_ADC_Configure();
    BSP_CAN_Configure();
    BSP_UART_Configure();
    BSP_I2C_Configure();
    BSP_FLASH_Configure();
    error.flash = !checkSPIFlashId();
    checkRTC();

    BSP_BEEP_Configure(1000);
    for(u8 i = 0; i < 2; i++){
        OpenLED();  BEEP_on(1000);  while(!delay(50));
        CloseLED(); BEEP_off();     while(!delay(200));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    SystemTick_Count++;
    gLwipCounter ++;
    processOver();
    scanKey();
    hci_tick();

    if (tickCnt++ >= 500) {
        tickFlag = true;
        tickCnt = 0;
    }
    if (rfCnt.lcd++ > 20) {
		rfCnt.lcd = 0;
		rf.lcd = true;
	}
    if (ready){
        adc_Tick();
        //if (rfCnt.flash++ > 200) {
        //    rfCnt.flash = 0;
        //    error.flash = !checkSPIFlashId();
        //    rf.flash = true;
        //}
        if (rfCnt.rtc++ > 200) {
            rfCnt.rtc = 0;
            rf.rtc = true;
        }
        if (rfCnt.i2c++ > 500) {
            rfCnt.i2c = 0;
            i2cRcvPacket(0, &i2cRx[0], 8);
            rfOverCnt.i2c = 0;
            rf.i2c = true;
        }
        if (rfCnt.can++ > 500) {
            rfCnt.can = 0;
            if ((canSel == 0) || (canSel == 1))
                CAN_SendPacket();
        }
        if (rfCnt.uart1++ > 500) {
            rfCnt.uart1 = 0;
            UART_SendData(UART2, uartTx1++);
        }
        if (rfCnt.uart2++ > 500) {
            rfCnt.uart2 = 0;
            UART_SendData(UART8, uartTx2--);
        }
        if (rfCnt.eth++ > 500) {
            rfCnt.eth = 0;
            rf.eth = true;
        }
        if (rfCnt.flash++ > 500) {
            rfCnt.flash = 0;
            rf.flash = true;
        }
        if (rfCnt.music++ >= 20) {
            rfCnt.music = 0;
            musicTick();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void processOver()
{
	if (menuCnt > 0){
		if (fKeyCnt++ > 5000) {
			fKey0 = true;		// set menuOverTime Flag
			fKeyCnt = 0;		// clear menuOverTime Count
		}
	}

	if ((rfOverCnt.i2c++ > 1200)  && (!rf.i2c)){
		rfOverCnt.i2c = 0;
		error.i2c = true;
		rf.i2c = true;
	}
	if ((rfOverCnt.can++ > 1200) && (!rf.can)){
		rfOverCnt.can = 0;
		error.can = true;
		rf.can = true;
	}
	if ((rfOverCnt.uart1++ > 1200) && (!rf.uart1)){
		rfOverCnt.uart1 = 0;
		error.uart1 = true;
		rf.uart1 = true;
	}
	if ((rfOverCnt.uart2++ > 1200) && (!rf.uart2)){
		rfOverCnt.uart2 = 0;
		error.uart2 = true;
		rf.uart2 = true;
	}
    if ((rfOverCnt.eth++ > 1200) && (!rf.eth) && enableETH){
		rfOverCnt.eth = 0;
		rf.eth   = true;
        retryEth = true;
	}
}

////////////////////////////////////////////////////////////////////////////////
void refresh()
{
	refreshLCD(REFRESH_ADC,     &rf.adc);
	refreshLCD(REFRESH_TEMP,    &rf.temp);
	refreshLCD(REFRESH_FLASH,   &rf.flash);
	refreshLCD(REFRESH_RTC,     &rf.rtc);
	refreshLCD(REFRESH_I2C,     &rf.i2c);
	refreshLCD(REFRESH_CAN,     &rf.can);
	refreshLCD(REFRESH_UART1,   &rf.uart1);
	refreshLCD(REFRESH_UART2,   &rf.uart2);
	refreshLCD(REFRESH_LED,     &rf.led);
    refreshLCD(REFRESH_MUSIC,   &rf.music);
    refreshLCD(REFRESH_SDIO,    &rf.sdio);
    refreshLCD(REFRESH_ETH,     &rf.eth);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  main function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    MCUID = SetSystemClock(emSYSTICK_On, AppTaskTick);

    initPeri();
    initPara();

    initLcdDemo();
	while (drawBlockCnt < 500){
		randRefresh();
	}
	text.fore = White;
    text.back = DarkCyan;
    clearAllScreen();

    ready = true;
    sPlayMusic.MusicNum = 1;
    sPlayMusic.PlayFlag = true;
    dispMUSIC(1);

    dispScreen();
	dispButton();
    fKey2 = true;

    while (1) {
        hci_task();
        if (menuCnt == 0) {
			refresh();
		}
#if LWIP_DHCP
        if (enableOK)
            dhcp_tick_check(&gnetif, gLwipCounter);
#endif
        if (enableETH){
            BSP_HTTP_Configure();
            enableETH = false;
        }
        if (tickFlag) {
            dispMUSIC(1);
            tickFlag = false;
        }
        sys_check_timeouts();

    }
}

/// @}

/// @}

/// @}
