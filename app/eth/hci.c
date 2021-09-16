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
#define _HCI_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"
#include "hci.h"
#include "main.h"
#include "lcd.h"
#include "datetime.h"
#include "can.h"
#include "uart.h"
#include "adc.h"
#include "eth.h"
#include "music.h"

#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_lcd.h"

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

#define Key4	KEY1_VAL    // Key0 is used, so named Key4
#define Key1	KEY2_VAL
#define Key2  	KEY3_VAL
#define Key3  	KEY4_VAL

////////////////////////////////////////////////////////////////////////////////
void hci_task()
{
    dispMenuButton(eventKey(0, &fKey0));		// menuOverTime
	dispMenuButton(eventKey(1, &fKey1));
	dispMenuButton(eventKey(2, &fKey2));
	dispMenuButton(eventKey(3, &fKey3));
    dispMenuButton(eventKey(4, &fKey4));
}

////////////////////////////////////////////////////////////////////////////////
void hci_tick()
{
    static u16 ledRunCnt = 0, state = 0;
    static bool ledRunFlag = false;
    
    if (ledRunCnt ++ > 200 - adcVolt * 100 / 4096){
        ledRunCnt = 0;
        ledRunFlag = true;
    }
    if (ledRunFlag){
        ledRunFlag = false;
        switch (state){
            case 0:
            LD1_on();
            LD2_off();
            LD3_off();
            LD4_off();
            state = 1;
            break;
            case 1:
            LD1_on();
            LD2_on();
            LD3_off();
            LD4_off();
            state = 2;
            break;
            case 2:
            LD1_off();
            LD2_on();
            LD3_on();
            LD4_off();
            state = 3;
            break;
            case 3:
            LD1_off();
            LD2_off();
            LD3_on();
            LD4_on();
            state = 4;
            break;
            case 4:
            LD1_off();
            LD2_off();
            LD3_off();
            LD4_on();
            state = 5;
            break;
            case 5:
            LD1_off();
            LD2_off();
            LD3_off();
            LD4_off();
            state = 0;
            break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void ProcessLED()
{
	switch (ledCnt++) {
		case 0:		LD1_on(); 	break;
		case 1:		LD1_off(); 	break;; 
		case 2:		LD2_on(); 	break;
		default:	LD2_off();	
					ledCnt = 0;		break;
	}
	rf.led = true;
}

////////////////////////////////////////////////////////////////////////////////
void modifyKeyStatus_2()
{
	char str[8];
	if (menuCnt == 3) {
		(Key2CntF) ? strcpy(str, "Last") : strcpy(str, "Next");
		showButton(255, BuOffset + 2 * offsetH, 1, str);
	}
}

////////////////////////////////////////////////////////////////////////////////
void modifyKeyStatus_3()
{
	char str[8];
	if (menuCnt == 3) {
		(Key3CntF) ? strcpy(str, "Down") : strcpy(str, "Up");
		showButton(255, BuOffset + 3 * offsetH, 1, str);
	}
}

////////////////////////////////////////////////////////////////////////////////
void scanKey()
{
	if (Key4 && !fKeyP4) {
		fKeyP4 = true;
		fKey4 = true;
	}
	else if (!Key4) {
		fKeyP4 = false;
	}		
//-----------------		
    if (Key1 && !fKeyP1) {
		fKeyP1 = true;
		fKey1 = true;
		fKeyCnt = 0;
	}
	else if (!Key1) {
		fKeyP1 = false;
	}		
	else {
		fKeyCnt = 0;
	}
//-----------------	
	if (Key2 && !fKeyP2) {
		fKeyP2 = true;
		fKey2 = true;
		fKeyCnt = 0;
	}
	else if (!Key2) {
		fKeyP2 = false;
		Key2Cnt = 0;
	}		
	else {
		fKeyCnt = 0;
		if ((Key2Cnt < 2001) && (Key2Cnt++ >= 2000)){
			Key2CntF = !Key2CntF;
			modifyKeyStatus_2();
		}
	}
//-----------------	
	if (Key3 && !fKeyP3) {
		fKeyP3 = true;
		fKey3 = true;
		fKeyCnt = 0;
	}
	else if (!Key3) {
		fKeyP3 = false;
		Key3Cnt = 0;
	}
	else {
		fKeyCnt = 0;
		if ((Key3Cnt < 1501) && (Key3Cnt++ >= 1500)){
			Key3CntF = !Key3CntF;
			modifyKeyStatus_3();
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
void clearMenu()
{
	menuTIME(timeSel, 0, 0);
	dispScreen();
}


////////////////////////////////////////////////////////////////////////////////
void showButton(u16 x, u16 y, u8 frame, char* str)
{
	drawButton(x, y, 60, 20, frame, 0, &str[0]);
}

void showETH()
{
    char str[8];
    if (enableOK || enableETH){
        strcpy(str, "----");
        showButton(255, BuOffset + 3 * offsetH, 0, str);
    }
    else {
        strcpy(str, "ETH");
        showButton(255, BuOffset + 3 * offsetH, 1, str);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispButton()
{
	char str[8];
	switch (menuCnt) {
	case 0:
		
		strcpy(str, "Menu");
		showButton(255, BuOffset + 1 * offsetH, 1, str);
		
		strcpy(str, "----");
		showButton(255, BuOffset + 2 * offsetH, 0, str);
		
		showETH();
		break;
	case 1:
	case 2:
		strcpy(str, "Menu");
		showButton(255, BuOffset + 1 * offsetH, 1, str);
		
		strcpy(str, "Next");
		showButton(255, BuOffset + 2 * offsetH, 1, str);
		
		strcpy(str, "----");
		showButton(255, BuOffset + 3 * offsetH, 0, str);
		break;
	default:
		strcpy(str, "Return");
		showButton(255, BuOffset + 1 * offsetH, 1, str);
		
		(Key2CntF) ? strcpy(str, "Last") : strcpy(str, "Next");
		showButton(255, BuOffset + 2 * offsetH, 1, str);
		
		if (timeSel == 6) {
			strcpy(str, "Save");
			showButton(255, BuOffset + 3 * offsetH, 1, str);
		}
		else {
			(Key3CntF) ? strcpy(str, "Down") : strcpy(str, "Up");
			showButton(255, BuOffset + 3 * offsetH, 1, str);
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
void dispMenu(u8 sta)
{
	switch (menuCnt) {
	case 0:										break;
	case 1:		menuCAN(canSel, 1, sta);		break;
	case 2:		menuUART(uartSel, 1, sta);		break;
	default:	menuTIME(timeSel, 1, sta);		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
u8 eventKey(u8 key, u8* flag)
{
	if (!*flag) return 0;
    
	*flag = false;
    
	switch (key) {
        // menuOverTime
        case 0:
		if (timeSel == 6) timeSel = 0; 
		menuCnt = 0;
		return 2;
        
        // Menu	
        case 1:
		if (++menuCnt > 3)  menuCnt = 0;
		
		switch(menuCnt){
            case 0:
			return 2;
            case 3:
			Key2CntF = false;
			Key3CntF = false;
			RTC_GetTime(&gtp);
			if (timeSel == 6) timeSel = 0;
            default:			
			return 1;
		}
        
        // Next & Last	
        case 2:
		switch (menuCnt) {
            case 1:
			if (++canSel > 2) canSel = 0;
			initCAN();
			break;
            case 2:
			if (++uartSel > 2) uartSel = 0;
			break;
            case 3:
			if (Key2CntF) 	{if (--timeSel == -1) 	timeSel = 6;}
			else 			{if (++timeSel > 6) 	timeSel = 0;}
            default:
			break;
		}
		return 3;
        
        
        case 4:
        rf.music = 1;
        if (sPlayMusic.PlayFlag){
            sPlayMusic.PlayFlag = false;
            setBuzzerEn(DISABLE);
        }
        else {
            sPlayMusic.PlayFlag = true;
        }
        return 4;
        
        case 3:										// Up & Down & Save & ETH
		switch (menuCnt) {
            case 3:
			modifyTime(timeSel);
			if (timeSel == 6) {
				menuCnt = 0;
				return 2;
			}
            break;
            case 0:
            if (!enableETH && !enableOK) {
                enableETH = true;
                rf.eth = true;
                showButton(255, BuOffset + 3 * offsetH, 0, "----");
            }
            break;
            default:
			break;
		}
		return 3;
	}
}

////////////////////////////////////////////////////////////////////////////////
void dispMenuButton(u8 idx)
{
	switch (idx) {
	case 0:
		break;
	case 1:
		dispMenu(0);
		dispButton();
		break;
	case 2:
		clearMenu();
	default:
		dispMenu(1);
		dispButton();
		break;
	}
}








////////////////////////////////////////////////////////////////////////////////
void Key1Down()
{
    K1Flag = true;
    ledStatus[0] = !ledStatus[0];
}

////////////////////////////////////////////////////////////////////////////////
void Key2Down()
{
    K2Flag = true;
    ledStatus[1] = !ledStatus[1];
}

////////////////////////////////////////////////////////////////////////////////
void Key3Down()
{
    K3Flag = true;
    ledStatus[2] = !ledStatus[2];
}

////////////////////////////////////////////////////////////////////////////////
void Key4Down()
{
    K4Flag = true;
    ledStatus[3] = !ledStatus[3];
}

////////////////////////////////////////////////////////////////////////////////
void Key1Pressing()
{
    ledStatus[0] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void Key2Pressing()
{
    ledStatus[1] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void Key3Pressing()
{
    ledStatus[2] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void resetTest()
{
    void (*fp)(void);
    fp = (void (*)(void))(* (vu32 *)(0x8000004));
    (*fp)(); 
}

////////////////////////////////////////////////////////////////////////////////
void Key4Pressing()
{
    ledStatus[3] = 1;
}

/// @}

/// @}

/// @}
