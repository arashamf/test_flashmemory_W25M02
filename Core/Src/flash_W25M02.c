
#include "spi.h"
#include "flash_W25M02.h"
#include "gpio.h"
#include "delay.h"
#include "usart.h"

extern char UART3_msg_TX [RS232_BUFFER_SIZE];

//*****************************************************************************************************************************************************//
void W25M_ReadData (uint8_t * RX_buffer, uint16_t DataSize, uint16_t ColumnNumber, uint16_t PageNumber, uint8_t ChipNumber)
{
	
	uint8_t buffer [] = {PAGE_READ, NOP, (uint8_t) (PageNumber >> 8), (uint8_t) (PageNumber), READ_DATA, (uint8_t)(ColumnNumber >> 8), (uint8_t)(ColumnNumber), NOP};
	SPI4_CS_ON (ChipNumber); 
	HAL_SPI_Transmit (&hspi4, buffer, 4, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);
	
	W25M_WaitForReady(ChipNumber);
		
	SPI4_CS_ON (ChipNumber); 
	HAL_SPI_Transmit (&hspi4, buffer+4, 4, 0xFFFF);
	HAL_SPI_Receive (&hspi4, RX_buffer, DataSize, 0xFFFF); 
	SPI4_CS_OFF (ChipNumber);
	UART3_SendString ((char*)RX_buffer);
}

//*****************************************************************************************************************************************************//
void W25M_WritePage (uint8_t * Data, uint16_t DataSize, uint16_t ColumnNumber, uint16_t PageNumber, uint8_t ChipNumber)
{
	
	uint8_t buffer [] = {DATA_LOAD, (uint8_t)(ColumnNumber >> 8), (uint8_t)(ColumnNumber), EXECUTE, NOP, (uint8_t) (PageNumber >> 8), (uint8_t) (PageNumber)};
		
	W25M_Write_Enable (ChipNumber);
	
	uint8_t READ_STATUS_3 [] = {READ_SR, SREGISTR_3};
	uint8_t status = 0;
	SPI4_CS_ON (ChipNumber);
	HAL_SPI_Transmit(&hspi4, READ_STATUS_3, 2, 0xFFFF);
	HAL_SPI_Receive(&hspi4, &status, 1, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);
	sprintf (UART3_msg_TX,"SR3=%x \r\n", status);		
	UART3_SendString ((char*)UART3_msg_TX);		
		
	SPI4_CS_ON (ChipNumber);	
	HAL_SPI_Transmit (&hspi4, buffer, 3, 0xFFFF);
	HAL_SPI_Transmit (&hspi4, Data, DataSize, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);
		
	SPI4_CS_ON (ChipNumber);	
	HAL_SPI_Transmit (&hspi4, (buffer+3), 4, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);		
		
	W25M_WaitForReady(ChipNumber);
}

//*****************************************************************************************************************************************************//
void W25M_EraseSector (uint16_t PageNumb, uint8_t ChipNumber)
{
	uint8_t buffer [] = {BLOCK_ERASE, NOP, (uint8_t)(PageNumb >> 8), (uint8_t)(PageNumb)};
	
	W25M_Write_Enable (ChipNumber);
	
	SPI4_CS_ON (ChipNumber);
	HAL_SPI_Transmit (&hspi4, buffer, 4, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);
	
	W25M_Write_Disable (ChipNumber);
	
	W25M_WaitForReady(ChipNumber);
}

//*****************************************************************************************************************************************************//
void W25M_EraseChip (uint8_t ChipNumber)
{
	
}

//*****************************************************************************************************************************************************//
void W25M_ReadID (uint8_t * RX_buffer)
{
	uint8_t buffer = READ_ID;
	uint8_t empty = 0;
	SPI4_CS1_ON;
//	HAL_SPI_Transmit_DMA (&hspi4, &buffer, 1);
//	HAL_SPI_Receive_DMA (&hspi4, rx_buffer, 3); 
	HAL_SPI_Transmit (&hspi4, &buffer, 1, 0xFFFF);
	HAL_SPI_Receive (&hspi4, &empty, 1, 0xFFFF); 
	HAL_SPI_Receive (&hspi4, RX_buffer, 3, 0xFFFF); 
	SPI4_CS1_OFF;
	
	SPI4_CS2_ON;
//	HAL_SPI_Transmit_DMA (&hspi4, &buffer, 1);
	//HAL_SPI_Receive_DMA (&hspi4, rx_buffer+3, 3);
	HAL_SPI_Transmit (&hspi4, &buffer, 1, 0xFFFF);
	HAL_SPI_Receive (&hspi4, &empty, 1, 0xFFFF); 
	HAL_SPI_Receive (&hspi4, RX_buffer+3, 3, 0xFFFF); 
	SPI4_CS2_OFF;
}

