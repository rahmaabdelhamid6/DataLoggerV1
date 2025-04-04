/*
 * hal_EEPROM.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */


/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "hal_EEPROM.h"
#include "hal_PC.h"
#include "hal_RTC.h"
#include "string.h"
#include <stdio.h>
#include "usbd_cdc_if.h"

/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/
uint8_t g_aui8EEPROMTxBuffer[HAL_EEPROM_PAGE_SIZE]={0};
uint8_t g_aui8EEPROMRxBuffer[HAL_EEPROM_PAGE_SIZE]={0};
uint16_t g_ui16EEPROMLastAddress = HAL_EEPROM_DATA_START_ADDR;

/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/**
 * @brief  Write a single byte in blocking mode to a specific memory address in the EEPROM
 * @param  ui16ByteNumber Internal memory byte address (0 to HAL_EEPROM_SIZE - 1)
 * @param  ui8WriteData Data byte to be written
 * @retval HALEepromStatus_et status:
 *           - HAL_EEPROM_SUCCESS: Operation successful
 *           - HAL_EEPROM_ERROR_ADDRESS: Invalid byte address
 *           - HAL_EEPROM_ERROR_INTERFACE: I2C communication error
 */
HALEepromStatus_et HAL_EEPROM_WriteByte(uint16_t ui16ByteNumber,uint8_t ui8WriteData)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	if (ui16ByteNumber >= HAL_EEPROM_SIZE)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_ADDRESS;
		return etHALEepromStatus;
	}
	if (HAL_I2C_Mem_Write(&hi2c2, HAL_EEPROM_ADDR, ui16ByteNumber, I2C_MEMADD_SIZE_16BIT, (uint8_t *)&ui8WriteData, sizeof(ui8WriteData), HAL_MAX_DELAY) !=  HAL_OK)
	{

		etHALEepromStatus =  HAL_EEPROM_ERROR_INTERFACE;
		return etHALEepromStatus;
	}
	return etHALEepromStatus;
}
/**
 * @brief  Write a page of data in blocking mode to a specific page in the EEPROM
 * @param  ui16PageNumber Page number to write to (0 to HAL_EEPROM_PAGE_NUMS - 1)
 * @param  aui8TxBuffer Pointer to the data buffer containing bytes to write
 * @param  ui8TxBufSize Number of bytes to write (must not exceed HAL_EEPROM_PAGE_SIZE)
 * @retval HALEepromStatus_et status:
 *           - HAL_EEPROM_SUCCESS: Operation successful
 *           - HAL_EEPROM_ERROR_PAGE_ADDRESS: Invalid page number
 *           - HAL_EEPROM_ERROR_BUFFER_SIZE: Buffer size exceeds page size
 *           - HAL_EEPROM_ERROR_INTERFACE: I2C communication error
 */
HALEepromStatus_et HAL_EEPROM_WritePage(uint16_t ui16PageNumber, uint8_t aui8TxBuffer[], uint8_t ui8TxBufSize)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	uint16_t ui16PageAddress = 0;

	if (ui16PageNumber > HAL_EEPROM_PAGE_NUMS)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_PAGE_ADDRESS;
		return etHALEepromStatus;
	}
	// Calculate the starting byte address from the page number
	ui16PageAddress = ui16PageNumber * HAL_EEPROM_PAGE_SIZE;

	// Check if buffer size is valid (should not exceed page size)
	if (ui8TxBufSize > HAL_EEPROM_PAGE_SIZE)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_BUFFER_SIZE;
		return etHALEepromStatus;
	}
	if (HAL_I2C_Mem_Write(&hi2c2, HAL_EEPROM_ADDR, ui16PageAddress, I2C_MEMADD_SIZE_16BIT, aui8TxBuffer, ui8TxBufSize, HAL_MAX_DELAY) !=  HAL_OK)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_INTERFACE;
		return etHALEepromStatus;
	}
	return etHALEepromStatus;
}
/**
 * @brief  Read a single byte in blocking mode from a specific memory address in the EEPROM
 * @param  ui16ByteNumber Internal memory byte address (0 to HAL_EEPROM_SIZE - 1)
 * @param  ui8ReadData Pointer to variable where the read byte will be stored
 * @retval HALEepromStatus_et status:
 *           - HAL_EEPROM_SUCCESS: Operation successful
 *           - HAL_EEPROM_ERROR_ADDRESS: Invalid byte address
 *           - HAL_EEPROM_ERROR_INTERFACE: I2C communication error
 */
