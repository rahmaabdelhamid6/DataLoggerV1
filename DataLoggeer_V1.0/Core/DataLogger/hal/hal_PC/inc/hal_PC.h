/*
 * hal_PC.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */


/*
 * hal_PC.h
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */

#ifndef DATALOGGER_HAL_HAL_PC_INC_HAL_PC_H_
#define DATALOGGER_HAL_HAL_PC_INC_HAL_PC_H_

/*******************************************************************************
*                               includes                             *
*******************************************************************************/
#include "stdint.h"
#include "stm32l4xx.h"

/*******************************************************************************
*                                Definitions                                  *
*******************************************************************************/



/*******************************************************************************
*                               Types Declaration                             *
*******************************************************************************/

typedef enum
{
	HAL_PC_SUCCESS,
	HAL_PC_ERROR
}HALPCStatus_et;
/*******************************************************************************
*                                Extern Variables                                 *
*******************************************************************************/
extern uint8_t g_ui8UsbRecieveLen;
extern uint8_t g_aui8UsbRxBuffer[64] ;

/*******************************************************************************
 *                             Function Declaration                             *
 *******************************************************************************/
HALPCStatus_et HAL_PC_read (uint8_t * pui8UsbRxBuffer);
HALPCStatus_et HAL_PC_write (uint8_t * pui8UsbTxBuffer , uint16_t ui16UsbTxBufferSize);


#endif /* DATALOGGER_HAL_HAL_PC_INC_HAL_PC_H_ */
