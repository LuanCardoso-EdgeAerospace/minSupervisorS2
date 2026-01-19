/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "util.h"
#include "INA230.h"
#include "TMP468.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define VERSION "0.1 alpha"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;
I2C_HandleTypeDef hi2c4;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef hlpuart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_FLASH_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_I2C4_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_LPUART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

enum SUPERVISOR_STATE { S_ASSERT, S_RELEASE };
#define STEP_DELAY 200
#define UART_LOG(...) /*Should use LPUART1*/

void ina230write(uint16_t addr, uint8_t reg, uint16_t pData){
  addr <<=1;
  uint8_t data[2]={0};
  data[0]=pData >> 8;
  data[1]=pData & 0xff;
  
  HAL_I2C_Mem_Write(&hi2c3, addr, reg, 1, data, 2, 1000);
}

uint16_t ina230read(uint16_t addr, uint8_t reg){
  addr <<=1;
  uint8_t data[2]={0};
  HAL_I2C_Mem_Read(&hi2c3, addr, reg, 1, data, 2, 1000);
  uint16_t res = (data[0]<<8)|(data[1]);
  return res;
}

void testINA230(uint8_t addr, uint16_t shuntResistance){
	INA230_t target;
	target = INA230_init(addr, 1, shuntResistance, INA230_SHUNT_ADC_CT_204, INA230_VBUS_ADC_CT_332, INA230_AVG_16, ina230write, ina230read);
	INA230_start(target, INA230_MODE_CONTINOUS_ALL);
	HAL_Delay(100);

	uint16_t id = INA230_getID(target);
	 int16_t bus = INA230_getVoltageBus(target);
	 int16_t shunt = INA230_getVoltageShunt(target);
	 int16_t current = INA230_getCurrent(target);
	uint16_t callibration = INA230_getCalibration(target);
	uint16_t power = INA230_getPower(target);
	log_printf("INA230 id:0x%04x addr 0x%x bus:%5dmV shunt:%4duV A:%3dmA P:%4umW calibr:%u\r\n", id, addr, bus, shunt, current, power, callibration);
	INA230_stop(target);
}

void TMP468write(uint16_t addr, uint8_t reg, uint16_t pData){
  addr <<=1;
  uint8_t data[2]={0};
  data[0]=pData >> 8;
  data[1]=pData & 0xff;
  
  HAL_I2C_Mem_Write(&hi2c2, addr, reg, 1, data, 2, 1000);
}

uint16_t TMP468read(uint16_t addr, uint8_t reg){
  addr <<=1;
  uint8_t data[2]={0};
  HAL_I2C_Mem_Read(&hi2c2, addr, reg, 1, data, 2, 1000);
  uint16_t res = (data[0]<<8)|(data[1]);
  return res;
}

void testTMP469(void){
	TMP468_t target  = TMP468_init(0x48, TMP468_CONVERSION_RATE_CONTINUOUS, 0, 255.5, 255.5,
					 TMP468write, TMP468read);


//	uint16_t mid = target.i2c_read(target.addr, TMP468_REG_MANID);
//	uint16_t did = target.i2c_read(target.addr, TMP468_REG_DEVID);
//	log_printf("TI instruments TMP469 mid:%x did:%d \r\n", mid, did);

	TMP468_unlock(target);

	do{
		   for (int i = 0; i<9; i++){
			   log_printf("ch%d->%5d  ", i, TMP468_getRemoteTemperature(target, i));
		   }
		   log_printf("\r");
//		   HAL_Delay(100);
	}while(0);

}

