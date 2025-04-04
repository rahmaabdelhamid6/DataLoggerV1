/*
 * appcb_RTC.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */


/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "stdint.h"
#include "main.h"
#include "hal_RTC.h"
#include "mcal_RTC_Isr.h"

/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/



/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/**
 * @brief  Callback function executed on RTC wake-up timer event to reconfigure the system clock
 */
void APP_RTC_WakeUpCallback(void)
{
	SystemClock_Config ();
}
