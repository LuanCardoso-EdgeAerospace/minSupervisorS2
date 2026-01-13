/*
 * util.c
 *
 *  Created on: Dec 15, 2025
 *      Author: luan
 */

#include "main.h"
#include "util.h"

extern UART_HandleTypeDef hlpuart1;
extern ADC_HandleTypeDef hadc1;

#ifndef LOG_UART_TIMEOUT_MS
#define LOG_UART_TIMEOUT_MS 100
#endif

#ifndef LOG_LINE_BUF
#define LOG_LINE_BUF 128 //buffer for writing. Try to not make writes too long
#endif
/* Hook points: you will replace these later with DMA/ring-buffer code */
static void log_backend_write_blocking(const uint8_t *p, uint16_t n) {
  if (n == 0) return;
  (void)HAL_UART_Transmit(&hlpuart1, (uint8_t*)p, n, LOG_UART_TIMEOUT_MS);
}

void log_write(const void *data, uint16_t len) {
  log_backend_write_blocking((const uint8_t*)data, len);
}

void log_printf(const char *fmt, ...){
	//For now this function is blocking
    va_list ap;
	va_start(ap, fmt);
	char buff[LOG_LINE_BUF];
	int n = vsnprintf(buff, sizeof(buff), fmt, ap);
	va_end(ap);

	if (n<0) return;

	uint16_t total = (uint16_t)strnlen(buff, sizeof(buff));
	log_backend_write_blocking((const uint8_t*)buff, total);
}

//busywait for a button to be pressed
void waitBtnPress(){
# ifdef WAIT_BTN_PRESS
	const int DEBOUNCE_DELAY = 20;
	//red LED to signal
	HAL_GPIO_WritePin(STATUS_LED2_RED_GPIO_Port, STATUS_LED2_RED_Pin, GPIO_PIN_SET);


	while (HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin) == GPIO_PIN_RESET); // wait for buttonpress
	HAL_Delay(DEBOUNCE_DELAY);
    if (HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin) == GPIO_PIN_SET){     // Confirm still pressed
    	while (HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin) == GPIO_PIN_SET); // Wait for release
    }

	HAL_GPIO_WritePin(STATUS_LED2_RED_GPIO_Port, STATUS_LED2_RED_Pin, GPIO_PIN_RESET);
#else
	return;
#endif //WAIT_BTN_PRESS
}

int waitSignalTimeout(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState State, uint32_t TimeoutMs){
	uint32_t start = HAL_GetTick();
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != State){
		if ((HAL_GetTick() - start) > TimeoutMs){
			return 0; //timeout
		}
	}
	return 1; //success
}

uint16_t ADC_IN0(){
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_39CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, ADCTimeout);
	uint16_t value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

    return value;
}

uint16_t ADC_IN1(){
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_39CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, ADCTimeout);
	uint16_t value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

    return value;
}

void reportADC(){  //read voltage from ADC to confirm 12V rail is up.
	  uint16_t IN_VOL_MEAS_RAW = ADC_IN0();
	  uint16_t I_SENSE_RAW = ADC_IN1();

	  //apply conversion
	  /*
	   * convert measurement from the ADC to volts:
	   * avdc = adcVal/(2^BIT_RESOLUTION -1) * VDDA
	   *
	   * Apply the inverse of the voltage divier
	   *
	   * voltage =  (100k+4.7k)/4.7k * vadc
	   */
	  uint16_t milvoltage = IN_VOL_MEAS_RAW*17.9518;

	  log_printf("> IN_VOL_MEAS = %d mV (%d adc) \r\n", milvoltage, IN_VOL_MEAS_RAW);

	  /*
	   *
	   */
	  uint16_t amps = I_SENSE_RAW * 0.531337; //(x/(2^12 - 1)×3.3)*(1200/1820)*1000
	  log_printf("> I_SENSE = %d mA (%d raw) \r\n", amps, I_SENSE_RAW );
}


#define REDON(n) do{ \
					 HAL_GPIO_WritePin(STATUS_LED2_RED_GPIO_Port, STATUS_LED2_RED_Pin, GPIO_PIN_SET); \
					 HAL_Delay(n); \
					 HAL_GPIO_WritePin(STATUS_LED2_RED_GPIO_Port, STATUS_LED2_RED_Pin, GPIO_PIN_RESET);\
				}while(0)

void HCF(void){
/* Function to be called when you want to enter an infinite loop. Will blink the red LED */
	const int onPeriod=100, offShort=200, offLong=600;
	do{
		REDON(onPeriod); HAL_Delay(offShort);
		REDON(onPeriod); HAL_Delay(offLong);
	}while(1);

//	REDON(10000);

#undef REDON
}






















