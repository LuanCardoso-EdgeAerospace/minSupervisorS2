/*
 * util.h
 *
 *  Created on: Dec 15, 2025
 *      Author: luan
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "stm32u0xx_hal.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


#define ADCTimeout 100 /*ms*/
/*
 * Abstraction to log information on the UART interface.
 */
void log_printf(const char *fmt, ...);
void waitBtnPress();
int waitSignalTimeout(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState State, uint32_t TimeoutMs);
void HCF(void);

uint16_t ADC_IN0();
uint16_t ADC_IN1();
void reportADC();


#endif /* INC_UTIL_H_ */