void powerUpSequence(){
  const int POWER_GOOD_MAX_DELAY = 1000; // 100; //ms

  log_printf("Power up sequence started with %d ms until HCF \r\n", POWER_GOOD_MAX_DELAY);

//	log_printf("Press button to enable Main Power\r\n");
//  waitBtnPress();
  HAL_GPIO_WritePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin, GPIO_PIN_SET); 
  log_printf("Main power on; PG not implemented, using max delay \r\n");
  HAL_Delay(POWER_GOOD_MAX_DELAY);

  log_printf("Press button to enable 12V Power\r\n");
  
  HAL_GPIO_WritePin(_12V0P_EN_GPIO_Port, _12V0P_EN_Pin, GPIO_PIN_SET);     
  log_printf("12V  power on; waiting for PowerGood signal\r\n");
  if (!waitSignalTimeout(_12V0P_EN_GPIO_Port, MAIN_12V0P_PG_Pin, GPIO_PIN_SET, POWER_GOOD_MAX_DELAY) ){
	  log_printf("[ERROR] Timeout on MAIN_12V0P_PG! \r\n HCF!\r\n"); HCF();
  } else {
	  log_printf("MAIN_12V0P_PG asserted\r\n");
  }


  log_printf("Press button to enable 3V3 Power\r\n");
  waitBtnPress();
  HAL_GPIO_WritePin(CPU_3V3P_EN_GPIO_Port, CPU_3V3P_EN_Pin, GPIO_PIN_SET); 
  log_printf("3V3  power on; waiting for PowerGood signal\r\n");
  if (!waitSignalTimeout(CPU_3V3P_PG_GPIO_Port, CPU_3V3P_PG_Pin, GPIO_PIN_SET, POWER_GOOD_MAX_DELAY) ){
	  log_printf("[ERROR] Timeout on CPU_3V3P_PG! \r\n HCF!\r\n"); HCF();
  } else {
	  log_printf("CPU_3V3P_PG asserted\r\n");
  }


  log_printf("Press button to enable 5V Power\r\n");
  waitBtnPress();
  HAL_GPIO_WritePin(_5V0P_EN_GPIO_Port, _5V0P_EN_Pin, GPIO_PIN_SET);       
  log_printf("5V   power on; waiting for PowerGood signal\r\n");
  if (!waitSignalTimeout(_5V0P_PG_GPIO_Port, _5V0P_PG_Pin, GPIO_PIN_SET, POWER_GOOD_MAX_DELAY) ){
	  log_printf("[ERROR] Timeout on _5V0P_PG! \r\n HCF!\r\n"); HCF();
  } else {
	  log_printf("_5V0P_PG asserted\r\n");
  }


  log_printf("Press button to enable PMIC Power\r\n");
  waitBtnPress();
  HAL_GPIO_WritePin(PMIC_EN_GPIO_Port, PMIC_EN_Pin, GPIO_PIN_SET);         
  log_printf("PMIC is enabled; waiting for PowerGood signal \r\n");
  if (!waitSignalTimeout(PMIC_POR_B_GPIO_Port, PMIC_POR_B_Pin, GPIO_PIN_SET, POWER_GOOD_MAX_DELAY) ){
	  log_printf("[ERROR] Timeout on PMIC_POR_B! \r\n HCF!\r\n"); HCF();
  } else {
	  log_printf("PMIC_POR_B asserted\r\n");
  }


  //-------------------------------------------------------------------------------------//

  log_printf("Checking power good on automatic rails:\r\n");

  if (!waitSignalTimeout(CPU_1V2P_PG_GPIO_Port, CPU_1V2P_PG_Pin, GPIO_PIN_SET, POWER_GOOD_MAX_DELAY) ){
	  log_printf("[ERROR] Timeout on CPU_1V2P_PG! \r\n HCF!\r\n");
//	  HCF();
  } else {
	  log_printf("CPU_1V2P_PG asserted\r\n");
  }

  if (!waitSignalTimeout(CPU_CORE_1V0P_PG_GPIO_Port, CPU_CORE_1V0P_PG_Pin, GPIO_PIN_SET, POWER_GOOD_MAX_DELAY) ){
	  log_printf("[ERROR] Timeout on CPU_CORE_1V0P_PG! \n HCF!\r\n");
//	  HCF();
  } else {
	  log_printf("CPU_CORE_1V0P_PG asserted\r\n");
  }

  if (!waitSignalTimeout(CPU_DDR_PG_GPIO_Port, CPU_DDR_PG_Pin, GPIO_PIN_SET, POWER_GOOD_MAX_DELAY) ){
	  log_printf("[ERROR] Timeout on CPU_DDR_PG! \r\n HCF!\r\n");
//	  HCF();
  } else {
	  log_printf("CPU_DDR_PG asserted\r\n");
  }

  log_printf("Power up sequence finished!\r\n");
  HAL_Delay(2000);
  log_printf("\033[2J"); //clear and
  do{
	  HAL_Delay(2000);
	  log_printf("\033[H"); //move cursor home
	  reportADC();
	  testINA230(0x40, 2);
	  testINA230(0x41, 2);
	  testINA230(0x42, 4);
	  testINA230(0x43, 4);
	  testINA230(0x44, 4);
	  testINA230(0x45, 1);
	  testINA230(0x46, 2);
	  testINA230(0x47, 2);
	  testINA230(0x48, 1);
	  testINA230(0x49, 2);
	  testINA230(0x4a, 2);

	  testTMP469();
  }while(1);

  	HCF(); //temporary stop
	return;
}

