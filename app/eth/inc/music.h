////////////////////////////////////////////////////////////////////////////////
/// @file     UID.H
/// @author   PX Liu
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
#ifndef __MUSIC_H
#define __MUSIC_H

// Files includes  -------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_UID
/// @brief UID example modules
/// @{

typedef enum {
    PLAYSTEP1,
    PLAYSTEP2,
    PLAYSTEP3
} emMusicStep;

typedef struct {
    u8 PlayFlag;
    u8 CurrentPlayFlag; 
    emMusicStep PlayStep;
    s8 MusicNum;
    u16 ToneCount;
    u16 ToneNumCount;
} _sPlayMusic;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Variables
/// @{

#ifdef _MUSIC_C_

#define GLOBAL
#else
#define GLOBAL extern

#endif

GLOBAL _sPlayMusic  sPlayMusic;
GLOBAL u16          playCnt;
#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

/// @}

void musicTick(void);
void setBuzzerEn(FunctionalState NewState);
void setBuzzerFreq(u16 Period);

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __MUSIC_H */
////////////////////////////////////////////////////////////////////////////////
