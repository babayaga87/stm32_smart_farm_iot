//
// Created by Trần Minh Khải on 2/12/25.
//
/*
 * pump.c
 *
 *  Created on: Sep 29, 2025
 *      Author: Admin
 */

#include "pump.h"

// Biến lưu chân điều khiển
static GPIO_TypeDef *pump_GPIO;
static uint16_t pump_Pin1;
static uint16_t pump_Pin2;

void Pump_Init(GPIO_TypeDef *GPIOx, uint16_t Pin1, uint16_t Pin2)
{
  pump_GPIO = GPIOx;
  pump_Pin1 = Pin1;
  pump_Pin2 = Pin2;

  // Mặc định tắt bơm
  HAL_GPIO_WritePin(pump_GPIO, pump_Pin1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(pump_GPIO, pump_Pin2, GPIO_PIN_RESET);
}

void Pump_On(void)
{
  HAL_GPIO_WritePin(pump_GPIO, pump_Pin1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(pump_GPIO, pump_Pin2, GPIO_PIN_RESET);
}

void Pump_Off(void)
{
  HAL_GPIO_WritePin(pump_GPIO, pump_Pin1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(pump_GPIO, pump_Pin2, GPIO_PIN_RESET);
}





