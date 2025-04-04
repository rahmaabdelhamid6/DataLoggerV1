/*
 * hal_EEPROM.h
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */

#ifndef DATALOGGER_HAL_HAL_EEPROM_INC_HAL_EEPROM_H_
#define DATALOGGER_HAL_HAL_EEPROM_INC_HAL_EEPROM_H_
/*******************************************************************************
*                               includes                             *
*******************************************************************************/
#include "stdint.h"
#include "stm32l4xx.h"

/*******************************************************************************
*                                Definitions                                  *
*******************************************************************************/
#define HAL_EEPROM_ADDR                      0xA0
#define HAL_EEPROM_SIZE                      65536
#define HAL_EEPROM_PAGE_NUMS                 512
#define HAL_EEPROM_PAGE_SIZE                 128
#define HAL_EEPROM_DATA_START_ADDR           2     // Data starts after the last address location
#define HAL_EEPROM_LAST_ADDRESS_LOC          0             // First 2 bytes in EEPROM for last address
#define HAL_EEPROM_USB_CHUNK_SIZE            1024    // 1KB per USB send
#define HAL_EEPROM_READING_SIZE              32      // 32 bytes per reading
/*******************************************************************************
*                               Types Declaration                             *
*******************************************************************************/
typedef enum
{
	HAL_EEPROM_SUCCESS,
	HAL_EEPROM_ERROR_BUFFER_SIZE,
	HAL_EEPROM_ERROR_PAGE_ADDRESS,
	HAL_EEPROM_ERROR_ADDRESS,
	HAL_EEPROM_ERROR_INTERFACE,
	HAL_EEPROM_FULL_SIZE
}HALEepromStatus_et;

/*******************************************************************************
*                                Extern Variables                                 *
*******************************************************************************/
extern uint8_t g_aui8EEPROMTxBuffer[HAL_EEPROM_PAGE_SIZE];
extern uint8_t g_aui8EEPROMRxBuffer[HAL_EEPROM_PAGE_SIZE];

extern I2C_HandleTypeDef hi2c2;

/*******************************************************************************
 *                             Function Declaration                             *
 *******************************************************************************/
HALEepromStatus_et HAL_EEPROM_WriteByte(uint16_t ui16ByteNumber,uint8_t ui8WriteData);
HALEepromStatus_et HAL_EEPROM_WritePage(uint16_t ui16PageAddr,uint8_t aui8TxBuffer[], uint8_t aui8TxBufSize);
HALEepromStatus_et HAL_EEPROM_ReadByte( uint16_t ui16ByteNumber, uint8_t *ui8ReadData);
HALEepromStatus_et HAL_EEPROM_ReadPage( uint16_t ui16PageAddr, uint8_t aui8RxBuffer[], uint8_t ui8RxBufSize);
HALEepromStatus_et HAL_EEPROM_Write(uint16_t ui16EepromAd, uint8_t* ui8WriteBuff, uint16_t ui16WriteBuffSize);
HALEepromStatus_et HAL_EEPROM_Read(uint16_t ui16EepromAdd, uint8_t* ui8ReadBuff, uint16_t ui16ReadBuffSize);
HALEepromStatus_et HAL_EEPROM_SaveLastAddress(void);
HALEepromStatus_et HAL_EEPROM_LoadLastAddress(void);
HAL_StatusTypeDef HAL_EEPROM_Write_DateTime(float fTemperature, float fHumidity);
HALEepromStatus_et HAL_EEPROM_Stream_Data(void);
#endif /* DATALOGGER_HAL_HAL_EEPROM_INC_HAL_EEPROM_H_ */