void powerDownSequence(){
	/* Reverse the power up sequence */
	log_printf("Power down started\r\n");
	const int POWER_DOWN_DELAY = 50; //ms
	// WARN: Ordering of the powerdown is important!
	HAL_GPIO_WritePin(PMIC_EN_GPIO_Port, PMIC_EN_Pin, GPIO_PIN_RESET);         log_printf("PMIC  disabled \r\n"); HAL_Delay(POWER_DOWN_DELAY);
	HAL_GPIO_WritePin(CPU_3V3P_EN_GPIO_Port, CPU_3V3P_EN_Pin, GPIO_PIN_RESET); log_printf("3V3   power off\r\n"); HAL_Delay(POWER_DOWN_DELAY);
	HAL_GPIO_WritePin(_5V0P_EN_GPIO_Port, _5V0P_EN_Pin, GPIO_PIN_RESET);       log_printf("5V    power off\r\n"); HAL_Delay(POWER_DOWN_DELAY);
	HAL_GPIO_WritePin(_12V0P_EN_GPIO_Port, _12V0P_EN_Pin, GPIO_PIN_RESET);     log_printf("12V   power off\r\n"); HAL_Delay(POWER_DOWN_DELAY);
	HAL_GPIO_WritePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin, GPIO_PIN_RESET); log_printf("Main  power off\r\n"); HAL_Delay(POWER_DOWN_DELAY);

	log_printf("Power down sequence finished!\r\n");
	return;
}

GPIO_PinState checkPowerGood(){
	/* Sample all the power good signals*/
	GPIO_PinState pg = GPIO_PIN_SET;

	pg &= HAL_GPIO_ReadPin(MAIN_12V0P_PG_GPIO_Port, MAIN_12V0P_PG_Pin);
	pg &= HAL_GPIO_ReadPin(CPU_3V3P_PG_GPIO_Port, CPU_3V3P_PG_Pin);
	pg &= HAL_GPIO_ReadPin(_5V0P_PG_GPIO_Port, _5V0P_PG_Pin);
	pg &= HAL_GPIO_ReadPin(PMIC_POR_B_GPIO_Port, PMIC_POR_B_Pin);

	pg &= HAL_GPIO_ReadPin(CPU_1V2P_PG_GPIO_Port, CPU_1V2P_PG_Pin);
	pg &= HAL_GPIO_ReadPin(CPU_CORE_1V0P_PG_GPIO_Port, CPU_CORE_1V0P_PG_Pin);
	pg &= HAL_GPIO_ReadPin(CPU_DDR_PG_GPIO_Port, CPU_DDR_PG_Pin);

	HAL_Delay(STEP_DELAY);
	return pg;
}

void RCW(enum SUPERVISOR_STATE PinState){
	static unsigned char ttable[]={0x0, 0x8, 0x9, 0xa, 0xd, 0xf, 0x00, 0x00};
	unsigned char RCW;
	if(PinState == S_ASSERT ){
		unsigned char dipState =   HAL_GPIO_ReadPin(CFG1_GPIO_Port, CFG1_Pin) \
								 | HAL_GPIO_ReadPin(CFG2_GPIO_Port, CFG2_Pin) << 1 \
								 | HAL_GPIO_ReadPin(CFG3_GPIO_Port, CFG3_Pin) << 2;
		RCW = ttable[dipState];
		log_printf("DipSwitch state: 0x%x \r\n", dipState);
	}
	else{
		RCW = 0b1111; //When the power up procedure is finished, and the RCW is not needed
                      //all the pins are set to 1
	}
    log_printf("RCW: 0x%x \r\n", RCW);

#define GET_BIT(c, n) (((c) >> (n)) & 1)
#define RCW_BIT(c, n) ( GET_BIT(c, n)? GPIO_PIN_SET: GPIO_PIN_RESET)

    HAL_GPIO_WritePin(CFG_RCW_SRC0_D_GPIO_Port, CFG_RCW_SRC0_D_Pin, RCW_BIT(RCW, 0));
    HAL_GPIO_WritePin(CFG_RCW_SRC1_D_GPIO_Port, CFG_RCW_SRC1_D_Pin, RCW_BIT(RCW, 1));
    HAL_GPIO_WritePin(CFG_RCW_SRC2_D_GPIO_Port, CFG_RCW_SRC2_D_Pin, RCW_BIT(RCW, 2));
    HAL_GPIO_WritePin(CFG_RCW_SRC3_D_GPIO_Port, CFG_RCW_SRC3_D_Pin, RCW_BIT(RCW, 3));

	HAL_Delay(STEP_DELAY);
	return;

#undef GET_BIT
#undef RCW_BIT
}

