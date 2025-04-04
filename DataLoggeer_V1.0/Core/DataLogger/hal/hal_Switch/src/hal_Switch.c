/*
 * hal_Switch.c
 *
 *  Created on: Mar 26, 2025
 *      Author: CairoProgramAdmin
 */




/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "hal_Switch.h"
#include "mcal_GPIO_Isr.h"


/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/

HALSwitchStatus_et g_etHalSwitchStatus = HAL_SWITCH_RELEASED ;

/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/**
 * @brief  Initialize the switch GPIO with a callback function for external interrupts
 * @param  pfnCallback Pointer to the callback function to be executed on interrupt
 * @retval HAL GPIO status
 */
HALGPIOStatus_et HAL_Switch_Init(void (*pfnCallback)(void))
{
	HALGPIOStatus_et etHALGPIOStatus = HAL_GPIO_SUCCESS;

	g_etHalSwitchStatus = HAL_SWITCH_RELEASED ;
	MCAL_ISR_GPIO_EXTICallback (pfnCallback);

	return etHALGPIOStatus;
}
/**
 * @brief  Set the status of the switch by controlling the GPIO pin
 * @param  etHalSwitchStatus The desired switch status (HAL_SWITCH_RELEASED or HAL_SWITCH_PRESSED)
 * @retval HAL GPIO status
 */
HALGPIOStatus_et HAL_Switch_SetStatus(HALSwitchStatus_et etHalSwitchStatus)
{
	HALGPIOStatus_et etHALGPIOStatus = HAL_GPIO_SUCCESS;

	if (etHalSwitchStatus == HAL_SWITCH_RELEASED )
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 , GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 , GPIO_PIN_RESET);
	}

	return etHALGPIOStatus;
}
/**
 * @brief  Get the current status of the switch by reading the GPIO pin
 * @param  etHalSwitchStatus The current switch status to be updated (HAL_SWITCH_RELEASED or HAL_SWITCH_PRESSED)
 * @retval HAL GPIO status
 */
HALGPIOStatus_et HAL_Switch_GetStatus(HALSwitchStatus_et etHalSwitchStatus)
{
	HALGPIOStatus_et etHALGPIOStatus = HAL_GPIO_SUCCESS;

	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_SET)
	{
		etHalSwitchStatus = HAL_SWITCH_RELEASED ;
	}
	else
	{
		etHalSwitchStatus = HAL_SWITCH_PRESSED;
	}

	return etHALGPIOStatus;
}
