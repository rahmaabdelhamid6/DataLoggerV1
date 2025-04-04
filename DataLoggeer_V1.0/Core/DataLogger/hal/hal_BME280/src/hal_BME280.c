/*
 * hal_BME280.c
 *
 *  Created on: Mar 23, 2025
 *      Author: CairoProgramAdmin
 */
#include "stm32l4xx_hal.h"
#include "hal_BME280.h"



/*Var for the Resiter the values from memory to vars*/
uint16_t dig_T1,  \
dig_P1, \
dig_H1, dig_H3;

int16_t  dig_T2, dig_T3, \
dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9, \
dig_H2,  dig_H4, dig_H5, dig_H6;

int32_t g_i32TempRaw = 0, g_i32PressureRaw = 0, g_i32HumidityRaw = 0;

/**
 * @brief  Read the Trimming parameters saved in the NVM ROM of the device
 * @param  void
 * @retval void
 */
HALBME280Status_et HAL_BME280_TrimmingRead(void)
{
	HALBME280Status_et etHALBME280Status = HAL_BME280_SUCCESS;
	/*Array to Store the Trimming Data From sensor*/
	uint8_t aui8TrimmingData[32];

	//Read NVM from 0x88 to 0xA1    -->> ATT why last address read is 0xA1 why noy 0xE7  and why 25 Byte not 32 byte
	HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, 0x88, HAL_BME280_SIZE_ADDRESS, aui8TrimmingData, 25, HAL_MAX_DELAY);

	// Read NVM from 0xE1 to 0xE7
	HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, 0xE1, HAL_BME280_SIZE_ADDRESS, (uint8_t *)aui8TrimmingData+25, 7, HAL_MAX_DELAY);


	// Arrange the data as per the datasheet (page no. 24)
	dig_T1 = (aui8TrimmingData[1]<<8)  | aui8TrimmingData[0];
	dig_T2 = (aui8TrimmingData[3]<<8)  | aui8TrimmingData[2];
	dig_T3 = (aui8TrimmingData[5]<<8)  | aui8TrimmingData[4];
	dig_P1 = (aui8TrimmingData[7]<<8)  | aui8TrimmingData[5];
	dig_P2 = (aui8TrimmingData[9]<<8)  | aui8TrimmingData[6];
	dig_P3 = (aui8TrimmingData[11]<<8) | aui8TrimmingData[10];
	dig_P4 = (aui8TrimmingData[13]<<8) | aui8TrimmingData[12];
	dig_P5 = (aui8TrimmingData[15]<<8) | aui8TrimmingData[14];
	dig_P6 = (aui8TrimmingData[17]<<8) | aui8TrimmingData[16];
	dig_P7 = (aui8TrimmingData[19]<<8) | aui8TrimmingData[18];
	dig_P8 = (aui8TrimmingData[21]<<8) | aui8TrimmingData[20];
	dig_P9 = (aui8TrimmingData[23]<<8) | aui8TrimmingData[22];
	dig_H1 =  aui8TrimmingData[24];
	dig_H2 = (aui8TrimmingData[26]<<8) | aui8TrimmingData[25];
	dig_H3 = (aui8TrimmingData[27]);
	dig_H4 = (aui8TrimmingData[28]<<4) | (aui8TrimmingData[29] & 0x0f);
	dig_H5 = (aui8TrimmingData[30]<<4) | (aui8TrimmingData[29]>>4);
	dig_H6 = (aui8TrimmingData[31]);

	return etHALBME280Status;
}


/* HAL_BME280_SensorConfig for the BME280

 * @param   ui8osrs_(Temp , Pressure , Humidity) is the oversampling to improve the accuracy
 *       if osrs is set to OSRS_OFF, the respective measurement will be skipped
 *       It can be set to OSRS_1, OSRS_2, OSRS_4, etc. Check the header file
 *
 * @param   ui8SensorMode can be used to set the mode for the device
 *       MODE_SLEEP will put the device in sleep
 *       MODE_FORCED device goes back to sleep after one measurement. You need to use the BME280_WakeUP() function before every measurement
 *       MODE_NORMAL device performs measurement in the normal mode. Check datasheet page no 16
 *
 * @param   ui8StandbyTime (t_sb) is time sensor waits before performing another measurement
 *       It is used along with the normal mode. Check datasheet page no 16 and page no 30
 *
 * @param   filter is the IIR filter coefficients
 *         IIR is used to avoid the short term fluctuations
 *         Check datasheet page no 18 and page no 30
 *
 * @retval HAL_StatusTypeDef status:
 */