HALEepromStatus_et HAL_EEPROM_ReadByte(uint16_t ui16ByteNumber, uint8_t *ui8ReadData)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	if (ui16ByteNumber >= HAL_EEPROM_SIZE)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_ADDRESS;
		return etHALEepromStatus;
	}
	if (HAL_I2C_Mem_Read(&hi2c2, HAL_EEPROM_ADDR, ui16ByteNumber, I2C_MEMADD_SIZE_16BIT, ui8ReadData, sizeof(ui8ReadData), HAL_MAX_DELAY) !=  HAL_OK)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_INTERFACE;
		return etHALEepromStatus;
	}

	return etHALEepromStatus;
}
/**
 * @brief  Read a page of data in blocking mode from a specific page in the EEPROM
 * @param  ui16PageNumber Page number to read from (0 to HAL_EEPROM_PAGE_NUMS - 1)
 * @param  aui8RxBuffer Pointer to the data buffer where read bytes will be stored
 * @param  ui8RxBufSize Number of bytes to read (must not exceed HAL_EEPROM_PAGE_SIZE)
 * @retval HALEepromStatus_et status:
 *           - HAL_EEPROM_SUCCESS: Operation successful
 *           - HAL_EEPROM_ERROR_PAGE_ADDRESS: Invalid page number
 *           - HAL_EEPROM_ERROR_BUFFER_SIZE: Buffer size exceeds page size
 *           - HAL_EEPROM_ERROR_INTERFACE: I2C communication error
 */
HALEepromStatus_et HAL_EEPROM_ReadPage(uint16_t ui16PageNumber, uint8_t aui8RxBuffer[], uint8_t ui8RxBufSize)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	// Calculate the starting byte address from the page number
	uint16_t ui16PageAddress = 0;

	if (ui16PageNumber > HAL_EEPROM_PAGE_NUMS)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_PAGE_ADDRESS;
		return etHALEepromStatus;
	}
	// Calculate the starting byte address from the page number
	ui16PageAddress = ui16PageNumber * HAL_EEPROM_PAGE_SIZE;

	// Check if buffer size is valid (should not exceed page size)
	if (ui8RxBufSize > HAL_EEPROM_PAGE_SIZE)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_BUFFER_SIZE;
		return etHALEepromStatus;
	}
	if (HAL_I2C_Mem_Read(&hi2c2, HAL_EEPROM_ADDR, ui16PageAddress, I2C_MEMADD_SIZE_16BIT, aui8RxBuffer, ui8RxBufSize, HAL_MAX_DELAY) !=  HAL_OK)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_INTERFACE;
		return etHALEepromStatus;
	}

	return etHALEepromStatus;
}
/**
  * @brief  Write an amount of data to a specific EEPROM address
  * @param  ui16EepromAdd EEPROM memory address to write to
  * @param  ui8WriteBuff Pointer to the data buffer to be written
  * @param  ui16WriteBuffSize Size of the data buffer to be written
  * @retval HAL EEPROM status
  */
HALEepromStatus_et HAL_EEPROM_Write(uint16_t ui16EepromAdd, uint8_t* ui8WriteBuff, uint16_t ui16WriteBuffSize)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	if (ui16WriteBuffSize >= HAL_EEPROM_SIZE)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_ADDRESS;
		return etHALEepromStatus;
	}
	if (HAL_I2C_Mem_Write(&hi2c2, HAL_EEPROM_ADDR, ui16EepromAdd, I2C_MEMADD_SIZE_16BIT, ui8WriteBuff, ui16WriteBuffSize, HAL_MAX_DELAY) !=  HAL_OK)
	{

		etHALEepromStatus =  HAL_EEPROM_ERROR_INTERFACE;
		return etHALEepromStatus;
	}
	return etHALEepromStatus;
}
/**
  * @brief  Read an amount of data from a specific EEPROM address
  * @param  ui16EepromAdd EEPROM memory address to read from
  * @param  ui8ReadBuff Pointer to the data buffer to store the read data
  * @param  ui16ReadBuffSize Size of the data buffer to be read
  * @retval HAL EEPROM status
  */
HALEepromStatus_et HAL_EEPROM_Read(uint16_t ui16EepromAdd ,uint8_t* ui8ReadBuff, uint16_t ui16ReadBuffSize)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	if (ui16ReadBuffSize >= HAL_EEPROM_SIZE)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_ADDRESS;
		return etHALEepromStatus;
	}
	if (HAL_I2C_Mem_Read(&hi2c2, HAL_EEPROM_ADDR, ui16EepromAdd, I2C_MEMADD_SIZE_16BIT, ui8ReadBuff, ui16ReadBuffSize, HAL_MAX_DELAY) !=  HAL_OK)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_INTERFACE;
		return etHALEepromStatus;
	}
	return etHALEepromStatus;
}

/**
  * @brief  Save the last used EEPROM address to a predefined location (fist two byte in the EEPROM)
  * @retval HAL EEPROM status
  */
