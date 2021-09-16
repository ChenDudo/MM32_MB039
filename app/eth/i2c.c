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
#define _I2C_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"
#include "main.h" 
#include "hal_i2c.h"
#include "hal_gpio.h"	
#include "hal_nvic.h"
#include "hal_rcc.h"

#include "main.h"
#include "i2c.h"




////////////////////////////////////////////////////////////////////////////////
void initGPIO_I2C(I2C_TypeDef *I2Cx)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    COMMON_EnableIpClock(emCLOCK_GPIOB);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4);

    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}


////////////////////////////////////////////////////////////////////////////////
void NVIC_I2C(I2C_TypeDef* I2Cx)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
    if(I2Cx == I2C1)
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;//check I2C1_EV_IRQn
  
    if(I2Cx == I2C2)
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_IRQn;
  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void i2cInitMaster(I2C_TypeDef *I2Cx, u8 sla, u32 uiI2C_speed) 
{
    I2C_InitTypeDef I2C_InitStruct;

    COMMON_EnableIpClock(emCLOCK_I2C1);

    I2C_StructInit(&I2C_InitStruct);
    
    //Configure I2C as master mode
    I2C_InitStruct.Mode = I2C_CR_MASTER;
    I2C_InitStruct.OwnAddress = 0;
    I2C_InitStruct.Speed = I2C_CR_FAST;
    I2C_InitStruct.ClockSpeed = uiI2C_speed;
    I2C_Init(I2Cx, &I2C_InitStruct);
    
    I2C_Send7bitAddress(I2C1, SLAVE_ADDR, WR);
    I2C_Cmd(I2Cx, ENABLE);
}


////////////////////////////////////////////////////////////////////////////////
void i2cComplete(u16 sta)
{
	I2C_ClearFlag(I2C2,sta);
	I2C_ClearITPendingBit(I2C2, I2C_IT_TX_EMPTY);
	
	i2c.rev = WR;
	i2c.sadd = true;
	i2c.ack = true;
	i2c.busy = false;
}

////////////////////////////////////////////////////////////////////////////////
void I2C2_EV_IRQHandler()
{
//    static  u8 n = 0;		// Rcv length or Send length
//	static  u8 m = 0;		// Rcv length
//	static	u8 _cnt;
//    
//    u16 status = I2C2->IC_RAW_INTR_STAT;
//
//	if (!i2c.busy) {
//		i2cComplete(status);
//	}
//	else if (i2c.rev == RD) {													// Read
//		if (status & I2C_IT_TX_ABRT) {
//			i2cComplete(status);
//			i2c.ack = FALSE;
//		}
//		else {
//			if (status & I2C_IT_TX_EMPTY) {
//				(m++ < (i2c.cnt - 1)) ? I2C_ReadCmd(I2C2) : I2C_ClearITPendingBit(I2C2, I2C_IT_TX_EMPTY);
//			}
//			
//			if (status & I2C_IT_RX_FULL) {
//				*i2c.ptr++ = I2C_ReceiveData(I2C2);
//				
//				if  ((n++ >= (i2c.cnt - 1))){
//					error.i2c = FALSE;
//					rf.i2c = TRUE;
//					i2cComplete(status);
//				}
//			}
//		}
//	}
//	else if (status & I2C_IT_TX_EMPTY){
//		if (i2c.sadd) {															// send slave address
//			I2C_SendData(I2C2, i2c.sub);								
//			i2c.sadd = FALSE;
//			 m = 0; n = 0;
//			_cnt = (i2c.opt == RD) ?  0 : i2c.cnt;								// cnt = 0 if RD , eles _cnt = i2c.cnt
//		}
//		else if (n < _cnt) {													// write
//			I2C_SendData(I2C2, *i2c.ptr++);	
//			n++;
//		}
//		else if  (status &  I2C_IT_STOP_DET) {
//			if (i2c.opt == WR) {
//				i2cComplete(status);
//			}
//			else {
//				i2c.rev = i2c.opt;
//				I2C_ReadCmd(I2C2);
//				I2C_ITConfig(I2C2, I2C_IT_RX_FULL | I2C_IT_RX_OVER , ENABLE);
//			}
//		}
//	}
}

////////////////////////////////////////////////////////////////////////////////
void I2C2_ER_IRQHandler()
{
    if(I2C_GetITStatus(I2C2, I2C_IT_STOP_DET)){
        I2C_ClearITPendingBit(I2C2, I2C_IT_STOP_DET);
	}
}

