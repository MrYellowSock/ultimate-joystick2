/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_hid.h"
#include <stdarg.h>
#include <button.h>
#include <joystick.h>
#include <rgb.h>
#include <macro.h>
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern USBD_HandleTypeDef hUsbDeviceFS;
volatile uint32_t adc_dma_result[adc_channel_count];
volatile uint8_t reportBuffer[COMMAND_SIZE] = { 0 };
volatile int16_t forces[2] = { 0 };
volatile uint32_t macroDownTick = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void sendString(UART_HandleTypeDef huart, char *tosend) {
	while (__HAL_UART_GET_FLAG(&huart,UART_FLAG_TC) == RESET) {
	}
	HAL_UART_Transmit(&huart, (uint8_t*) tosend, strlen(tosend),
	HAL_MAX_DELAY);
}
void setcmd_Button(uint8_t button, uint8_t state) {
	uint8_t *target = (button < 8) ? &reportBuffer[0] : &reportBuffer[1];
	uint8_t realButton = button % 8;
	if (state) {
		*target |= (1 << realButton);
	} else {
		*target ^= (1 << realButton);
	}
}
void setcmd_LeftXY(int8_t x, int8_t y) {
	reportBuffer[2] = x;
	reportBuffer[3] = y;
}
void setcmd_RightXY(int8_t x, int8_t y) {
	reportBuffer[4] = x;
	reportBuffer[5] = y;
}

int average_64(int x){
	static int samples[16];
	static int i = 0;
	static int total = 0;

	total += x- samples[i];
	samples[i] = x;

	i = (i==15?0:i+1);

	return total >> 4;
}

// map your rgb to each channel
void writeColor(Color c) {
	float brightness = (float)(4095 - average_64(adc_dma_result[4])) / (float)4096;
	htim1.Instance->CCR3 = c.blue * brightness;
	htim1.Instance->CCR2 = c.green * brightness;
	htim1.Instance->CCR1 = c.red * brightness;
}

void sendCustomReport(uint8_t buffer[COMMAND_SIZE]) {
	USBD_HID_SendReport(&hUsbDeviceFS, buffer, COMMAND_SIZE);
}

void sendReport() {
	sendCustomReport(reportBuffer);
}

void writeVibration() {
	htim4.Instance->CCR3 = (forces[0] > 3) ? forces[0] : 0;
	htim4.Instance->CCR4 = (forces[1] > 3) ? forces[1] : 0;
}
void DEBUGPRINTF(const char *format, ...) {
	static char buffer[500]; // You can adjust the buffer size as needed
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	sendString(huart3, buffer);
}

int led0LightAngle = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		Color hsv = mkColor_sineLED(led0LightAngle);
		writeColor(hsv);
		writeVibration();
		if (getMacroState() == MACRO_STATE_PLAY) {
			led0LightAngle = (led0LightAngle + 10) % 360;
		} else if (getMacroState() == MACRO_STATE_NONE) {
			led0LightAngle = (led0LightAngle + 1) % 360;
		}

	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	HAL_ADC_Stop_DMA(&hadc1);
	StickStateEvent ev = onSticksChanged(adc_dma_result);
	if (ev.isChanged) {
		setcmd_LeftXY(ev.x, ev.y);
		setcmd_RightXY(ev.Rx, ev.Ry);
		if (ev.x > 0) {
			forces[0] = ev.x;
		} else if (ev.x < 0) {
			forces[1] = -ev.x;
		}
//		forces[1] = MAX(abs(ev.x), abs(ev.y));
//		forces[0] = MAX(abs(ev.Rx), abs(ev.Ry));
		sendReport();
	}

	HAL_Delay(3);
//	DEBUGPRINTF("%u %u %u %u\n", x, y, Rx, Ry);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_dma_result, adc_channel_count);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	ButtonStateEvent ev = onButtonPinINT(GPIO_Pin);
	if (ev.button == 0xFF)
		return;
	else if (ev.button == BUTTON_MACRO) {
		if (ev.state == 1) {
			macroDownTick = HAL_GetTick();
		} else if (HAL_GetTick() - macroDownTick > 500) {
			recordToggle();
		} else {
			playToggle();
		}
	} else {
		setcmd_Button(ev.button, ev.state);
		recordAdd(reportBuffer);
		sendReport();
		DEBUGPRINTF("%u %u %u %u %u %u\n", reportBuffer[0], reportBuffer[1],
				reportBuffer[2], reportBuffer[3], reportBuffer[4],
				reportBuffer[5]);
	}
	DEBUGPRINTF("%u %u\n", ev.button, ev.state);
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
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  MX_TIM6_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_dma_result, adc_channel_count);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	HAL_TIM_Base_Start_IT(&htim6);
	Action now;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if (getMacroState() == MACRO_STATE_PLAY) {
			now = nextAction();
//			DEBUGPRINTF("%u %u %u %u %u %u %u\n", now.command[0],
//					now.command[1], now.command[2], now.command[3],
//					now.command[4], now.command[5], now.timeSpan);
			memcpy(reportBuffer, now.command, COMMAND_SIZE);
			HAL_Delay(now.timeSpan);
			sendReport();
		}
		DEBUGPRINTF("LDR: %d\n",adc_dma_result[4]);
		HAL_Delay(500);
		// array {ปุ่ม,ปุ่ม,x,y,z}
		// uint8_t HID_buffer[5] = { 1, 1, 0, 0, 0 };
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		for (int i = 0; i < 16; i++) {
//			setcmd_Button(i, 1);
//			sendCommand();
//			HAL_Delay(300);
//			setcmd_Button(i, 0);
//			sendCommand();
//			HAL_Delay(100);
//		}
//		for (int i = -127; i < 127; i++) {
//			setcmd_LeftXY(i,i);
//			setcmd_RightXY(i,i);
//			sendCommand();
//			HAL_Delay(10);
//		}
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