HALEepromStatus_et  HAL_EEPROM_SaveLastAddress(void)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	uint8_t aui8AddrData[2] = { (g_ui16EEPROMLastAddress >> 8) & 0xFF, g_ui16EEPROMLastAddress & 0xFF };

	if (HAL_EEPROM_Write(HAL_EEPROM_LAST_ADDRESS_LOC, aui8AddrData, sizeof(aui8AddrData))!=  HAL_EEPROM_SUCCESS)
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_ADDRESS;
		return etHALEepromStatus;
	}
	return etHALEepromStatus;
}
/**
  * @brief  Load the last used EEPROM address from a predefined location
  * @retval HAL EEPROM status
  */
HALEepromStatus_et HAL_EEPROM_LoadLastAddress(void)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;
	uint8_t aui8AddrData[2];
	if (HAL_EEPROM_Read(HAL_EEPROM_LAST_ADDRESS_LOC, aui8AddrData, sizeof(aui8AddrData)) ==  HAL_EEPROM_SUCCESS)
	{
		etHALEepromStatus = HAL_EEPROM_SUCCESS;
		g_ui16EEPROMLastAddress = (aui8AddrData[0] << 8) | aui8AddrData[1];
	}
	else
	{
		etHALEepromStatus =  HAL_EEPROM_ERROR_ADDRESS;
		return etHALEepromStatus;
	}
	return etHALEepromStatus;
}
/**
  * @brief  Write temperature and humidity data to EEPROM with a timestamp
  * @param  fTemperature Temperature value to be stored
  * @param  fHumidity Humidity value to be stored
  * @retval HAL EEPROM status
  */
HAL_StatusTypeDef HAL_EEPROM_Write_DateTime(float fTemperature, float fHumidity)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;
	char cDateTimeBuff[32];
	int i32length = 0;

	HAL_RTC_Get_TimeDate();

	i32length = snprintf(cDateTimeBuff, sizeof(cDateTimeBuff), "%02d-%02d-%02d %02d:%02d:%02d,%.2f,%.2f,",
			g_sTimeStampDate.Year + 2000, g_sTimeStampDate.Month, g_sTimeStampDate.Date,
			g_sTimeStampTime.Hours, g_sTimeStampTime.Minutes, g_sTimeStampTime.Seconds,
			fTemperature, fHumidity);

	if (g_ui16EEPROMLastAddress + i32length >= HAL_EEPROM_SIZE)
	{
		return HAL_EEPROM_FULL_SIZE; // EEPROM Full
	}

	etHALEepromStatus = HAL_EEPROM_Write(g_ui16EEPROMLastAddress, (uint8_t *)cDateTimeBuff, sizeof(cDateTimeBuff));

	HAL_Delay(10); // EEPROM write cycle delay

	if (etHALEepromStatus == HAL_EEPROM_SUCCESS)
	{
		g_ui16EEPROMLastAddress += i32length;
		HAL_EEPROM_SaveLastAddress(); // Save updated address
	}
	else
	{
		return etHALEepromStatus;
	}
	HAL_Delay(10); // EEPROM write cycle delay

	return etHALEepromStatus;
}
/**
  * @brief  Stream all stored EEPROM data over USB
  * @retval HAL EEPROM status
  */
HALEepromStatus_et HAL_EEPROM_Stream_Data(void)
{
	HALEepromStatus_et etHALEepromStatus = HAL_EEPROM_SUCCESS;

	uint32_t ui32TotalBytes = (g_ui16EEPROMLastAddress - HAL_EEPROM_DATA_START_ADDR);
	uint32_t ui32EepromAddr = HAL_EEPROM_DATA_START_ADDR;
	uint8_t ui8UsbBuffer[HAL_EEPROM_USB_CHUNK_SIZE];
	uint16_t ui16UsbIndex = 0;
	char cReadBuffer[HAL_EEPROM_READING_SIZE];

	HAL_EEPROM_LoadLastAddress();

	while (ui32EepromAddr < ui32TotalBytes)
	{
		// Read one reading (32 bytes)
		if (HAL_EEPROM_Read(ui32EepromAddr, (uint8_t *)cReadBuffer,  HAL_EEPROM_READING_SIZE) != HAL_EEPROM_SUCCESS)
		{
			etHALEepromStatus =  HAL_EEPROM_ERROR_ADDRESS;
			// Handle read error
			break;
		}

		// Copy to USB buffer
		memcpy(&ui8UsbBuffer[ui16UsbIndex], cReadBuffer, HAL_EEPROM_READING_SIZE);
		ui16UsbIndex += HAL_EEPROM_READING_SIZE;
		ui32EepromAddr += HAL_EEPROM_READING_SIZE;

		// If buffer is full or we reached the end, send it
		if (ui16UsbIndex >= HAL_EEPROM_USB_CHUNK_SIZE || ui32EepromAddr >= ui32TotalBytes) {
			HAL_PC_write(ui8UsbBuffer, ui16UsbIndex);
			ui16UsbIndex = 0; // Reset buffer
		}
	}
	return etHALEepromStatus;
}

