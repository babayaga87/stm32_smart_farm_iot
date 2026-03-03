/*
 * AHT20.c
 *
 *  Created on: Dec 1, 2025
 *      Author: Vy Khang
 */

/*  ----------------* Includes *-----------------  */
#include "AHT20.h"
#define AHT20_ADDRESS 0x38<<1

/*  ----------------* Variables *-----------------  */
I2C_HandleTypeDef *AHT20_I2C;

/*  ----------------* Functions *-----------------  */

void AHT20_Init(I2C_HandleTypeDef *hi2c){
	AHT20_I2C=hi2c;
	HAL_Delay(40);
	uint8_t status;
	HAL_I2C_Mem_Read(AHT20_I2C, AHT20_ADDRESS, 0x71 , 1, &status, 1, 1000);
	if((status>>3 &0x01)==0){
		uint8_t init_commands[3] = {0xBE,0x08,0x00};
		HAL_I2C_Master_Transmit(AHT20_I2C, AHT20_ADDRESS, init_commands , 3, 1000);
		HAL_Delay(10);
	}
}

int AHT20_Measure(float *Hump, float *Temp){
	uint8_t measure_commands[3] = {0xAC,0x33,0x00};
	HAL_I2C_Master_Transmit(AHT20_I2C, AHT20_ADDRESS, measure_commands , 3, 1000);
	HAL_Delay(80);

	uint8_t status;
	do{
		HAL_I2C_Mem_Read(AHT20_I2C, AHT20_ADDRESS, 0x71 , 1, &status, 1, 1000);
		HAL_Delay(100);
	}while((status>>7 &0x01)==1);

	uint8_t RxData[7];
	HAL_I2C_Master_Receive(AHT20_I2C, AHT20_ADDRESS, RxData , 7, 1000);

	uint32_t HumpData = ((RxData[1]<<12)|(RxData[2]<<4)|RxData[3]>>4);
	*Hump = (float)(HumpData/(1024.0*1024.0))*100;

	uint32_t TempData = (((RxData[3]&0x0F)<<16)|(RxData[4]<<8)|RxData[5]);
	*Temp = (float)(((TempData/1024.0)/1024.0) * 200.0) -50.0;
	return 1;
}
