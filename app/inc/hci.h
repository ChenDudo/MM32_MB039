////////////////////////////////////////////////////////////////////////////////
/// @file     HCI.H
/// @author   
/// @version  v1.0.0
/// @date     2021-03-13
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
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2021 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#ifndef __HCI_H
#define __HCI_H

// Files includes  -------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_UID
/// @brief UID example modules
/// @{

#define BuOffset 55

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Variables
/// @{

#ifdef _HCI_C_

#define GLOBAL

GLOBAL bool K1Flag = false;
GLOBAL bool K2Flag = false;
GLOBAL bool K3Flag = false;

#else
#define GLOBAL extern

#endif

GLOBAL bool K1Flag,K2Flag,K3Flag,K4Flag;
GLOBAL u16  LDFreq[4], LDCnt[4];

/**/
GLOBAL u8	ledCnt;

GLOBAL u8	fKey0;		// menuOverTime Flag
GLOBAL u32	fKeyCnt;	// menuOverTime Count

GLOBAL u8	fKey1,   fKey2,   fKey3,  fKey4;
GLOBAL u8	fKeyP1,  fKeyP2,  fKeyP3, fKeyP4;

GLOBAL u32	Key1Cnt,   Key2Cnt,   Key3Cnt, Key4Cnt;
GLOBAL u8	Key1CntF,  Key2CntF,  Key3CntF, Key4CntF;


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

/// @}

void dispButton();
void showButton(u16 x, u16 y, u8 frame, char* str);
u8 eventKey(u8 key, u8* flag);
void dispMenuButton(u8 idx);
void scanKey();
void dispMenu(u8 sta);
void clearMenu();

void hci_task(void);
void hci_tick();
void Key1Down(void);
void Key2Down(void);
void Key3Down(void);
void Key4Down(void);
void Key1Pressing(void);
void Key2Pressing(void);
void Key3Pressing(void);
void Key4Pressing(void);

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __MUSIC_H */
////////////////////////////////////////////////////////////////////////////////