//*****************************************************************************************************************************************************//
void W25M_Read_SR (uint8_t * RX_buffer, uint8_t ChipNumber)
{
	uint8_t READ_STATUS_1 [] = {READ_SR, SREGISTR_1};
	uint8_t READ_STATUS_2 [] = {READ_SR, SREGISTR_2};
	uint8_t READ_STATUS_3 [] = {READ_SR, SREGISTR_3};
	
	SPI4_CS_ON (ChipNumber);
	HAL_SPI_Transmit(&hspi4, READ_STATUS_1, 2, 0xFFFF);
	HAL_SPI_Receive(&hspi4, RX_buffer, 1, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);
	
	delay_us (5);
	
	SPI4_CS_ON (ChipNumber);
	HAL_SPI_Transmit(&hspi4, READ_STATUS_2, 2, 0xFFFF);
	HAL_SPI_Receive(&hspi4, RX_buffer+1, 1, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);
	
	delay_us (5);
	
	SPI4_CS_ON (ChipNumber);
	HAL_SPI_Transmit(&hspi4, READ_STATUS_3, 2, 0xFFFF);
	HAL_SPI_Receive(&hspi4, RX_buffer+2, 1, 0xFFFF);
	SPI4_CS_OFF (ChipNumber);	
	
}

//*****************************************************************************************************************************************************//
void W25M_Write_SR (uint8_t Status_Registr, uint8_t New_Status, uint8_t ChipNumber)
{
	uint8_t buffer [] = {WRITE_SR, Status_Registr, New_Status};
	SPI4_CS_ON (ChipNumber);
	HAL_SPI_Transmit(&hspi4, buffer, 3, 0xFFFF);

	SPI4_CS_OFF (ChipNumber);
	
}

//*****************************************************************************************************************************************************//
void W25M_Reset (void)
{
	uint8_t buf =  RESET_MEMORY;
	SPI4_CS1_ON;
	HAL_SPI_Transmit (&hspi4, &buf, 1, 0xFFFF);
	SPI4_CS1_OFF;
	SPI4_CS2_ON;
	HAL_SPI_Transmit (&hspi4, &buf, 1, 0xFFFF);
	SPI4_CS2_OFF;
	HAL_Delay(5);
}

//*****************************************************************************************************************************************************//
void W25M_Write_Enable(uint8_t ChipNumber)
{
	SPI4_CS_ON (ChipNumber); 
  uint8_t buf = WRITE_ENABLE;
	HAL_SPI_Transmit (&hspi4, &buf, 1, 0xFFFF);
	SPI4_CS_OFF (ChipNumber); 
}

//*****************************************************************************************************************************************************//
void W25M_Write_Disable(uint8_t ChipNumber)
{
	SPI4_CS_ON (ChipNumber); 
  uint8_t buf = WRITE_DISABLE;
	HAL_SPI_Transmit (&hspi4, &buf, 1, 0xFFFF);
	SPI4_CS_OFF (ChipNumber); 
}
//*****************************************************************************************************************************************************//
// Чтение бита занятости
void W25M_WaitForReady(uint8_t ChipNumber)
{
  uint8_t READ_STATUS_3 [] = {READ_SR, SREGISTR_3};
  uint8_t status = 0;

    
  do
  {
    status = 0;
    
		SPI4_CS_ON (ChipNumber);
		
		HAL_SPI_Transmit(&hspi4, READ_STATUS_3, 2, 0xFFFF); // Передать команду чтения статусного регистра  
		HAL_SPI_Receive(&hspi4, &status, 1, 0xFFFF); // Прочитать статусный регистр
		
//		sprintf (UART3_msg_TX,"SR3=%x \r\n", status);
//		UART3_SendString ((char*)UART3_msg_TX);	
		
		SPI4_CS_OFF (ChipNumber);
  }
  while (status & 0x01);      
}
