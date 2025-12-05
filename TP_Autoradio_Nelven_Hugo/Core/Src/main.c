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
#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h> // pour atoi function

#include "shell.h"
#include "chenille.h"
#include "sgtl5000.h"
#include "sgtl5000_signals.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int chr)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&chr, 1, HAL_MAX_DELAY);
	return chr;
}

uint8_t drv_uart_receive(char * pData, uint16_t size)
{
	HAL_UART_Receive(&huart2, (uint8_t*)pData, size, HAL_MAX_DELAY);
	return 0;
}

uint8_t drv_uart_transmit(char * pData, uint16_t size)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)pData, size, HAL_MAX_DELAY);
	return 0;
}

h_shell_t h_shell =
{
		.drv_shell = {
				.drv_shell_transmit = drv_uart_transmit,
				.drv_shell_receive = drv_uart_receive
		}
};

int fonction(h_shell_t * h_shell, int argc, char ** argv)
{
	printf("Je suis une fonction bidon\r\n");

	printf("argc = %d\r\n", argc);

	for (int i = 0 ; i < argc ; i++)
	{
		printf("argv[%d] = %s\r\n", i, argv[i]);
	}

	return 0;
}

int addition(h_shell_t * h_shell, int argc, char ** argv)
{
	if (argc != 3)
	{
		printf("Error: expected two arguments\r\n");
		return -1;
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int c = a + b;

	printf("%d + %d = %d\r\n", a, b, c);

	return 0;
}

int led_control(h_shell_t *h_shell, int argc, char **argv)
{
	int numero;
	int size;
	numero = atoi(argv[1]);
	if (argc==3)
	{
		if ((numero > 8) && (numero < 17)){
			if (numero > 8)
			{
				Select_LED('B', 16-numero,atoi(argv[2]));
			}
			else if (numero < 8)
			{
				int state = atoi(argv[2]);
				Select_LED('A', numero,state);
			}
			else {
				size = snprintf(h_shell->print_buffer,BUFFER_SIZE,"[N>16] ! Il y a pas assez de led\r\n");
			}
			size = snprintf(h_shell->print_buffer,BUFFER_SIZE,"LED OK\r\n");
		}
		else{
			size = snprintf(h_shell->print_buffer,BUFFER_SIZE,"[N>16] ! Il y a pas assez de led\r\n");
		}
	}
	else if (argc==4)
	{
		numero = atoi(argv[1]) * 10 + atoi(argv[2]);
		if (numero < 17){
			Select_LED('B', 16-numero,atoi(argv[3]));
			size = snprintf(h_shell->print_buffer,BUFFER_SIZE,"LED OK\r\n");
		}
		else{
			size = snprintf(h_shell->print_buffer,BUFFER_SIZE,"[N>16] ! Il y a pas assez de led\r\n");
		}
	}
	else
	{
		size = snprintf(h_shell->print_buffer,BUFFER_SIZE,"Écrire : l <id[0:16]> <0|1>\r\n");
	}
	drv_uart_transmit(h_shell->print_buffer,size);
	return 0;
}

void task_shell(void * unused)
{
	shell_init(&h_shell);
	shell_add(&h_shell, 'f', fonction, "Une fonction inutile");
	shell_add(&h_shell, 'a', addition, "Ma super addition");
	shell_add(&h_shell, 'l', led_control, "LED control: l <id[0:16]> <0|1>");
	shell_run(&h_shell);

	// Une tâche ne doit *JAMAIS* retourner
	// Ici elle ne retourne pas parce qu'il y a une boucle infinie dans shell_run();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)	// LPUART1
	{
		// Caractère reçu : Donner le sémaphore pour débloquer task_shell
		shell_uart_rx_callback(&h_shell);
	}
}

void task_led(void * unused)
{
	for (;;)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		vTaskDelay(250);
	}
}

void task_chenillard(void *unused)
{
	uint8_t led_state = 0x01;
	for (;;)
	{
		// Allume la LED correspondante sur MCP23S17 port A
		//	    	MCP23S17_WriteRegister(0x12, led_state);

		// Décale la LED à allumer
		led_state <<= 1;
		if (led_state == 0) {
			led_state = 0x01;  // Reset au début du chenillard
		}

		vTaskDelay(pdMS_TO_TICKS(200));  // délai 200 ms


	}
}


void task_i2c_update(void *unused)
{
    uint8_t id = 0;
    HAL_StatusTypeDef ret;
	for (;;)
	{
	    ret = HAL_I2C_Mem_Read(&hi2c2, (uint16_t)(CODEC_I2C_ADDR), (uint16_t)CODEC_REG_CHIP_ID, (uint16_t)I2C_MEMADD_SIZE_8BIT, &id, (uint16_t)sizeof(id), HAL_MAX_DELAY);

	    if (ret != HAL_OK){
	    	printf("error i2c read init\r\n");
	    	Error_Handler();
	    }

	    vTaskDelay(pdMS_TO_TICKS(100));
	}
}


void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
	Error_Handler();
}




//void configureTimerForRunTimeStats(void)
//{
//	// Démarrer un timer
//	HAL_TIM_Base_Start(&htim2);
//}
//
//unsigned long getRunTimeCounterValue(void)
//{
//	static unsigned long value = 0;
//	static unsigned long counter_prev = 0;
//
//	unsigned long counter = __HAL_TIM_GET_COUNTER(&htim2);
//
//	if (counter < counter_prev)
//	{
//		value += 65535;
//	}
//
//	counter_prev = counter;
//
//	// Renvoyer la valeur du timer
//	return value + counter;
//}
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

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SPI3_Init();
  MX_I2C2_Init();
  MX_SAI2_Init();
  /* USER CODE BEGIN 2 */
	MCP23S17_Init();
	MCP23S17_SetAllPinsLow();
	HAL_Delay(1000);
	MCP23S17_SetAllPinsHigh();

	__HAL_SAI_ENABLE(&hsai_BlockA2);

	if (CODEC_Init()!= HAL_OK){
		printf("error codec init\r\n");
		Error_Handler();
	}



	if (sgtl5000_init(&sgtl5000) != HAL_OK) {
	    Error_Handler();
	}

	// Démarre la transmission DMA
	if (sgtl5000_start(&sgtl5000) != HAL_OK) {
	    Error_Handler();
	}


//	sgtl5000_fill_triangle(&sgtl5000, 12000);

	printf("==== Autoradio Hugo Nelven Start ====\r\n");
	if (xTaskCreate(task_shell, "Shell", 512, NULL, 1, NULL) != pdPASS)
	{
		printf("Error creating task Shell\r\n");
		Error_Handler();
	}

	if (xTaskCreate(task_led, "LED", 128, NULL, 2, NULL) != pdPASS)
	{
		printf("Error creating task LED\r\n");
		Error_Handler();
	}

	if (xTaskCreate(task_chenillard, "Chenillard", 512, NULL, 3, NULL) != pdPASS)
	{
		printf("Error creating task Chenillard\r\n");
		Error_Handler();
	}

	if (xTaskCreate(task_i2c_update, "i2c update", 512, NULL, 4, NULL) != pdPASS)
	{
		printf("Error creating task i2c update\r\n");
		Error_Handler();
	}

	vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
  PeriphClkInit.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 13;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV17;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
