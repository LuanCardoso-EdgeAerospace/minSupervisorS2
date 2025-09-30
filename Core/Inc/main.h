/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I_SENSE_Pin GPIO_PIN_1
#define I_SENSE_GPIO_Port GPIOC
#define CFG_RCW_SRC0_D_Pin GPIO_PIN_2
#define CFG_RCW_SRC0_D_GPIO_Port GPIOC
#define CFG_RCW_SRC1_D_Pin GPIO_PIN_3
#define CFG_RCW_SRC1_D_GPIO_Port GPIOC
#define MAIN_PWR_EN_Pin GPIO_PIN_1
#define MAIN_PWR_EN_GPIO_Port GPIOA
#define MCU_RS422_TX_Pin GPIO_PIN_2
#define MCU_RS422_TX_GPIO_Port GPIOA
#define MCU_RS422_RX_Pin GPIO_PIN_3
#define MCU_RS422_RX_GPIO_Port GPIOA
#define SNS_I2C_SDA_Pin GPIO_PIN_6
#define SNS_I2C_SDA_GPIO_Port GPIOA
#define SNS_I2C_SCL_Pin GPIO_PIN_7
#define SNS_I2C_SCL_GPIO_Port GPIOA
#define CFG_RCW_SRC2_D_Pin GPIO_PIN_4
#define CFG_RCW_SRC2_D_GPIO_Port GPIOC
#define CFG_RCW_SRC3_D_Pin GPIO_PIN_5
#define CFG_RCW_SRC3_D_GPIO_Port GPIOC
#define MAIN_12V0P_PG_Pin GPIO_PIN_0
#define MAIN_12V0P_PG_GPIO_Port GPIOB
#define CPU_3V3P_PG_Pin GPIO_PIN_1
#define CPU_3V3P_PG_GPIO_Port GPIOB
#define CPU_1V2P_PG_Pin GPIO_PIN_2
#define CPU_1V2P_PG_GPIO_Port GPIOB
#define _5V0P_PG_Pin GPIO_PIN_7
#define _5V0P_PG_GPIO_Port GPIOE
#define STATUS_LED1_18_Pin GPIO_PIN_8
#define STATUS_LED1_18_GPIO_Port GPIOE
#define STATUS_LED2_18_Pin GPIO_PIN_9
#define STATUS_LED2_18_GPIO_Port GPIOE
#define MCU_CPU_TX_Pin GPIO_PIN_10
#define MCU_CPU_TX_GPIO_Port GPIOB
#define MCU_CPU_RX_Pin GPIO_PIN_11
#define MCU_CPU_RX_GPIO_Port GPIOB
#define CPU_CORE_1V0P_PG_Pin GPIO_PIN_12
#define CPU_CORE_1V0P_PG_GPIO_Port GPIOB
#define TEMP_I2C_SCL_Pin GPIO_PIN_13
#define TEMP_I2C_SCL_GPIO_Port GPIOB
#define TEMP_I2C_SDA_Pin GPIO_PIN_14
#define TEMP_I2C_SDA_GPIO_Port GPIOB
#define CPU_DDR_PG_Pin GPIO_PIN_15
#define CPU_DDR_PG_GPIO_Port GPIOB
#define _12V0P_EN_Pin GPIO_PIN_8
#define _12V0P_EN_GPIO_Port GPIOD
#define _5V0P_EN_Pin GPIO_PIN_9
#define _5V0P_EN_GPIO_Port GPIOD
#define CPU_3V3P_EN_Pin GPIO_PIN_10
#define CPU_3V3P_EN_GPIO_Port GPIOD
#define PMIC_EN_Pin GPIO_PIN_11
#define PMIC_EN_GPIO_Port GPIOD
#define PMIC_SCL_Pin GPIO_PIN_12
#define PMIC_SCL_GPIO_Port GPIOD
#define PMIC_SDA_Pin GPIO_PIN_13
#define PMIC_SDA_GPIO_Port GPIOD
#define UEFI_SCL_Pin GPIO_PIN_9
#define UEFI_SCL_GPIO_Port GPIOA
#define UEFI_SDA_Pin GPIO_PIN_10
#define UEFI_SDA_GPIO_Port GPIOA
#define SYS_CLK_EN_MCU_Pin GPIO_PIN_4
#define SYS_CLK_EN_MCU_GPIO_Port GPIOD
#define MCU_PORESET_B_Pin GPIO_PIN_4
#define MCU_PORESET_B_GPIO_Port GPIOB
#define MCU_HRESET_B_Pin GPIO_PIN_5
#define MCU_HRESET_B_GPIO_Port GPIOB
#define MCU_RESET_REQ_B_Pin GPIO_PIN_6
#define MCU_RESET_REQ_B_GPIO_Port GPIOB
#define MCU_DRR4_RST_N_Pin GPIO_PIN_7
#define MCU_DRR4_RST_N_GPIO_Port GPIOB
#define MCU_eMMC_RST_N_Pin GPIO_PIN_8
#define MCU_eMMC_RST_N_GPIO_Port GPIOB
#define MCU_NOR_RST_N_Pin GPIO_PIN_9
#define MCU_NOR_RST_N_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