HALBME280Status_et HAL_BME280_SensorConfig (uint8_t ui8osrs_Temp, uint8_t ui8osrs_Pressure, uint8_t ui8osrs_Humidity, uint8_t ui8SensorMode, uint8_t ui8StandbyTime, uint8_t ui8Filter)
{

	HALBME280Status_et etHALBME280Status = HAL_BME280_SUCCESS;

	// Read the Trimming parameters
	HAL_BME280_TrimmingRead();

	/*Data to Write To Sensor*/
	uint8_t ui8DataWrite = 0;

	/*Variable to checking the correct Data Ok is Write */
	uint8_t ui8DataCheck = 0;

	// Reset sequence the device
	ui8DataWrite = HAL_BME280_RESET_COMPLETE;

	/*Write in Reset Address to apply Reset*/
	if (HAL_I2C_Mem_Write(&hi2c1, HAL_BME280_ADDRESS, RESET_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataWrite, 1, 1000) != HAL_OK)
	{
		etHALBME280Status = HAL_BME280_ERROR;      //NACK
		return etHALBME280Status;
	}
	HAL_Delay (100);


	// write the humidity oversampling to 0xF2
	ui8DataWrite = ui8osrs_Humidity;
	if (HAL_I2C_Mem_Write(&hi2c1, HAL_BME280_ADDRESS, CTRL_HUM_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataWrite, 1, 1000) != HAL_OK)
	{
		etHALBME280Status = HAL_BME280_ERROR;
		return etHALBME280Status;
	}
	HAL_Delay (100);
	/*Read for Checking the Value in CTRL_HUM_REG */
	HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, CTRL_HUM_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataCheck, 1, 1000);
	if (ui8DataCheck != ui8DataWrite)
	{
		etHALBME280Status = HAL_BME280_ERROR;
		return etHALBME280Status;
	}


	// write the standby time and IIR ui8Filter coeff to 0xF5
	ui8DataWrite = (ui8StandbyTime <<5) |(ui8Filter << 2);
	if (HAL_I2C_Mem_Write(&hi2c1, HAL_BME280_ADDRESS, CONFIG_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataWrite, 1, 1000) != HAL_OK)
	{
		etHALBME280Status = HAL_BME280_ERROR;
		return etHALBME280Status;
	}
	HAL_Delay (100);
	HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, CONFIG_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataCheck, 1, 1000);
	if (ui8DataCheck != ui8DataWrite)
	{
		etHALBME280Status = HAL_BME280_ERROR;
		return etHALBME280Status;
	}


	// write the pressure and temp oversampling along with mode to 0xF4
	ui8DataWrite = (ui8osrs_Temp <<5) |(ui8osrs_Pressure << 2) | ui8SensorMode;
	if (HAL_I2C_Mem_Write(&hi2c1, HAL_BME280_ADDRESS, CTRL_MEAS_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataWrite, 1, 1000) != HAL_OK)
	{
		etHALBME280Status =  HAL_BME280_ERROR;
		return etHALBME280Status;
	}
	HAL_Delay (100);
	HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, CTRL_MEAS_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataCheck, 1, 1000);
	if (ui8DataCheck != ui8DataWrite)
	{
		etHALBME280Status = HAL_BME280_ERROR;
		return etHALBME280Status;
	}

	return etHALBME280Status;
}

/**
 * @brief  Reads raw pressure, temperature, and humidity data from the BME280 sensor.
 *
 * This function performs the following steps:
 * 1. Checks the chip ID to verify communication with the sensor.
 * 2. Reads 8 bytes of data from registers 0xF7 to 0xFE.
 * 3. Extracts raw pressure (20-bit), temperature (20-bit), and humidity (16-bit) values.
 * 4. Returns HAL_OK if successful, otherwise HAL_ERROR.
 *
 * @return HALBME280Status_et - HAL_BME280_SUCCESS if data is read successfully, HAL_BME280_ERROR if chip ID check fails.
 */
HALBME280Status_et HAL_BME280_ReadRawData(void)
{
	HALBME280Status_et etHALBME280Status = HAL_BME280_SUCCESS;

	uint8_t ui8RawData[8];

	/*For ID Check Chip*/
	uint8_t ui8ChipID;

	// Check the chip ID before reading sensor data
	HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, ID_REG, HAL_BME280_SIZE_ADDRESS, &ui8ChipID, 1, 1000);

	// Verify if the sensor has been properly initialized (device reset number check)
	if (ui8ChipID == HAL_BM280_DEVICE_FINISHED_RESET_NUMBER)
	{
		// Read 8 bytes starting from the PRESS_MSB_REG (0xF7 to 0xFE)
		// Data includes pressure (20-bit), temperature (20-bit), and humidity (16-bit)
		HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, PRESS_MSB_REG, HAL_BME280_SIZE_ADDRESS, ui8RawData, 8, HAL_MAX_DELAY);

		/* Extract raw pressure, temperature, and humidity values */
		// Pressure: 20-bit value extracted from registers 0xF7, 0xF8, and 0xF9
		g_i32PressureRaw = (ui8RawData[0] << 12) | (ui8RawData[1] << 4) | (ui8RawData[2] >> 4);

		// Temperature: 20-bit value extracted from registers 0xFA, 0xFB, and 0xFC
		g_i32TempRaw = (ui8RawData[3] << 12) | (ui8RawData[4] << 4) | (ui8RawData[5] >> 4);

		// Humidity: 16-bit value extracted from registers 0xFD and 0xFE
		g_i32HumidityRaw = (ui8RawData[6] << 8) | (ui8RawData[7]);
	}
	else
	{
		// Return error if the chip ID check fails
		etHALBME280Status = HAL_BME280_ERROR;
	}

	return etHALBME280Status;
}

