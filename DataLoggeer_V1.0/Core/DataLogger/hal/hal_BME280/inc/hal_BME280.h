/*
 * hal_BME280.h
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */

#ifndef DATALOGGER_HAL_HAL_BME280_INC_HAL_BME280_H_
#define DATALOGGER_HAL_HAL_BME280_INC_HAL_BME280_H_

/* Configuration for the BME280

 * @osrs is the oversampling to improve the accuracy
 *       if osrs is set to OSRS_OFF, the respective measurement will be skipped
 *       It can be set to OSRS_1, OSRS_2, OSRS_4, etc. Check the header file
 *
 * @mode can be used to set the mode for the device
 *       MODE_SLEEP will put the device in sleep
 *       MODE_FORCED device goes back to sleep after one measurement. You need to use the BME280_WakeUP() function before every measurement
 *       MODE_NORMAL device performs measurement in the normal mode. Check datasheet page no 16
 *
 * @t_sb is the standby time. The time sensor waits before performing another measurement
 *       It is used along with the normal mode. Check datasheet page no 16 and page no 30
 *
 * @filter is the IIR filter coefficients
 *         IIR is used to avoid the short term fluctuations
 *         Check datasheet page no 18 and page no 30
 */

/*******************************************************************************
*                                Definitions                                  *
*******************************************************************************/
// Oversampling definitions
#define OSRS_OFF    	0x00
#define OSRS_1      	0x01
#define OSRS_2      	0x02
#define OSRS_4      	0x03
#define OSRS_8      	0x04
#define OSRS_16     	0x05

// MODE Definitions
#define MODE_SLEEP      0x00
#define MODE_FORCED     0x01
#define MODE_NORMAL     0x03

// Standby Time
#define T_SB_0p5    	0x00
#define T_SB_62p5   	0x01
#define T_SB_125    	0x02
#define T_SB_250    	0x03
#define T_SB_500    	0x04
#define T_SB_1000   	0x05
#define T_SB_10     	0x06
#define T_SB_20     	0x07

// IIR Filter Coefficients
#define IIR_OFF     	0x00
#define IIR_2       	0x01
#define IIR_4       	0x02
#define IIR_8       	0x03
#define IIR_16      	0x04


// REGISTERS DEFINITIONS
#define ID_REG      	0xD0
#define RESET_REG  		0xE0
#define CTRL_HUM_REG    0xF2
#define STATUS_REG      0xF3
#define CTRL_MEAS_REG   0xF4
#define CONFIG_REG      0xF5
#define PRESS_MSB_REG   0xF7



/*Using for magic numbers in BME280.c  */
#define HAL_BME280_START_ADDRESS_MEMORY     0x88
#define HAL_BME280_NUMBER_BYTE_READ         32
#define HAL_BME280_SIZE_ADDRESS             1
#define HAL_BME280_ADDRESS                  0xEC  // SDIO is grounded, the 7 bit address is 0x76 and 8 bit address = 0x76<<1 = 0xEC
#define HAL_BM280_DEVICE_FINISHED_RESET_NUMBER      0x60
#define HAL_BME280_RESET_COMPLETE                   0xB6
#define SUPPORT_64BIT                       1



/*******************************************************************************
*                               Types Declaration                             *
*******************************************************************************/
typedef enum
{
	HAL_BME280_SUCCESS = 0,
	HAL_BME280_ERROR = -1,

}HALBME280Status_et;

/*******************************************************************************
*                                Extern Variables                                 *
*******************************************************************************/
extern I2C_HandleTypeDef hi2c1;

/*******************************************************************************
 *                             Function Declaration                             *
 *******************************************************************************/
HALBME280Status_et HAL_BME280_TrimmingRead(void);
HALBME280Status_et HAL_BME280_SensorConfig (uint8_t ui8osrs_Temp, uint8_t ui8osrs_Pressure, uint8_t ui8osrs_Humidity, uint8_t ui8SensorMode, uint8_t ui8StandbyTime, uint8_t ui8Filter);
HALBME280Status_et HAL_BME280_ReadRawData(void);
HALBME280Status_et HAL_BME280_WakeUPSensor(void);
HALBME280Status_et HAL_BME280_Measure (float *fpTemperatureRead  ,float *fpHumidityRead );


int32_t HAL_BME280_compensate_T_int32(int32_t adc_T);
uint32_t HAL_BME280_compensate_P_int64(int32_t adc_P);
uint32_t HAL_BME280_compensate_H_int32(int32_t adc_H);



#endif /* DATALOGGER_HAL_HAL_BME280_INC_HAL_BME280_H_ */
