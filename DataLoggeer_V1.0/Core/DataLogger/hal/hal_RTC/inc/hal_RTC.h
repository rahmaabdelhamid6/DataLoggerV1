/*
 * hal_RTC.h
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */

#ifndef DATALOGGER_HAL_HAL_RTC_INC_HAL_RTC_H_
#define DATALOGGER_HAL_HAL_RTC_INC_HAL_RTC_H_

/*******************************************************************************
*                               includes                             *
*******************************************************************************/
#include "stdint.h"
#include "stm32l4xx.h"

/*******************************************************************************
*                                Definitions                                  *
*******************************************************************************/

#define HAL_RTC_ONE_MINUTES   60
#define HAL_RTC_SECOND_IN_MINUTE   60
#define HAL_RTC_MINUTES_IN_HOUR    60
#define HAL_RTC_HOURS_IN_DAY       24
/*******************************************************************************
*                               Types Declaration                             *
*******************************************************************************/
typedef enum
{
	HAL_RTC_SUCCESS,
	HAL_RTC_FAILED
}HALRTCStatus_et;

/*******************************************************************************
*                                Extern Variables                                 *
*******************************************************************************/
extern RTC_TimeTypeDef g_sTimeStampTime;
extern RTC_DateTypeDef g_sTimeStampDate;
extern RTC_HandleTypeDef hrtc;

/*******************************************************************************
 *                             Function Declaration                             *
 *******************************************************************************/
HALRTCStatus_et HAL_RTC_init(void (*pfnCallback)(void));
HALRTCStatus_et HAL_RTC_Get_TimeDate(void);
HALRTCStatus_et HAL_RTC_ValidateTimeDate(void);
HALRTCStatus_et HAL_RTC_SetWakeUpTime(uint8_t ui8Mins);

#endif /* DATALOGGER_HAL_HAL_RTC_INC_HAL_RTC_H_ */
