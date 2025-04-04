/*
 * mcal_RTC_Isr.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */



/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "stdint.h"
#include "mcal_RTC_Isr.h"

/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/
void (*g_pfnRTCMcalWakeUpTimerEventCallback) (void) = NULL;


/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/**
 * @brief  Register a callback function for RTC wake-up timer events in the MCAL layer
 * @param  g_pfnCallback Pointer to the callback function to be executed on wake-up timer event
 */
void MCAL_ISR_RTC_WakeUpCallback (void (*g_pfnCallback)(void))
{
	g_pfnRTCMcalWakeUpTimerEventCallback = g_pfnCallback;
}
/**
 * @brief  Execute the registered callback function when an RTC wake-up timer event occurs
 * @param  hrtc Pointer to the RTC handle structure
 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	if (g_pfnRTCMcalWakeUpTimerEventCallback != NULL)
	{
		g_pfnRTCMcalWakeUpTimerEventCallback();
	}
}
