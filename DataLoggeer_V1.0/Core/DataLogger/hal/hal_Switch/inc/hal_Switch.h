/*
 * hal_Switch.h
 *
 *  Created on: Mar 26, 2025
 *      Author: CairoProgramAdmin
 */

#ifndef DATALOGGER_HAL_HAL_SWITCH_INC_HAL_SWITCH_H_
#define DATALOGGER_HAL_HAL_SWITCH_INC_HAL_SWITCH_H_

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
	HAL_GPIO_SUCCESS,
	HAL_GPIO_FAILED
}HALGPIOStatus_et;


typedef enum
{
	HAL_SWITCH_PRESSED,
	HAL_SWITCH_RELEASED
}HALSwitchStatus_et;
/*******************************************************************************
*                                Extern Variables                                 *
*******************************************************************************/
extern HALSwitchStatus_et g_etHalSwitchStatus;

/*******************************************************************************
 *                             Function Declaration                             *
 *******************************************************************************/
HALGPIOStatus_et HAL_Switch_Init(void (*pfnCallback)(void));
HALGPIOStatus_et HAL_Switch_SetStatus(HALSwitchStatus_et etHalSwitchStatus);
HALGPIOStatus_et HAL_Switch_GetStatus(HALSwitchStatus_et etHalSwitchStatus);
#endif /* DATALOGGER_HAL_HAL_SWITCH_INC_HAL_SWITCH_H_ */
