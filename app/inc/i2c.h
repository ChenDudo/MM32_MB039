////////////////////////////////////////////////////////////////////////////////
#ifndef __I2C_H_
#define __I2C_H_
////////////////////////////////////////////////////////////////////////////////

#define PAGESIZE 16

#if defined(__FM24C02)
#define SLAVE_ADDR 0xA0
#else
#define SLAVE_ADDR 0xA8
#endif

typedef struct {
    u8  busy;
    u8  ack;
    u8  error;
    u8  opt;
    u8  sub;
    u8  cnt;
    u8* ptr;
    u8  rev;
    u8  sadd;
} I2C_def;

////////////////////////////////////////////////////////////////////////////////
#ifdef _I2C_C_
#define GLOBAL

enum { WR, RD };

u8 i2cTx[] = {0x81, 0x81, 0x81, 0xF0, 0xee, 0xff, 0x11, 0x22};
u8 i2cRx[] = {0, 0, 0, 0, 0, 0, 0, 0};

#else
#define GLOBAL extern

extern u8 i2cTx[8];
extern u8 i2cRx[8];

#endif

GLOBAL I2C_def i2c;

#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

void BSP_I2C_Configure();

void i2cInitMaster(I2C_TypeDef* I2Cx, u8 sla, u32 speed);
void i2cSendPacket(u8 sub, u8* ptr, u16 cnt);
void i2cRcvPacket(u8 sub, u8* ptr, u16 cnt);

void initGPIO_I2C(I2C_TypeDef* I2Cx);
void NVIC_I2C(I2C_TypeDef* I2Cx);
void initI2C();

void EEPROM_Write(u8 sub, u8* ptr, u16 len);
void EEPROM_Read(u8 sub, u8* ptr, u16 len);
void EEPROM_WaitEEready(void);
void EEPROM_WriteByte(u8 dat);
u8   EEPROM_WriteBuff(u8 sub, u8* ptr, u16 cnt);
u8   EEPROM_SendPacket(u8 sub, u8* ptr, u16 cnt);
void EEPROM_ReadBuff(void);
void EEPROM_ReadPacket(u8 sub, u8* ptr, u16 cnt);
void I2C_WR_EepromInit(void);
void I2C_WR_EepromTest(void);

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
