/*
 * pump.h
 *
 *  Created on: Sep 29, 2025
 *      Author: Admin
 */

#ifndef INC_PUMP_H_
#define INC_PUMP_H_

#include "stm32f1xx_hal.h"

// Khai báo hàm khởi tạo và điều khiển
void Pump_Init(GPIO_TypeDef *GPIOx, uint16_t Pin1, uint16_t Pin2);
void Pump_On(void);
void Pump_Off(void);
#endif /* INC_PUMP_H_ */