////////////////////////////////////////////////////////////////////////////////
void i2cAccess(u8 sub, u8* ptr, u16 cnt)
{
	i2c.sub = sub;
	i2c.cnt = cnt;
	i2c.ptr = ptr;

	i2c.busy = true;
	i2c.ack = false;
	i2c.error = false;
	i2c.sadd = true;
	i2c.rev = WR;
	I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY | I2C_IT_STOP_DET | I2C_IT_START_DET | I2C_IT_ACTIVITY, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
void i2cSendPacket(u8 sub, u8* ptr, u16 cnt)
{
	i2c.opt = WR;
	i2cAccess(sub,  ptr,  cnt);
}
	 
////////////////////////////////////////////////////////////////////////////////
void i2cRcvPacket(u8 sub, u8* ptr, u16 cnt )
{
	i2c.opt = RD;
	i2cAccess(sub,  ptr,  cnt);
}

////////////////////////////////////////////////////////////////////////////////
void i2cRead(u8 sub, u8* ptr, u16 len)
{
	do{	
		i2cRcvPacket(sub, ptr, len);	
		while(i2c.busy);
	} 
	while(!i2c.ack);
}

////////////////////////////////////////////////////////////////////////////////
void i2cWrite(u8 sub, u8* ptr, u16 len)
{
	do{	
		i2cSendPacket(sub, ptr, len);	
		while(i2c.busy);
	} 
	while(!i2c.ack);
}

////////////////////////////////////////////////////////////////////////////////
void i2cCheck()
{
	i2cRcvPacket(0, &i2cRx[0], 8);	
	while(!i2c.ack);
	
	if (memcmp(i2cRx, i2cTx, 8) != 0) {
		i2cSendPacket(0, &i2cTx[0], 8);
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief  Wait for EEPROM getting ready.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_WaitEEready(void)
{
    //eeprom operation interval delay
    u32 i = 10000;
    while(i--);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send a byte.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_WriteByte(u8 dat)
{
    //Send data
    I2C_SendData(I2C1, dat);
    //Checks whether transmit FIFO completely empty or not
    while(I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE) == 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive a byte.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_ReadBuff(void)
{
    u8 i, flag = 0, _cnt = 0;
    for (i = 0; i < i2c.cnt; i++) {
        while(1) {
            //Write command is sent when RX FIFO is not full
            if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0)) {
                //Configure to read
                I2C_ReadCmd(I2C1);
                _cnt++;
                //When flag is set, receive complete
                if (_cnt == i2c.cnt)
                    flag = 1;
            }
            //Check receive FIFO not empty
            if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE)) {
                //read data to gEeprom.ptr
                i2c.ptr[i] = I2C_ReceiveData(I2C1);
                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Send bytes
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
u8 EEPROM_WriteBuff(u8 sub, u8* ptr, u16 cnt)
{
    //Send sub address
    EEPROM_WriteByte(sub);
    while (cnt --) {
        //Send data
        EEPROM_WriteByte(*ptr);
        //Point to the next data
        ptr++;
    }
    //Stop transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);
    i2c.ack = true;
    //I2C operation stops
    i2c.busy = false;
    //Wait for EEPROM getting ready.
    EEPROM_WaitEEready();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send a data packet
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : The state of this transmission.
////////////////////////////////////////////////////////////////////////////////
u8 EEPROM_SendPacket(u8 sub, u8* ptr, u16 cnt)
{
    u8 i;
    //i2c option flag set to write
    i2c.opt = WR;
    //number to send
    i2c.cnt = cnt;
    //sub address
    i2c.sub = sub;
    //I2C operation starts
    i2c.busy = true;
    i2c.ack = false;

    if ((sub % PAGESIZE) > 0) {
        //Need temp number of data, just right to the page address
        u8 temp = MIN((PAGESIZE - sub % PAGESIZE), i2c.cnt);
        //If WRITE successful
        if(EEPROM_WriteBuff(sub, ptr, temp)) {
            //Point to the next page
            ptr +=  temp;
            i2c.cnt -=  temp;
            sub += temp;
        }
        //gEeprom.cnt = 0 means transmition complete
        if (i2c.cnt == 0) return true;
    }
    for (i = 0; i < (i2c.cnt / PAGESIZE); i++) {
        //Full page write
        if (EEPROM_WriteBuff(sub, ptr, PAGESIZE)) {
            //Point to the next page
            ptr += PAGESIZE;
            sub += PAGESIZE;
            i2c.cnt -= PAGESIZE;
        }
        if (i2c.cnt == 0) return true;
    }
    if (i2c.cnt > 0) {
        if (EEPROM_WriteBuff(sub, ptr, i2c.cnt)) return true;
    }
    //I2C operation ends
    i2c.busy = false;
    i2c.ack = true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive a data packet
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_ReadPacket(u8 sub, u8* ptr, u16 cnt)
{
    //I2C operation starts
    i2c.busy = true;
    i2c.ack = false;
    i2c.sub = sub;
    i2c.ptr = ptr;
    i2c.cnt = cnt;

    //Send sub address
    EEPROM_WriteByte(i2c.sub);
    //receive bytes
    EEPROM_ReadBuff();
    //Stop transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);

    //I2C operation ends
    i2c.busy = false;
    i2c.ack = true;
    EEPROM_WaitEEready();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EEPROM_Write
/// @note   Write a data packet.
/// @param  : sub (Sub address of EEPROM)
/// @param  : ptr (Data in the buffer)
/// @param  : len (Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Write(u8 sub, u8* ptr, u16 len)
{
    do {
        EEPROM_SendPacket(sub, ptr, len);
        while(i2c.busy);
    } while(!i2c.ack);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EEPROM_Read
/// @note   Receive a data packet.
/// @param  : sub (Sub address of EEPROM)
/// @param  : ptr (Buffer to storage data)
/// @param  : len (Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Read(u8 sub, u8* ptr, u16 len)
{
    do {
        //read data
        EEPROM_ReadPacket(sub, ptr, len);
        //till I2C is not work
        while(i2c.busy);
    } while(!i2c.ack);
}



////////////////////////////////////////////////////////////////////////////////
void BSP_I2C_Configure()
{
    initGPIO_I2C(I2C1);
    i2cInitMaster(I2C1, SLAVE_ADDR, 100000);
    
    EEPROM_Read(0x00, i2cRx, 8);
    
    if (memcmp(i2cRx, i2cTx, 8) != 0) {
        EEPROM_Write(0x00, i2cTx, 8);
	}
}