void MCU_RST(enum SUPERVISOR_STATE state){

	GPIO_PinState PinState;

	if (state == S_ASSERT) PinState=GPIO_PIN_RESET; // Reset is activated on a low-voltage state.
	else PinState=GPIO_PIN_SET;

	HAL_GPIO_WritePin(MCU_PORESET_B_GPIO_Port, MCU_PORESET_B_Pin, PinState);
	HAL_GPIO_WritePin(MCU_HRESET_B_GPIO_Port, MCU_HRESET_B_Pin, PinState);
	HAL_GPIO_WritePin(MCU_RESET_REQ_B_GPIO_Port, MCU_RESET_REQ_B_Pin, PinState);
	HAL_GPIO_WritePin(MCU_DRR4_RST_N_GPIO_Port, MCU_DRR4_RST_N_Pin, PinState);
	HAL_GPIO_WritePin(MCU_eMMC_RST_N_GPIO_Port, MCU_eMMC_RST_N_Pin, PinState);
	HAL_GPIO_WritePin(MCU_NOR_RST_N_GPIO_Port, MCU_NOR_RST_N_Pin, PinState);
	HAL_Delay(STEP_DELAY);
	return;
}

void enableClocks(){
	HAL_GPIO_WritePin(SYS_CLK_EN_MCU_GPIO_Port, SYS_CLK_EN_MCU_Pin, GPIO_PIN_SET);
	HAL_Delay(STEP_DELAY);
	return;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_FLASH_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_I2C4_Init();
  MX_LPUART1_UART_Init();
  MX_LPUART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /*
   * Power up
   */
  //Enable the RS422 level shifter
  HAL_GPIO_WritePin(MCU_RS422_EN_GPIO_Port, MCU_RS422_EN_Pin, GPIO_PIN_SET);
  //Enable i_sense
  HAL_GPIO_WritePin(DIAG_EN_GPIO_Port, DIAG_EN_Pin, GPIO_PIN_SET);
  log_printf("\033[2J\033[H"); //clear and move cursor home
  log_printf("Supervisor version " VERSION "\r\n");
  log_printf("------------------------------\r\n\r\n");

  HAL_Delay(100);


  MCU_RST(S_ASSERT);
  log_printf("MCU reset signal asserted.\r\n");
  powerUpSequence();
  /*Set RCW*/
  RCW(S_ASSERT);
  enableClocks();
  MCU_RST(S_RELEASE);
  RCW(S_RELEASE);
  unsigned int loopCounter=0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (!checkPowerGood()){
		  //TODO: PowerGood as interrupts, or moving power up sequencing into a state machine.
		  powerDownSequence();
		  HCF();
	  }
	  loopCounter++;
	  HAL_GPIO_WritePin(STATUS_LED1_GREEN_GPIO_Port, STATUS_LED1_GREEN_Pin, (loopCounter >> 2)&0x1); //blink green every multiple of the counter
	  HAL_Delay(STEP_DELAY);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief FLASH Initialization Function
  * @param None
  * @retval None
  */
static void MX_FLASH_Init(void)
{

  /* USER CODE BEGIN FLASH_Init 0 */

  /* USER CODE END FLASH_Init 0 */

  /* USER CODE BEGIN FLASH_Init 1 */

  /* USER CODE END FLASH_Init 1 */
  if (HAL_FLASH_Unlock() != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_FLASH_Lock() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FLASH_Init 2 */

  /* USER CODE END FLASH_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00303D5B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00303D5B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00303D5B;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x00303D5B;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief LPUART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART2_UART_Init(void)
{

  /* USER CODE BEGIN LPUART2_Init 0 */

  /* USER CODE END LPUART2_Init 0 */

  /* USER CODE BEGIN LPUART2_Init 1 */

  /* USER CODE END LPUART2_Init 1 */
  hlpuart2.Instance = LPUART2;
  hlpuart2.Init.BaudRate = 209700;
  hlpuart2.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart2.Init.StopBits = UART_STOPBITS_1;
  hlpuart2.Init.Parity = UART_PARITY_NONE;
  hlpuart2.Init.Mode = UART_MODE_TX_RX;
  hlpuart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart2.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART2_Init 2 */

  /* USER CODE END LPUART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CFG_RCW_SRC0_D_Pin|CFG_RCW_SRC1_D_Pin|CFG_RCW_SRC2_D_Pin|CFG_RCW_SRC3_D_Pin
                          |UEFI_SDA_Pin|CFG_ENABLE_N_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MAIN_PWR_EN_Pin|DIAG_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, STATUS_LED1_GREEN_Pin|STATUS_LED2_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, _12V0P_EN_Pin|_5V0P_EN_Pin|CPU_3V3P_EN_Pin|PMIC_EN_Pin
                          |MCU_RS422_EN_Pin|SYS_CLK_EN_MCU_Pin|UEFI_CPU_EN_Pin|UEFI_CPU_WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, MCU_PORESET_B_Pin|MCU_HRESET_B_Pin|MCU_RESET_REQ_B_Pin|MCU_DRR4_RST_N_Pin
                          |MCU_eMMC_RST_N_Pin|MCU_NOR_RST_N_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CFG2_Pin SNS_I2C_ALERT_Pin UEFI_SCL_Pin PUSH_BUTTON_Pin
                           CFG1_Pin */
  GPIO_InitStruct.Pin = CFG2_Pin|SNS_I2C_ALERT_Pin|UEFI_SCL_Pin|PUSH_BUTTON_Pin
                          |CFG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : MCU_nRST_Pin */
  GPIO_InitStruct.Pin = MCU_nRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MCU_nRST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CFG_RCW_SRC0_D_Pin CFG_RCW_SRC1_D_Pin CFG_RCW_SRC2_D_Pin CFG_RCW_SRC3_D_Pin
                           UEFI_SDA_Pin CFG_ENABLE_N_Pin */
  GPIO_InitStruct.Pin = CFG_RCW_SRC0_D_Pin|CFG_RCW_SRC1_D_Pin|CFG_RCW_SRC2_D_Pin|CFG_RCW_SRC3_D_Pin
                          |UEFI_SDA_Pin|CFG_ENABLE_N_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : MAIN_PWR_EN_Pin DIAG_EN_Pin */
  GPIO_InitStruct.Pin = MAIN_PWR_EN_Pin|DIAG_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : MAIN_12V0P_PG_Pin CPU_3V3P_PG_Pin CPU_1V2P_PG_Pin CPU_CORE_1V0P_PG_Pin
                           CPU_DDR_PG_Pin */
  GPIO_InitStruct.Pin = MAIN_12V0P_PG_Pin|CPU_3V3P_PG_Pin|CPU_1V2P_PG_Pin|CPU_CORE_1V0P_PG_Pin
                          |CPU_DDR_PG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : _5V0P_PG_Pin */
  GPIO_InitStruct.Pin = _5V0P_PG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(_5V0P_PG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : STATUS_LED1_GREEN_Pin STATUS_LED2_RED_Pin */
  GPIO_InitStruct.Pin = STATUS_LED1_GREEN_Pin|STATUS_LED2_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : _12V0P_EN_Pin _5V0P_EN_Pin CPU_3V3P_EN_Pin PMIC_EN_Pin
                           MCU_RS422_EN_Pin SYS_CLK_EN_MCU_Pin UEFI_CPU_EN_Pin UEFI_CPU_WP_Pin */
  GPIO_InitStruct.Pin = _12V0P_EN_Pin|_5V0P_EN_Pin|CPU_3V3P_EN_Pin|PMIC_EN_Pin
                          |MCU_RS422_EN_Pin|SYS_CLK_EN_MCU_Pin|UEFI_CPU_EN_Pin|UEFI_CPU_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PMIC_INTB_Pin PMIC_POR_B_Pin MCU_THERMAL_ERR_Pin CFG3_Pin */
  GPIO_InitStruct.Pin = PMIC_INTB_Pin|PMIC_POR_B_Pin|MCU_THERMAL_ERR_Pin|CFG3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : MCU_CPU_GPIO0_Pin MCU_CPU_GPIO1_Pin MCU_WATCHDOG_PULSE_Pin */
  GPIO_InitStruct.Pin = MCU_CPU_GPIO0_Pin|MCU_CPU_GPIO1_Pin|MCU_WATCHDOG_PULSE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : MCU_PORESET_B_Pin MCU_HRESET_B_Pin MCU_RESET_REQ_B_Pin MCU_DRR4_RST_N_Pin
                           MCU_eMMC_RST_N_Pin MCU_NOR_RST_N_Pin */
  GPIO_InitStruct.Pin = MCU_PORESET_B_Pin|MCU_HRESET_B_Pin|MCU_RESET_REQ_B_Pin|MCU_DRR4_RST_N_Pin
                          |MCU_eMMC_RST_N_Pin|MCU_NOR_RST_N_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
