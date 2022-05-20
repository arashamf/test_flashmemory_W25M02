
#ifndef __FLASH_W25M02_H__
#define __FLASH_W25M02_H__

#include "main.h"

#define RESET_MEMORY 0xFF
#define READ_ID 0x9F
#define READ_SR 0x0F
#define WRITE_SR 0x1F
#define PAGE_READ 0x13
#define READ_DATA 0x03
#define FAST_READ 0x0B
#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define BLOCK_ERASE 0xD8
#define DATA_LOAD 0x02
#define EXECUTE 0x10
#define NOP 0xFF
#define SREGISTR_1 0xA0
#define SREGISTR_2 0xB0
#define SREGISTR_3 0xC0

void W25M_ReadData (uint8_t *, uint16_t , uint16_t, uint16_t , uint8_t);
void W25M_WritePage (uint8_t * , uint16_t , uint16_t , uint16_t , uint8_t);
void W25M_EraseSector (uint16_t , uint8_t);
void W25M_EraseChip (uint8_t);
void W25M_ReadID (uint8_t * );
void W25M_Read_SR (uint8_t * , uint8_t );
void W25M_Write_SR (uint8_t , uint8_t , uint8_t);
void W25M_Reset (void);
void W25M_Write_Enable(uint8_t );
void W25M_Write_Disable(uint8_t );
void W25M_WaitForReady(uint8_t ChipNumber);
#endif /* __FLASH_W25M02_H__ */

