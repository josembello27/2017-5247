/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ESTADO_ERROR        0
#define ESTADO_ABIERTO      1
#define ESTADO_CERRADO      2
#define ESTADO_ABRIENDO     3
#define ESTADO_CERRANDO     4
#define ESTADO_INTERMEDIO   5
#define ESTADO_INIT         6
#define FALSE 0
#define TRUE  1
#define inTrue  0
#define inFalse 1
#define TIME_Ca 60
#define PUERTO GPIOA
#define PIN_Sa GPIO_PIN_0    //Sensor Puerta Abierta
#define PIN_Sc GPIO_PIN_4    //Sensor Puerta Cerrada
#define PIN_Ma GPIO_PIN_6    //Motor para Abrir
#define PIN_Mc GPIO_PIN_7    //Motor para Cerrar
#define PIN_Ba GPIO_PIN_9    //Boton Abrir
#define PIN_Bc GPIO_PIN_8    //Boton Cerrar
#define PIN_Led GPIO_PIN_5
#define LED_ON    1
#define LED_OFF   0
#define INTER_ON  1     //Intermitente
#define INTER_OFF 0
#define INTER_RAPIDO 25 //Definicion del tiempo rapido multiplo de 10ms
#define INTER_LENTO  50
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_DRD_FS;

/* USER CODE BEGIN PV */
int Func_ESTADO_ERROR(void);
int Func_ESTADO_ABIERTO(void);
int Func_ESTADO_CERRADO(void);
int Func_ESTADO_ABRIENDO(void);
int Func_ESTADO_CERRANDO(void);
int Func_ESTADO_INTERMEDIO(void);
int Func_ESTADO_INIT(void);
int Func_ESTADO_SETTINGLED(int, int);        //Funcion para configurar el Led

