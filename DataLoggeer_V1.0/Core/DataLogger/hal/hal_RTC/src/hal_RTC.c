/*
 * hal_RTC.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */


/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "hal_RTC.h"
#include "mcal_RTC_Isr.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"

/*******************************************************************************
 *                               Global Variables                             *
 *******************************************************************************/
RTC_TimeTypeDef g_sTimeStampTime;
RTC_DateTypeDef g_sTimeStampDate;
const uint8_t g_aui8DaysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};



/*******************************************************************************
 *                              Function Definition                            *
 *******************************************************************************/
/**
 * @brief  Initialize the RTC with a callback function for wake-up events
 * @param  pfnCallback Pointer to the callback function to be executed on wake-up
 * @retval HAL RTC status
 */
HALRTCStatus_et HAL_RTC_init(void (*pfnCallback)(void))
{
	HALRTCStatus_et etHALRTCStatus = HAL_RTC_SUCCESS;

	MCAL_ISR_RTC_WakeUpCallback(pfnCallback);

	return etHALRTCStatus;
}
/**
 * @brief  Validate and correct the current time and date stored in RTC
 * @retval HAL RTC status
 */
HALRTCStatus_et HAL_RTC_ValidateTimeDate(void)
{
	HALRTCStatus_et etHALRTCStatus = HAL_RTC_SUCCESS;
//	bool isLeapYear = false;
//	uint8_t ui8MaxDays = 0;
//	uint16_t ui16Year = g_sTimeStampDate.Year + 2000;

	HAL_RTC_GetTime(&hrtc, &g_sTimeStampTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &g_sTimeStampDate, RTC_FORMAT_BCD);

	if (g_sTimeStampTime.Seconds >= 60){
		g_sTimeStampTime.Seconds %= 60;
		g_sTimeStampTime.Minutes += g_sTimeStampTime.Seconds / HAL_RTC_SECOND_IN_MINUTE;
	}
	if (g_sTimeStampTime.Minutes >= 60){
		g_sTimeStampTime.Minutes %= 60;
		g_sTimeStampTime.Hours += g_sTimeStampTime.Minutes / HAL_RTC_MINUTES_IN_HOUR;
	}
	if (g_sTimeStampTime.Hours >= 24){
		g_sTimeStampTime.Hours %= 24;
		g_sTimeStampDate.Date = g_sTimeStampTime.Hours / HAL_RTC_HOURS_IN_DAY;
	}
//	if ((ui16Year % 4 == 0 && ui16Year % 100 != 0) || (ui16Year % 400 == 0)) {
//		isLeapYear = true;
//	}
//	// Months (1-12)
//	if (g_sTimeStampDate.Month == 0 || g_sTimeStampDate.Month > 12) {
//		g_sTimeStampDate.Month = 1;  // Default to January if invalid
//	}
//	// Days (1-31, depending on month)
//	ui8MaxDays = (g_sTimeStampDate.Month == 2 && isLeapYear) ? 29 : g_aui8DaysInMonth[g_sTimeStampDate.Month];
//	if (g_sTimeStampDate.Date == 0) {
//		g_sTimeStampDate.Date = 1;  // Minimum day
//	}
//	while (g_sTimeStampDate.Date > ui8MaxDays) {
//		g_sTimeStampDate.Date -= ui8MaxDays;
//		g_sTimeStampDate.Month++;
//
//		if (g_sTimeStampDate.Month > 12) {
//			g_sTimeStampDate.Month = 1;
//			g_sTimeStampDate.Year++;
//			ui16Year++;
//			isLeapYear = ((ui16Year % 4 == 0 && ui16Year % 100 != 0) || (ui16Year % 400 == 0));
//		}
//		ui8MaxDays = (g_sTimeStampDate.Month == 2 && isLeapYear) ? 29 : g_aui8DaysInMonth[g_sTimeStampDate.Month];
//	}
	return etHALRTCStatus;
}
/**
 * @brief  Retrieve the current time and date from the RTC
 * @retval HAL RTC status
 */
HALRTCStatus_et HAL_RTC_Get_TimeDate(void)
{
	HALRTCStatus_et etHALRTCStatus = HAL_RTC_SUCCESS;

	HAL_RTC_GetTime(&hrtc, &g_sTimeStampTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &g_sTimeStampDate, RTC_FORMAT_BCD);

	return etHALRTCStatus;
}
/**
 * @brief  Set the wake-up timer duration in minutes for the RTC
 * called one time before while (1) main to set the duration of the waiting time
 * @param  ui8Mins Duration in minutes for the wake-up timer
 * @retval HAL RTC status
 */
HALRTCStatus_et HAL_RTC_SetWakeUpTime(uint8_t ui8Mins)
{
	HALRTCStatus_et etHALRTCStatus = HAL_RTC_SUCCESS;
	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, (ui8Mins*HAL_RTC_ONE_MINUTES), RTC_WAKEUPCLOCK_CK_SPRE_16BITS, 0) != HAL_OK)
	{
		etHALRTCStatus = HAL_RTC_FAILED;
		Error_Handler();
	}
	return etHALRTCStatus;
}
