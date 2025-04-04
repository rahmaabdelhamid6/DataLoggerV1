/*
 * appcb_GPIO.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */



/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "stdint.h"
#include "main.h"
#include "hal_Switch.h"
#include "mcal_GPIO_Isr.h"
#include "appcb_GPIO.h"

/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/
uint8_t g_ui8StopModeFlag = 0 ;
uint8_t g_ui8_WriteToEepromFlag = 0;
/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/**
 * @brief  Callback function executed on GPIO external interrupt to reconfigure the system clock and toggle stop mode/EEPROM write flags
 */
void APP_GPIO_EXTI_Callback(void)
{
	SystemClock_Config();
	if(g_ui8StopModeFlag==0)
	{
		g_ui8_WriteToEepromFlag = 1;
		g_ui8StopModeFlag=1;
	}
	else
	{
		g_ui8StopModeFlag=0;
		g_ui8_WriteToEepromFlag = 0;
	}
}
