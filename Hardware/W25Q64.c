#include "W25Q64.h"
#include "Serial.h"
#include <stdio.h>

volatile uint8_t W25Q64_Buffer[4096] = {0};

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	SPI2_Start();
	Serial_Printf("SPI2_Start");
	SPI2_SwapByte(W25Q64_JEDEC_ID);	
	Serial_Printf("SPI2_swap");
	*MID = SPI2_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = SPI2_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= SPI2_SwapByte(W25Q64_DUMMY_BYTE);
	SPI2_Stop();
}

void W25Q64_WriteEnable(void)
{
	SPI2_Start();
	SPI2_SwapByte(W25Q64_WRITE_ENABLE);
	SPI2_Stop();
}

void W25Q64_WaitBusy(void)
{
	//uint32_t Timeout;
	SPI2_Start();
	SPI2_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	//Timeout = 100000;
	while ((SPI2_SwapByte(W25Q64_DUMMY_BYTE) & 0x01));
//	while ((SPI2_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
//	{
//		Timeout --;
//		if (Timeout == 0)
//		{
//			break;
//		}
//	}
	SPI2_Stop();
}

void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	SPI2_Start();
	SPI2_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	SPI2_SwapByte(Address >> 16);
	SPI2_SwapByte(Address >> 8);
	SPI2_SwapByte(Address);
	SPI2_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_Wirte_Protect_Disable(void)
{
	SPI2_Start();
	SPI2_SwapByte(W25Q64_GLOBAL_BLOCK_UNLOCK);
	SPI2_Stop();
	
	W25Q64_WriteEnable();
	SPI2_Start();
	SPI2_SwapByte(W25Q64_WRITE_STATUS_REGISTER);
	SPI2_SwapByte(0x00);		
	SPI2_SwapByte(0x00);
	SPI2_SwapByte(0x00);
	W25Q64_WaitBusy();
	SPI2_Stop();
	
//	printf("%X\n",SPI2_SwapByte(W25Q64_READ_STATUS_REGISTER_1));
}



void W25Q64_Write_Sector(uint32_t Address)
{
	// Erase the sector
	W25Q64_SectorErase(Address);
	// Enable write once before writing all pages
	volatile uint16_t i, j;
	volatile uint32_t Temp_Address;

	for (j = 0; j < 16; j++){
		W25Q64_WriteEnable();
		SPI2_Start();
		Temp_Address = Address + j * 0x000100;

		// Send the page program command and address
		SPI2_SwapByte(W25Q64_PAGE_PROGRAM);
		SPI2_SwapByte(Temp_Address >> 16);
		SPI2_SwapByte(Temp_Address >> 8);
		SPI2_SwapByte(Temp_Address);

		// Write data to the SPI flash
		for (i = 0; i < 256; i++){
			SPI2_SwapByte(W25Q64_Buffer[256 * j + i]);
		}

		SPI2_Stop();

		// Wait until the write operation is complete
		W25Q64_WaitBusy();
	}
}

//void W25Q64_Write_Sector(uint32_t Address)
//{
//	// Erase the sector
//	W25Q64_SectorErase(Address);
//	// Enable write once before writing all pages
//	uint16_t i;
//	//W25Q64_Wirte_Protect_Disable();
//		W25Q64_WriteEnable();
//		SPI2_Start();
//		
//		// Send the page program command and address
//		SPI2_SwapByte(W25Q64_PAGE_PROGRAM);
//		SPI2_SwapByte(Address >> 16);
//		SPI2_SwapByte(Address >> 8);
//		SPI2_SwapByte(Address);
//		// Write data to the SPI flash
//		for (i = 0; i < 256; i++){
//			SPI2_SwapByte(W25Q64_Buffer[i]);
//			printf("%d\n",i);
//		}

//		SPI2_Stop();
//		// Wait until the write operation is complete
//		W25Q64_WaitBusy();
//}

void W25Q64_Read_Sector(uint32_t Address)
{
	volatile uint16_t i;
	SPI2_Start();
	// Send the read command and address
	SPI2_SwapByte(W25Q64_READ_DATA);
	SPI2_SwapByte(Address >> 16);
	SPI2_SwapByte(Address >> 8);
	SPI2_SwapByte(Address);

	// Read 4KB into the buffer
	for (i = 0; i < 4096; i++) {
		W25Q64_Buffer[i] = SPI2_SwapByte(W25Q64_DUMMY_BYTE);
	}

	SPI2_Stop();
}

void W25Q64_Print_Buffer(void)
{
	volatile uint16_t i;
	for (i = 0; i < 256; i ++)		
	{
//		printf("%d:%d\n",i,W25Q64_Buffer[i]);
		Serial_Printf("%d:%d\n",i,W25Q64_Buffer[i]);
	}
}


uint16_t W25Q64_Array2Buffer(const uint16_t *data, uint16_t count)
{
	uint16_t capacity = (4096u - 2u) / 2u;
	if (count > capacity) count = capacity;

	W25Q64_Buffer[0] = (uint8_t)(count & 0xFFu);
	W25Q64_Buffer[1] = (uint8_t)((count >> 8) & 0xFFu);

	uint16_t i;
	for (i = 0; i < count; i++)
	{
		uint8_t lo = (uint8_t)(data[i] & 0xFFu);
		uint8_t hi = (uint8_t)((data[i] >> 8) & 0xFFu);
		W25Q64_Buffer[2u + i*2u] = lo;
		W25Q64_Buffer[3u + i*2u] = hi;
	}
	return count;
}

uint16_t W25Q64_Buffer2Array(uint16_t *out, uint16_t maxCount)
{
	uint16_t len = (uint16_t)(((uint16_t)W25Q64_Buffer[1] << 8) | (uint16_t)W25Q64_Buffer[0]);
	uint16_t capacity = (4096u - 2u) / 2u;
	if (len > capacity) len = capacity;
	if (len > maxCount) len = maxCount;
	
	uint16_t i;
	for (i = 0; i < len; i++)
	{
		
		uint8_t lo = W25Q64_Buffer[2u + i*2u];
		uint8_t hi = W25Q64_Buffer[3u + i*2u];
		out[i] = (uint16_t)(((uint16_t)hi << 8) | (uint16_t)lo);
	}

	return len;
}

void W25Q64_Init(void){
	SPI2_Init();
}