volatile int ESTADO_ANTERIOR = ESTADO_INIT;
volatile int ESTADO_ACTUAL = ESTADO_INIT;
volatile int ESTADO_SIGUIENTE = ESTADO_INIT;
volatile int CntTimeCa = 0;
volatile struct INOUT
{
    unsigned int Sa:1;
    unsigned int Sc:1;
    unsigned int Mc:1;
    unsigned int Ma:1;
    unsigned int Bc:1;
    unsigned int Ba:1;
    unsigned int Led:1;
} inout;
volatile struct LEDS
{
	unsigned int StatusLED: 1;      //Estado del Led
	unsigned int InterLED;          //Intermitencia del Led
}LedStatus;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_ICACHE_Init();
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	     if(ESTADO_SIGUIENTE == ESTADO_INIT)
	      {
	          ESTADO_SIGUIENTE = Func_ESTADO_INIT();
	      }

	      if(ESTADO_SIGUIENTE == ESTADO_ABIERTO)
	      {
	          ESTADO_SIGUIENTE = Func_ESTADO_ABIERTO();
	      }
	      if(ESTADO_SIGUIENTE == ESTADO_CERRADO)
	      {
	          ESTADO_SIGUIENTE = Func_ESTADO_CERRADO();
	      }
	      if(ESTADO_SIGUIENTE == ESTADO_ABRIENDO)
	      {
	          ESTADO_SIGUIENTE = Func_ESTADO_ABRIENDO();
	      }
	      if(ESTADO_SIGUIENTE == ESTADO_CERRANDO)
	      {
	          ESTADO_SIGUIENTE = Func_ESTADO_CERRANDO();
	      }
	      if(ESTADO_SIGUIENTE == ESTADO_INTERMEDIO)
	      {
	          ESTADO_SIGUIENTE = Func_ESTADO_INTERMEDIO();
	      }
	      if(ESTADO_SIGUIENTE == ESTADO_ERROR)
	      {
	          ESTADO_SIGUIENTE = Func_ESTADO_ERROR();
	      }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 250;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache (default 2-ways set associative cache)
  */
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 80;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_DRD_FS.Instance = USB_DRD_FS;
  hpcd_USB_DRD_FS.Init.dev_endpoints = 8;
  hpcd_USB_DRD_FS.Init.speed = USBD_FS_SPEED;
  hpcd_USB_DRD_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_DRD_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.battery_charging_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.vbus_sensing_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.bulk_doublebuffer_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.iso_singlebuffer_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_DRD_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, USER_LED_Pin|Ma_Pin|Mc_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_FS_PWR_EN_GPIO_Port, USB_FS_PWR_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_BLUE_USER_BUTTON_Pin */
  GPIO_InitStruct.Pin = B1_BLUE_USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_BLUE_USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Sa_Pin Sc_Pin Bc_Pin Ba_Pin */
  GPIO_InitStruct.Pin = Sa_Pin|Sc_Pin|Bc_Pin|Ba_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : T_VCP_RX_Pin */
  GPIO_InitStruct.Pin = T_VCP_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_USART3;
  HAL_GPIO_Init(T_VCP_RX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USER_LED_Pin Ma_Pin Mc_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin|Ma_Pin|Mc_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_PWR_EN_Pin */
  GPIO_InitStruct.Pin = USB_FS_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_FS_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_OVCR_Pin */
  GPIO_InitStruct.Pin = USB_FS_OVCR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_FS_OVCR_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int Func_ESTADO_ERROR(void)
{
    for(;;)
    {
        inout.Led = TRUE;
    }
}
int Func_ESTADO_ABIERTO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABIERTO;
    inout.Ma = FALSE;
    inout.Mc = FALSE;
    Func_ESTADO_SETTINGLED(LED_ON, INTER_RAPIDO);
    for(;;)
    {
        if(inout.Bc == TRUE)
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_CERRADO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRADO;
    inout.Mc = FALSE;
    inout.Ma = FALSE;
    Func_ESTADO_SETTINGLED(LED_OFF, INTER_OFF);
    for(;;)
    {
        if(inout.Ba == TRUE)
        {
            return ESTADO_ABRIENDO;
        }
    }

}
int Func_ESTADO_ABRIENDO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABRIENDO;
    inout.Mc = FALSE;
    inout.Ma = TRUE;
    Func_ESTADO_SETTINGLED(LED_ON, INTER_LENTO);
    for(;;)
    {
        if(inout.Sa == TRUE)
        {
            return ESTADO_ABIERTO;
        }
    }
}
int Func_ESTADO_CERRANDO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRANDO;
    inout.Mc = TRUE;
    inout.Ma = FALSE;
    Func_ESTADO_SETTINGLED(LED_ON, INTER_LENTO);
    for(;;)
    {
        if(inout.Sc == TRUE)
        {
            return ESTADO_CERRADO;
        }
    }
}
int Func_ESTADO_INTERMEDIO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_INTERMEDIO;
    inout.Ma = FALSE;
    inout.Mc = FALSE;
    for(;;)
    {
        if(inout.Ba == TRUE)
        {
            return ESTADO_ABRIENDO;
        }
        if(inout.Bc == TRUE)
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_INIT(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_INIT;
    inout.Ma = FALSE;
    inout.Mc = FALSE;
    for(;;)
    {
        if((inout.Sa == TRUE) && (inout.Sc == TRUE))
        {
            return ESTADO_ERROR;
        }

        if(inout.Sa == TRUE)
        {
            return ESTADO_ABIERTO;
        }

        if(inout.Sc == TRUE)
        {
            return ESTADO_CERRADO;
        }

        if((inout.Sa == FALSE) && (inout.Sc == FALSE))
        {
            return ESTADO_INTERMEDIO;
        }
    }
}

int Func_ESTADO_SETTINGLED(int St, int Inter)
{
	LedStatus.StatusLED = St;       //Estado del Led
	LedStatus.InterLED = Inter;     //Velocidad de Intermitencia
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim2)  //Interrupciones
{

	    if(HAL_GPIO_ReadPin(PUERTO, PIN_Ba) == GPIO_PIN_SET)
	    {
	   	    inout.Ba = TRUE;
	   	}
	   	else
	   	{
	   	    inout.Ba = FALSE;
	   	}

	    if(HAL_GPIO_ReadPin(PUERTO, PIN_Bc) == GPIO_PIN_SET)
	    {
	        inout.Bc = TRUE;
	    }
	    else
	    {
	        inout.Bc = FALSE;
	    }

	    if(HAL_GPIO_ReadPin(PUERTO, PIN_Sc) == GPIO_PIN_SET)
	    {
	        inout.Sc = TRUE;
	    }
	    else
	    {
	        inout.Sc = FALSE;
	    }

	    if(HAL_GPIO_ReadPin(PUERTO, PIN_Sa) == GPIO_PIN_SET)
	    {
	        inout.Sa = TRUE;
	    }
	    else
	    {
	        inout.Sa = FALSE;
	    }

	    if(inout.Ma == TRUE)
	    {
	        HAL_GPIO_WritePin(PUERTO, PIN_Ma, TRUE);
	    }
	    else
	    {
	        HAL_GPIO_WritePin(PUERTO, PIN_Ma, FALSE);
	    }

	    if(inout.Mc == TRUE)
	    {
	        HAL_GPIO_WritePin(PUERTO, PIN_Mc, TRUE);
	    }
	    else
	    {
	        HAL_GPIO_WritePin(PUERTO, PIN_Mc, FALSE);
	    }

	    static unsigned cont_LED = 0;       //Contador Led
	    cont_LED++;

	    if(LedStatus.StatusLED == LED_ON)
	    	{
	    		if(cont_LED >= LedStatus.InterLED)
	    		{
	    			if(HAL_GPIO_ReadPin(PUERTO, PIN_Led) == LED_ON)
	    			{
	    				HAL_GPIO_WritePin(PUERTO, PIN_Led, FALSE);
	    			}
	    			else
	    			{
	    				HAL_GPIO_WritePin(PUERTO, PIN_Led, TRUE);
	    			}
	    			cont_LED = 0;
	    		}
	    	}
	    else
	    {
	    	cont_LED = 0;
	    	HAL_GPIO_WritePin(PUERTO, PIN_Led, FALSE);
	    }
}
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
