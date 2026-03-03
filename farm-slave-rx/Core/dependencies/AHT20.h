/*
 * AHT20.h
 *
 *  Created on: Dec 1, 2025
 *      Author: Vy Khang
 */

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

/* ----------------* C++ detection *----------------- */

#ifdef __cplusplus
extern "C"{
#endif

/*  ----------------* Includes *-----------------  */

#include "main.h"
/*  ----------------* Function Prototypes *-----------------  */
void AHT20_Init(I2C_HandleTypeDef *hi2c);
int AHT20_Measure(float *Hump, float *Temp);

#endif /* INC_AHT20_H_ */
