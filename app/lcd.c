////////////////////////////////////////////////////////////////////////////////
#define _LCD_C_
////////////////////////////////////////////////////////////////////////////////
#if defined(__MDM2803) || defined(__MDM2803_N)
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mm32_types.h"

#include "common.h"
#include "hal_gpio.h"
#include "bsp_lcd.h"
#include "bsp_eth.h"

#include "lcd.h"
#include "adc.h"
#include "can.h"
#include "main.h"
#include "hci.h"
#include "datetime.h"
#include "uart.h"
#include "spiflash.h"
#include "i2c.h"
#include "music.h"
#include "eth.h"

/* WIDTH = 320, HEIGHT = 240 */

////////////////////////////////////////////////////////////////////////////////
void lcd_tick() {}

////////////////////////////////////////////////////////////////////////////////
void menuCAN(u8 idx, u8 sta, u8 sel)
{
    Color_Def c;
    char      str[16];

    if (sta) {
        if (!sel) {
            strcpy(str, " CAN ");
            c.fore = DarkCyan;
            c.back = LightGrey;
            drawDialog(140, 85, &str[0], c);
        }
        setCAN(140, 85, idx, sel);
    }
    else {
        c.fore = LightGrey;
        c.back = DarkCyan;
        clearDialog(140, 85, c);
    }
}

////////////////////////////////////////////////////////////////////////////////
void menuUART(u8 idx, u8 sta, u8 sel)
{
    Color_Def c;
    char      str[12];

    if (sta) {
        if (!sel) {
            strcpy(str, " UART ");
            c.fore = DarkCyan;
            c.back = LightGrey;
            drawDialog(170, 85, &str[0], c);
        }
        c.fore = White;
        c.back = LightGrey;
        setUART(170, 85, idx, sel);
    }
    else {
        c.fore = LightGrey;
        c.back = DarkCyan;
        clearDialog(170, 85, c);
    }
}

////////////////////////////////////////////////////////////////////////////////
void menuTIME(u8 idx, u8 sta, u8 sel)
{
    Color_Def c;
    char      str[12];

    if (sta) {
        if (!sel) {
            strcpy(str, " Set Time ");
            c.fore = DarkCyan;
            c.back = LightGrey;
            drawDialog(170, 178, &str[0], c);
        }
        c.fore = White;
        c.back = LightGrey;
        setTIME(170, 178, idx, sel);
    }
    else {
        c.fore = LightGrey;
        c.back = DarkCyan;
        clearDialog(170, 178, c);
    }
}

