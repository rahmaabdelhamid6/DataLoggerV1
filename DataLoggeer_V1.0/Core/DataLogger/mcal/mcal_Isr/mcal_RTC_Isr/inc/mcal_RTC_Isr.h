/*
 * mcal_RTC_Isr.h
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */

#ifndef DATALOGGER_MCAL_MCAL_ISR_MCAL_RTC_ISR_INC_MCAL_RTC_ISR_H_
#define DATALOGGER_MCAL_MCAL_ISR_MCAL_RTC_ISR_INC_MCAL_RTC_ISR_H_



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


/*******************************************************************************
*                                Extern Variables                                 *
*******************************************************************************/
extern void (*g_pfnRTCWakeUpTimerEventCallback) (void);

/*******************************************************************************
 *                             Function Declaration                             *
 *******************************************************************************/
void MCAL_ISR_RTC_WakeUpCallback (void (*g_pfnCallback)(void));

#endif /* DATALOGGER_MCAL_MCAL_ISR_MCAL_RTC_ISR_INC_MCAL_RTC_ISR_H_ */
