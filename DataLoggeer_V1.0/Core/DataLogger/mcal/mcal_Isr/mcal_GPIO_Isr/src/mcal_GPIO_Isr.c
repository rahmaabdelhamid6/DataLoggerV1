/*
 * mcal_GPIO_Isr.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */



/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "stdint.h"
#include "hal_RTC.h"
#include "mcal_GPIO_Isr.h"

/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/

void (*g_pfnGPIOMcalEXTICallback) (void) = NULL;


/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/**
 * @brief  Register a callback function for GPIO external interrupt handling in the MCAL layer
 * @param  g_pfnCallback Pointer to the callback function to be executed on interrupt
 */
void MCAL_ISR_GPIO_EXTICallback (void (*g_pfnCallback)(void))
{
	g_pfnGPIOMcalEXTICallback = g_pfnCallback;
}
/**
 * @brief  Execute the registered callback function when a GPIO external interrupt occurs
 * @param  GPIO_Pin The GPIO pin number that triggered the interrupt
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (g_pfnGPIOMcalEXTICallback != NULL)
	{
		g_pfnGPIOMcalEXTICallback();
	}
}