////////////////////////////////////////////////////////////////////////////////
void setCAN(u16 w, u16 h, u8 idx, u8 sel)
{
    char str[16];

    u16 x = (320 - w) / 2;
    u16 y = (240 - h) / 2;

    for (u8 i = 0; i < 3; i++) {
        drawCircle(x + 20, y + i * 20 + 25, 3, (i == idx) ? Yellow : LightGrey);
        drawCircle(x + 20, y + i * 20 + 25, 2, (i == idx) ? Yellow : LightGrey);
        drawCircle(x + 20, y + i * 20 + 25, 1, (i == idx) ? Yellow : LightGrey);
    }

    if (!sel) {
        strcpy(str, "Self testing");
        putStr(x + 35, y + 18, 2, 1, str);
        strcpy(str, "Master");
        putStr(x + 35, y + 38, 2, 1, str);
        strcpy(str, "Slave");
        putStr(x + 35, y + 58, 2, 1, str);

        for (u8 i = 0; i < 3; i++) {
            drawCircle(x + 20, y + i * 20 + 25, 6, Yellow);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void setUART(u16 w, u16 h, u8 idx, u8 sel)
{
    char str[16];
    u16  x = (320 - w) / 2;
    u16  y = (240 - h) / 2;

    for (u8 i = 0; i < 3; i++) {
        drawCircle(x + 20, y + i * 20 + 25, 3, (i == idx) ? Yellow : LightGrey);
        drawCircle(x + 20, y + i * 20 + 25, 2, (i == idx) ? Yellow : LightGrey);
        drawCircle(x + 20, y + i * 20 + 25, 1, (i == idx) ? Yellow : LightGrey);
    }

    if (!sel) {
        strcpy(str, "UART1 <-> UART2");
        putStr(x + 35, y + 18, 2, 0, str);
        strcpy(str, "UART1 Transmit");
        putStr(x + 35, y + 38, 2, 0, str);
        strcpy(str, "UART1 Receive");
        putStr(x + 35, y + 58, 2, 0, str);
        for (u8 i = 0; i < 3; i++) {
            drawCircle(x + 20, y + i * 20 + 25, 6, Yellow);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void setTIME(u16 w, u16 h, u8 idx, u8 sel)
{
    char str[16];
    u16  x = (320 - w) / 2;
    u16  y = (240 - h) / 2;

    if (!sel) {
        strcpy(str, "  Year");
        putStr(x + 20, y + 18, 2, 0, str);
        strcpy(str, " Mouth");
        putStr(x + 20, y + 38, 2, 0, str);
        strcpy(str, "   Day");
        putStr(x + 20, y + 58, 2, 0, str);
        strcpy(str, "  Hour");
        putStr(x + 20, y + 78, 2, 0, str);
        strcpy(str, "Minute");
        putStr(x + 20, y + 98, 2, 0, str);
        strcpy(str, "second");
        putStr(x + 20, y + 118, 2, 0, str);
        drawLine(x + 15, y + 142, x + 155, y + 142, DarkGrey);
    }

    (idx == 0) ? frontBack(1) : frontBack(0);
    sprintf((char*)str, "%0.4d", gtp.year);
    putStr(x + 110, y + 18, 2, 1, str);
    (idx == 1) ? frontBack(1) : frontBack(0);
    sprintf((char*)str, "%0.2d", gtp.month);
    putStr(x + 126, y + 38, 2, 1, str);
    (idx == 2) ? frontBack(1) : frontBack(0);
    sprintf((char*)str, "%0.2d", gtp.day);
    putStr(x + 126, y + 58, 2, 1, str);
    (idx == 3) ? frontBack(1) : frontBack(0);
    sprintf((char*)str, "%0.2d", gtp.hours);
    putStr(x + 126, y + 78, 2, 1, str);
    (idx == 4) ? frontBack(1) : frontBack(0);
    sprintf((char*)str, "%0.2d", gtp.minute);
    putStr(x + 126, y + 98, 2, 1, str);
    (idx == 5) ? frontBack(1) : frontBack(0);
    sprintf((char*)str, "%0.2d", gtp.second);
    putStr(x + 126, y + 118, 2, 1, str);
    (idx == 6) ? frontBack(1) : frontBack(0);
    strcpy(str, " Save & Exit ");
    putStr(x + 32, y + 148, 2, 1, str);
}

////////////////////////////////////////////////////////////////////////////////
void dispScreen()
{
    text.fore = White;
    text.back = DarkCyan;

    dispADC(0);
    dispTEMP(0);
    dispFLASH(0);

    dispI2C(0);
    dispCAN(0);
    dispRTC(0);
    dispUART1(0);
    dispUART2(0);
    dispMUSIC(0);
    // dispLED(0);
    dispSDIO(0);
    dispETH(0);

    text.fore = Yellow;

    char str[32];

    text.fore = Yellow;
    strcpy(str, "MM32-EVBoard - MindMotion");
    putStr(27, 214, 2, 1, str);
    drawRec(27, 230, 202, 1, Blue);

    text.fore = White;
}

///////////////////////////////////////////////////////////////////////////////
void dispADC(u8 sta)
{
    char str[32];
    u16  s;
    if (!sta) {
        drawTab(5, 10, 120, 15, 2, 1);
        drawTab(124, 10, 120, 15, 1, 1);
        strcpy(str, "ADC");
        putStr(27, 14, 0, 1, str);
    }
    else {
        sprintf((char*)str, "0x%04X", adcVolt);
        putStr(77, 14, 0, 1, str);
        s = 108 * adcVolt / 4096;
        drawRec(130, 13, s, 8, Red);
        drawRec(130 + s, 13, 108 - s, 8, DarkCyan);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispTEMP(u8 sta)
{
    char str[32];
    u16  s;
    if (!sta) {
        drawTab(5, 30, 120, 15, 2, 1);
        drawTab(124, 30, 120, 15, 1, 1);

        strcpy(str, "Temp");
        putStr(24, 34, 0, 1, str);

        strcpy(str, "0");
        putStr(130, 34, 0, 1, str);

        strcpy(str, "50");
        putStr(227, 34, 0, 1, str);

        drawRec(142, 39, 80 * 20 / 50, 2, Blue);
        drawRec(142 + 80 * 21 / 50, 39, 80 * (25 - 20) / 50, 2, Green);
        drawRec(142 + 80 * 26 / 50, 39, 80 * (50 - 26) / 50, 2, Red);
    }
    else {
        sprintf((char*)str, "%.1fC", adcTemp);
        putStr(80, 34, 0, 1, str);

        s = (u16)(80 * adcTemp / 50);
        if (s > 80) {
            s       = 80;
            adcTemp = 25;
        }
        drawRec(142, 33, s, 5, DarkCyan);
        drawRec(142 + s, 33, 1, 5, White);
        drawRec(142 + s + 1, 33, 80 - s, 5, DarkCyan);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispFLASH(u8 sta)
{
    char str[32];
    if (!sta) {
        drawTab(5, 50, 60, 15, 1, 1);
        drawTab(64, 50, 84, 15, 1, 1);
        drawTab(147, 50, 98, 15, 1, 1);

        strcpy(str, "Flash");
        putStr(22, 54, 0, 1, str);
    }
    else {
        if (error.flash) {
            strcpy(str, "Read ID Fail");
            putStr(73, 54, 0, 1, str);
            strcpy(str, "Confire SW2 ->");
            putStr(155, 54, 0, 1, str);
        }
        else {
            strcpy(str, "Read ID pass");
            putStr(73, 54, 0, 1, str);
            sprintf((char*)str, "0x%02X 0x%02X 0x%02X", spiId[0], spiId[1], spiId[2]);
            putStr(155, 54, 0, 1, str);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispRTC(u8 sta)
{
    dateTimeDef tp;

    char str[32];
    if (!sta) {
        drawTab(5, 70, 60, 15, 1, 1);
        drawTab(64, 70, 180, 15, 1, 1);

        strcpy(str, "RTC");
        putStr(27, 74, 0, 1, str);
    }
    else {
        RTC_GetTime(&tp);
        sprintf((char*)str, "%0.4d/%0.2d/%0.2d %0.2d:%0.2d:%0.2d", tp.year, tp.month, tp.day, tp.hours, tp.minute, tp.second);
        putStr(75, 74, 0, 1, str);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispI2C(u8 sta)
{
    char str[32];
    if (!sta) {
        drawTab(5, 90, 60, 15, 1, 1);
        drawTab(64, 90, 180, 15, 1, 1);

        strcpy(str, "I2C");
        putStr(27, 94, 0, 1, str);
    }
    else {
        sprintf((char*)str, "%02X %02X %02X %02X %02X %02X %02X %02X ", i2cRx[0], i2cRx[1], i2cRx[2], i2cRx[3], i2cRx[4],
                i2cRx[5], i2cRx[6], i2cRx[7]);
        putStr(75, 94, 0, 1, str);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispMUSIC(u8 sta)
{
    char str[32];
    if (sPlayMusic.PlayFlag) {
        strcpy(str, "Pause");
        showButton(255, BuOffset + 0 * offsetH, 1, str);
    }
    else {
        strcpy(str, "Music");
        showButton(255, BuOffset + 0 * offsetH, 1, str);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispCAN(u8 sta)
{
    char str[32];
    if (!sta) {
        drawTab(5, 110, 60, 15, 1, 1);
        drawTab(64, 110, 180, 15, 1, 1);

        strcpy(str, "CAN");
        putStr(27, 114, 0, 1, str);
    }
    else {
        if (error.can) {
            for (u8 i = 0; i < 8; i++) {
                str[i * 3 + 0] = '-';
                str[i * 3 + 1] = '-';
                str[i * 3 + 2] = ' ';
            }
            str[24] = 0;
        }
        else {
            sprintf((char*)str, "%02X %02X %02X %02X %02X %02X %02X %02X ", gPeliRxMessage.Data[0], gPeliRxMessage.Data[1],
                    gPeliRxMessage.Data[2], gPeliRxMessage.Data[3], gPeliRxMessage.Data[4], gPeliRxMessage.Data[5],
                    gPeliRxMessage.Data[6], gPeliRxMessage.Data[7]);
        }
        putStr(75, 114, 0, 1, str);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispUART1(u8 sta)
{
    char str[32];
    if (!sta) {
        drawTab(5, 130, 60, 15, 1, 1);
        drawTab(64, 130, 180, 15, 1, 1);

        strcpy(str, "UART1");
        putStr(22, 134, 0, 1, str);
    }

    if (error.uart1) {
        for (u8 i = 0; i < 8; i++) {
            str[i * 3 + 0] = '-';
            str[i * 3 + 1] = '-';
            str[i * 3 + 2] = ' ';
        }
        str[24] = 0;
    }
    else {
        sprintf((char*)str, "%02X %02X %02X %02X %02X %02X %02X %02X ", uart1RxBuf[0], uart1RxBuf[1], uart1RxBuf[2],
                uart1RxBuf[3], uart1RxBuf[4], uart1RxBuf[5], uart1RxBuf[6], uart1RxBuf[7]);
    }
    putStr(75, 134, 0, 1, str);
}

////////////////////////////////////////////////////////////////////////////////
void dispUART2(u8 sta)
{
    char str[32];
    if (!sta) {
        drawTab(5, 150, 60, 15, 1, 1);
        drawTab(64, 150, 180, 15, 1, 1);
        strcpy(str, "UART2");
        putStr(22, 154, 0, 1, str);
    }
    if (error.uart2) {
        for (u8 i = 0; i < 8; i++) {
            str[i * 3 + 0] = '-';
            str[i * 3 + 1] = '-';
            str[i * 3 + 2] = ' ';
        }
        str[24] = 0;
    }
    else {
        sprintf((char*)str, "%02X %02X %02X %02X %02X %02X %02X %02X ", uart2RxBuf[0], uart2RxBuf[1], uart2RxBuf[2],
                uart2RxBuf[3], uart2RxBuf[4], uart2RxBuf[5], uart2RxBuf[6], uart2RxBuf[7]);
    }
    putStr(75, 154, 0, 1, str);
}

////////////////////////////////////////////////////////////////////////////////
void dispLED(u8 sta)
{
    char str[32];
    if (!sta) {
        drawTab(5, 190, 60, 15, 1, 1);
        drawTab(64, 190, 180, 15, 1, 1);

        strcpy(str, "LED");
        putStr(27, 194, 0, 1, str);
        drawRec(210, 192, 15, 8, Red);
    }
    else {
        switch (ledCnt) {
            case 0: drawRec(85, 192, 15, 8, DarkCyan); break;
            case 1:
                drawRec(85, 192, 15, 8, Blue);
                break;
                ;
            case 2: drawRec(120, 192, 15, 8, DarkCyan); break;
            default: drawRec(120, 192, 15, 8, Yellow); break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispETH(u8 sta)
{
    char str[32];
    u8   ip[4];
    if (!sta) {
        drawTab(5, 170, 60, 15, 1, 1);
        drawTab(64, 170, 180, 15, 1, 1);

        strcpy(str, "ETH");
        putStr(27, 174, 0, 1, str);
        strcpy(str, "Press \"Eth\" to start Server");
        putStr(75, 174, 0, 1, str);
    }
    else {
        if (error.eth) {
            strcpy(str, "Confirm SW2 switch on Left ");
        }
        else if (EthErr < 0) {
            switch (EthErr) {
                case -1: strcpy(str, "Confirm SW2 switch on Left "); break;
                case -2: strcpy(str, "Connect to NetworkPort CN5 "); break;
            }
        }
        else if (!enableOK && !enableETH) {
            strcpy(str, "Press \"Eth\" to start Server");
            dispButton();
        }
        else {
            if (gIPAddr == 00)
                strcpy(str, "Waiting for Server start!  ");
            else {
                ip[0] = (u8)gIPAddr;
                ip[1] = (u8)(gIPAddr >> 8);
                ip[2] = (u8)(gIPAddr >> 16);
                ip[3] = (u8)(gIPAddr >> 24);
                sprintf((char*)str, "HTTP Server://%02d.%02d.%02d.%02d  ", ip[0], ip[1], ip[2], ip[3]);
            }
        }
        putStr(75, 174, 0, 1, str);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispSDIO(u8 sta)
{
#if defined(__USE_SDIO)
    char str[32];
    if (!sta) {
        drawTab(5, 190, 60, 15, 1, 1);
        drawTab(64, 190, 180, 15, 1, 1);

        strcpy(str, "SDCard");
        putStr(18, 194, 0, 1, str);
    }
    else {
        if (error.sdio) {
            switch (SDErroCode) {
                case SD_NoInsert: strcpy(str, "Please Insert SD-Card  "); break;
                case SD_OK: strcpy(str, "Read OK                "); break;
                case SD_CMD_RSP_TIMEOUT: strcpy(str, "Read Time Out          "); break;
                default: strcpy(str, "Read Error             "); break;
            }
        }
        else {
            sprintf((char*)str, "%02X %02X %02X %02X %02X %02X %02X %02X ", sdCardbuf[0], sdCardbuf[1], sdCardbuf[2],
                    sdCardbuf[3], sdCardbuf[4], sdCardbuf[5], sdCardbuf[6], sdCardbuf[7]);
            ;
        }
        putStr(75, 194, 0, 1, str);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void frontBack(u8 sel)
{
    if (sel) {
        text.fore = White;
        text.back = DarkCyan;
    }
    else {
        text.fore = White;
        text.back = LightGrey;
    }
}

////////////////////////////////////////////////////////////////////////////////
void clearLeftScreen()
{
    drawRec(dx, dy, dw, dh + 95, text.back);
}

////////////////////////////////////////////////////////////////////////////////
void clearAllScreen()
{
    drawRec(0, 0, WIDTH, HEIGHT, text.back);
}

////////////////////////////////////////////////////////////////////////////////
void clearButtomScreen()
{
    drawRec(dx, dy + dh + 95, dw, 240, text.back);
}

////////////////////////////////////////////////////////////////////////////////
u16 getColor(u8 c)
{
    u16 colorTab[] = {0x0000, 0x000F, 0x03E0, 0x03EF, 0x7800, 0x780F, 0x7BE0, 0xC618,
                      0x7BEF, 0x001F, 0x07E0, 0x07FF, 0xF800, 0xF81F, 0xFFE0, 0xFFFF};
    return colorTab[c];
}

////////////////////////////////////////////////////////////////////////////////
void drawMM(u16 x, u16 y, u16 w)
{
    u16 color = SkyBlue;
    u8  xsub = 1, ysub = 1;

    drawLine(x + 0 * w, y + 1 * w, x + 1 * w, y + 0 * w, color);                              // M0 - M1
    drawLine(x + 1 * w, y + 0 * w, x + 2 * w - xsub, y + 0 * w, color);                       // M1 - M2
    drawLine(x + 2 * w - xsub, y + 0 * w, x + 2 * w - xsub, y + 1 * w + ysub, color);         // M2 - M3
    drawLine(x + 1 * w, y + 2 * w, x + 2 * w, y + 1 * w, color);                              // M3 - M4
    drawLine(x + 1 * w, y + 2 * w, x + 2 * w, y + 3 * w, color);                              // M4 - M5
    drawLine(x + 2 * w, y + 3 * w, x + 3 * w - xsub, y + 2 * w + ysub, color);                // M5 - M6
    drawLine(x + 3 * w - xsub, y + 2 * w + ysub, x + 4 * w - xsub, y + 2 * w + ysub, color);  // M6 - M7
    drawLine(x + 4 * w - xsub, y + 2 * w + ysub, x + 4 * w - xsub, y + 3 * w + ysub, color);  // M7 - M8
    drawLine(x + 3 * w, y + 4 * w, x + 4 * w - xsub, y + 3 * w + ysub, color);                // M8 - M9
    drawLine(x + 3 * w, y + 4 * w, x + 1 * w, y + 4 * w, color);                              // M9 - M10
    drawLine(x + 1 * w, y + 4 * w, x + 1 * w, y + 3 * w, color);                              // M10 - M11
    drawLine(x + 1 * w, y + 3 * w, x + 0 * w, y + 3 * w, color);                              // M11 - M12
    drawLine(x + 0 * w, y + 3 * w, x + 0 * w, y + 1 * w, color);                              // M12 - M0

    color = Yellow;
    drawLine(x + 2 * w + xsub, y + 1 * w - ysub, x + 3 * w, y + 0 * w, color);                // M0 - M1
    drawLine(x + 3 * w, y + 0 * w, x + 5 * w, y + 0 * w, color);                              // M1 - M2
    drawLine(x + 5 * w, y + 0 * w, x + 5 * w, y + 1 * w, color);                              // M2 - M3
    drawLine(x + 5 * w, y + 1 * w, x + 6 * w, y + 1 * w, color);                              // M3 - M4
    drawLine(x + 6 * w, y + 1 * w, x + 6 * w, y + 3 * w, color);                              // M4 - M5
    drawLine(x + 5 * w, y + 4 * w, x + 6 * w, y + 3 * w, color);                              // M5 - M6
    drawLine(x + 4 * w + xsub, y + 4 * w, x + 5 * w, y + 4 * w, color);                       // M6 - M7
    drawLine(x + 4 * w + xsub, y + 4 * w, x + 4 * w + xsub, y + 3 * w - ysub, color);         // M7 - M8
    drawLine(x + 4 * w + xsub, y + 3 * w - ysub, x + 5 * w, y + 2 * w, color);                // M8 - M9
    drawLine(x + 4 * w, y + 1 * w, x + 5 * w, y + 2 * w, color);                              // M9 - M10
    drawLine(x + 3 * w + xsub, y + 2 * w - ysub, x + 4 * w, y + 1 * w, color);                // M10 - M11
    drawLine(x + 2 * w + xsub, y + 2 * w - ysub, x + 3 * w + xsub, y + 2 * w - ysub, color);  // M11 - M12
    drawLine(x + 2 * w + xsub, y + 2 * w - ysub, x + 2 * w + xsub, y + 1 * w - ysub, color);  // M12 - M0

    text.fore = SkyBlue;
    putStr(x + 6 * w, y + 0 * w + 2, 2, 1, "MindMotion");
    putStr(x + 6 * w + 3, y + 1 * w, 0, 1, "Soc Solutions");

    drawLine(198, 225, 315, 225, text.fore);
    putStr(205, 230, 0, 1, "MindMotion Nanjing");
}

////////////////////////////////////////////////////////////////////////////////
void randRefresh()
{
    u16 x, y, w, h, c;
    drawSquare(dx, dy, dw, dh, SPACE, NUL);
    if (drawBlockCnt++ % 2) {
        x = rand();
        x %= (dw - 2);
        y = rand();
        y %= (dh - 2);
        w = rand();
        w %= DMAX;
        h = rand();
        h %= DMAX;
        c = rand();
        c &= 0x0f;
        if ((x + w) > (dw - 2))
            x = dw - w - 2;
        if ((y + h) > (dh - 2))
            y = dh - h - 2;
        drawRec(x + dx + 1, y + dy + 1, w, h, getColor(c));
    }
    else {
        c = rand();
        c &= 0x0f;
        drawRec(dx + 1, dy + 1, dw - 2, dh - 2, getColor(c));
    }
}

////////////////////////////////////////////////////////////////////////////////
void initLcdDemo()
{
    drawSquare(dx, dy, dw, dh, SPACE, NUL);
    color.c1 = LightGrey;
    color.c2 = DarkCyan;
    color.c3 = Black;
    color.c4 = Yellow;
    drawMM(20, 155, 20);
}

////////////////////////////////////////////////////////////////////////////////
void refreshLCD(u8 sele, u16* flag)
{
    if (!*flag)
        return;
    *flag = false;

    text.back = DarkCyan;

    switch (sele) {
        case REFRESH_ADC: dispADC(1); break;
        case REFRESH_TEMP: dispTEMP(1); break;
        case REFRESH_FLASH: dispFLASH(1); break;
        case REFRESH_UART1: dispUART1(1); break;
        case REFRESH_UART2: dispUART2(1); break;
        case REFRESH_CAN: dispCAN(1); break;
        case REFRESH_LED: dispLED(1); break;
        case REFRESH_RTC: dispRTC(1); break;
        case REFRESH_I2C: dispI2C(1); break;
        case REFRESH_MUSIC: dispMUSIC(1); break;
        case REFRESH_SDIO: dispSDIO(1); break;
        case REFRESH_ETH: dispETH(1); break;
        default: break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BSP_LCD_Configure()
{
    initGPIO_LCD();
    initFSMC();

    LCDC_Init_Reg();
    lcdFillColor(White);
    lcdBlcH();
    clearAllScreen();

    initLcdDemo();
}

#endif