/* To be used when doing the force measurement
 * the Device need to be put in forced mode every time the measurement is needed
 */
HALBME280Status_et HAL_BME280_WakeUPSensor(void)
{
	HALBME280Status_et etHALBME280Status = HAL_BME280_SUCCESS;
	uint8_t ui8DataWrite = 0;

	// first read the register
	HAL_I2C_Mem_Read(&hi2c1, HAL_BME280_ADDRESS, CTRL_MEAS_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataWrite, 1, 1000);

	// modify the data with the forced mode
	ui8DataWrite = ui8DataWrite | MODE_FORCED;

	// write the new data to the register
	HAL_I2C_Mem_Write(&hi2c1, HAL_BME280_ADDRESS, CTRL_MEAS_REG, HAL_BME280_SIZE_ADDRESS, &ui8DataWrite, 1, 1000);

	HAL_Delay (100);

	return etHALBME280Status;
}

/**
 * @brief  Reads temperature, pressure, and humidity data from the BME280 sensor.
 *
 * This function retrieves raw sensor data, compensates the values using the
 * appropriate formulas from the BME280 datasheet, and stores the results
 * in the provided pointers.
 *
 * @param[out] fpTemperatureRead Pointer to store the measured temperature (in °C).
 * @param[out] fpPressureRead    Pointer to store the measured pressure (in Pa).
 * @param[out] fpHumidityRead    Pointer to store the measured humidity (in %).
 *
 * @note The function will return 0 for any parameter if the measurement was disabled
 *       or if the sensor is not properly connected.
 */
HALBME280Status_et HAL_BME280_Measure (float *fpTemperatureRead  ,float *fpHumidityRead )
{
	HALBME280Status_et etHALBME280Status = HAL_BME280_SUCCESS;
	if (HAL_BME280_ReadRawData() == HAL_BME280_SUCCESS)
	{
		if (g_i32TempRaw == 0x800000)
		{
			*fpTemperatureRead = 0; // value in case temp measurement was disabled
		}
		else
		{
			*fpTemperatureRead = (HAL_BME280_compensate_T_int32 (g_i32TempRaw))/100.0;  // as per datasheet, the temp is x100
		}

		if (g_i32HumidityRaw == 0x800000)
		{
			*fpHumidityRead = 0; // value in case temp measurement was disabled
		}
		else
		{
			*fpHumidityRead = (HAL_BME280_compensate_H_int32 (g_i32HumidityRaw))/1024.0;  // as per datasheet, the temp is x1024
		}
	}


	// if the device is detached
	else
	{
		*fpTemperatureRead = 0;
		*fpHumidityRead    = 0;
		 etHALBME280Status = HAL_BME280_ERROR;
		 return etHALBME280Status;
	}
	return etHALBME280Status;
}




/************* COMPENSATION CALCULATION AS PER DATASHEET (page 25) **************************/

/************* Using this Functions in HAL noly Not Used in any Layer ***********************/

/* Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
   t_fine carries fine temperature as global value
 */
int32_t t_fine;
int32_t HAL_BME280_compensate_T_int32(int32_t adc_T)
{
	int32_t var1, var2, T;
	var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1)))>> 12) *((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}


#if SUPPORT_64BIT
/* Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
   Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
 */
uint32_t HAL_BME280_compensate_P_int64(int32_t adc_P)
{
	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
	return (uint32_t)p;
}

#elif SUPPORT_32BIT
// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
uint32_t BME280_compensate_P_int32(int32_t adc_P)
{
	int32_t var1, var2;
	uint32_t p;
	var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
	var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
	var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
	var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) *var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
	if (p < 0x80000000)
	{
		p = (p << 1) / ((uint32_t)var1);
	}
	else
	{
		p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)dig_P8))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
	return p;
}
#endif

/* Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
   Output value of “47445” represents 47445/1024 = 46.333 %RH
 */
uint32_t HAL_BME280_compensate_H_int32(int32_t adc_H)
{
	int32_t v_x1_u32r;
	v_x1_u32r = (t_fine - ((int32_t)76800));
	v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) *\
			v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r *\
					((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) +\
							((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) +\
					8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *\
			((int32_t)dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (uint32_t)(v_x1_u32r>>12);
}
/*********************************************************************************************************/

