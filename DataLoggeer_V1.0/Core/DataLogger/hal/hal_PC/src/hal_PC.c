/*
 * hal_PC.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */


/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/


#include "string.h"
#include <stdio.h>
#include "usbd_cdc_if.h"
#include "hal_PC.h"




/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/

uint8_t g_ui8UsbRecieveLen = 0;// used in usbd_cdc_if.c file in the CDC_Receive_FS(uint8_t* Buf, uint32_t *Len); function
uint8_t g_aui8UsbRxBuffer[64] = {0};

/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/*
 *
 * \brief	This function is used to read PC Message
 * \param	pui8Buffer	pointer to hold read data
 * \param	ui16DataSize	size of data to be read
 * \return	uint32_t status
 * */
HALPCStatus_et HAL_PC_read (uint8_t * pui8UsbRxBuffer)
{
	HALPCStatus_et etHALPCStatus = HAL_PC_SUCCESS;

	// Copy data to the buffer
	memcpy((void *) pui8UsbRxBuffer , (void *) &g_aui8UsbRxBuffer[0] , g_ui8UsbRecieveLen);

	return etHALPCStatus;
}

/*
 *
 * \brief	This function is used to write PC Message
 * \param	pui8Buffer	pointer to hold read data
 * \param	ui16DataSize	size of data to be read
 * \return	uint32_t status
 * Note : ui16DataSize must not exceed 32 bytes
 * */
HALPCStatus_et HAL_PC_write (uint8_t * pui8UsbTxBuffer , uint16_t ui16UsbTxBufferSize)
{
	HALPCStatus_et etHALPCStatus = HAL_PC_SUCCESS;

	// Send PC Message
	if (CDC_Transmit_FS(pui8UsbTxBuffer, ui16UsbTxBufferSize) != USBD_OK)
	{
		etHALPCStatus = HAL_PC_ERROR;
		return etHALPCStatus;
	}

	return etHALPCStatus;
}




