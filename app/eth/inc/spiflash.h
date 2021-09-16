////////////////////////////////////////////////////////////////////////////////
#ifndef __SPIFLASH_H_
#define __SPIFLASH_H_
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#ifdef _SPIFLASH_C_
#define GLOBAL


#else
#define GLOBAL extern
#endif


GLOBAL u8 spiId[3];

#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

u8 checkSPIFlashId();
void initGPIO_SPI(SPI_TypeDef* SPIx);
void BSP_FLASH_Configure();

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////




