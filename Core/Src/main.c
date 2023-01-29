/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that cman be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "INA219.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GPS_BUF_SIZE 2048
#define LORA_RX_BUF_SIZE 128
#define LORA_TX_BUF_SIZE 128
#define DEVICE_ADDRESS 'A'
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 3000 * 4,
  .priority = (osPriority_t) osPriorityRealtime7,
};
/* Definitions for blinkTask */
osThreadId_t blinkTaskHandle;
const osThreadAttr_t blinkTask_attributes = {
  .name = "blinkTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for gpsTask */
osThreadId_t gpsTaskHandle;
const osThreadAttr_t gpsTask_attributes = {
  .name = "gpsTask",
  .stack_size = 2100 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for keypadScanTask */
osThreadId_t keypadScanTaskHandle;
const osThreadAttr_t keypadScanTask_attributes = {
  .name = "keypadScanTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for INA219Task */
osThreadId_t INA219TaskHandle;
const osThreadAttr_t INA219Task_attributes = {
  .name = "INA219Task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for loraTask */
osThreadId_t loraTaskHandle;
const osThreadAttr_t loraTask_attributes = {
  .name = "loraTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for loraSendTask */
osThreadId_t loraSendTaskHandle;
const osThreadAttr_t loraSendTask_attributes = {
  .name = "loraSendTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for transmitQ */
osMessageQueueId_t transmitQHandle;
const osMessageQueueAttr_t transmitQ_attributes = {
  .name = "transmitQ"
};
/* Definitions for receiveQ */
osMessageQueueId_t receiveQHandle;
const osMessageQueueAttr_t receiveQ_attributes = {
  .name = "receiveQ"
};
/* Definitions for latitudeQ */
osMessageQueueId_t latitudeQHandle;
const osMessageQueueAttr_t latitudeQ_attributes = {
  .name = "latitudeQ"
};
/* Definitions for longitudeQ */
osMessageQueueId_t longitudeQHandle;
const osMessageQueueAttr_t longitudeQ_attributes = {
  .name = "longitudeQ"
};
/* Definitions for testQ */
osMessageQueueId_t testQHandle;
const osMessageQueueAttr_t testQ_attributes = {
  .name = "testQ"
};
/* Definitions for keypadQ */
osMessageQueueId_t keypadQHandle;
const osMessageQueueAttr_t keypadQ_attributes = {
  .name = "keypadQ"
};
/* Definitions for gpsLengthQ */
osMessageQueueId_t gpsLengthQHandle;
const osMessageQueueAttr_t gpsLengthQ_attributes = {
  .name = "gpsLengthQ"
};
/* Definitions for currentQ */
osMessageQueueId_t currentQHandle;
const osMessageQueueAttr_t currentQ_attributes = {
  .name = "currentQ"
};
/* Definitions for voltageQ */
osMessageQueueId_t voltageQHandle;
const osMessageQueueAttr_t voltageQ_attributes = {
  .name = "voltageQ"
};
/* Definitions for rxLengthQ */
osMessageQueueId_t rxLengthQHandle;
const osMessageQueueAttr_t rxLengthQ_attributes = {
  .name = "rxLengthQ"
};
/* USER CODE BEGIN PV */
//SerialPrint
uint8_t buffer[512];
uint16_t bufferLength;

//DEBUGGING
uint16_t rxLength = 0;
uint8_t rxBuffer[128];

//GPS
float latitude = 0.0, longitude = 0.0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM7_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void *argument);
void StartBlinkTask(void *argument);
void StartGPSTask(void *argument);
void StartKeypadScanTask(void *argument);
void StartINATask(void *argument);
void StartLoRaTask(void *argument);
void StartLoraSendTask(void *argument);

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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_CRC_Init();
  MX_TIM7_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  //MX_TouchGFX_Init();
  /* Call PreOsInit function */
  MX_TouchGFX_PreOSInit();
  /* USER CODE BEGIN 2 */
  HAL_Delay(50);
  HAL_TIM_Base_Start_IT(&htim7);
  ILI9341_Init();
  MX_TouchGFX_Init();

  memset(rxBuffer, 0, LORA_RX_BUF_SIZE);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxBuffer, LORA_RX_BUF_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of transmitQ */
  transmitQHandle = osMessageQueueNew (4, sizeof(String_t), &transmitQ_attributes);

  /* creation of receiveQ */
  receiveQHandle = osMessageQueueNew (4, sizeof(String_t), &receiveQ_attributes);

  /* creation of latitudeQ */
  latitudeQHandle = osMessageQueueNew (2, sizeof(float), &latitudeQ_attributes);

  /* creation of longitudeQ */
  longitudeQHandle = osMessageQueueNew (2, sizeof(float), &longitudeQ_attributes);

  /* creation of testQ */
  testQHandle = osMessageQueueNew (1, sizeof(uint16_t), &testQ_attributes);

  /* creation of keypadQ */
  keypadQHandle = osMessageQueueNew (1, sizeof(uint8_t), &keypadQ_attributes);

  /* creation of gpsLengthQ */
  gpsLengthQHandle = osMessageQueueNew (2, sizeof(uint16_t), &gpsLengthQ_attributes);

  /* creation of currentQ */
  currentQHandle = osMessageQueueNew (2, sizeof(uint16_t), &currentQ_attributes);

  /* creation of voltageQ */
  voltageQHandle = osMessageQueueNew (2, sizeof(uint16_t), &voltageQ_attributes);

  /* creation of rxLengthQ */
  rxLengthQHandle = osMessageQueueNew (2, sizeof(uint16_t), &rxLengthQ_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of blinkTask */
  blinkTaskHandle = osThreadNew(StartBlinkTask, NULL, &blinkTask_attributes);

  /* creation of gpsTask */
  gpsTaskHandle = osThreadNew(StartGPSTask, NULL, &gpsTask_attributes);

  /* creation of keypadScanTask */
  keypadScanTaskHandle = osThreadNew(StartKeypadScanTask, NULL, &keypadScanTask_attributes);

  /* creation of INA219Task */
  INA219TaskHandle = osThreadNew(StartINATask, NULL, &INA219Task_attributes);

  /* creation of loraTask */
  loraTaskHandle = osThreadNew(StartLoRaTask, NULL, &loraTask_attributes);

  /* creation of loraSendTask */
  loraSendTaskHandle = osThreadNew(StartLoraSendTask, NULL, &loraSendTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 168 - 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 30000 - 1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SPI1_NSS_Pin|RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED3_Pin|LED2_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, M0_Pin|M1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_Pin|KEYPAD_R2_Pin|KEYPAD_R3_Pin|KEYPAD_R4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : SPI1_NSS_Pin */
  GPIO_InitStruct.Pin = SPI1_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_NSS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RESET_Pin LED_Pin */
  GPIO_InitStruct.Pin = RESET_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DC_Pin */
  GPIO_InitStruct.Pin = DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : T_CS_Pin */
  GPIO_InitStruct.Pin = T_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(T_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3_Pin LED2_Pin LED1_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED2_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : IRQ_Pin */
  GPIO_InitStruct.Pin = IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : M0_Pin M1_Pin KEYPAD_R1_Pin KEYPAD_R2_Pin
                           KEYPAD_R3_Pin KEYPAD_R4_Pin */
  GPIO_InitStruct.Pin = M0_Pin|M1_Pin|KEYPAD_R1_Pin|KEYPAD_R2_Pin
                          |KEYPAD_R3_Pin|KEYPAD_R4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : KEYPAD_C1_Pin KEYPAD_C2_Pin KEYPAD_C3_Pin KEYPAD_C4_Pin */
  GPIO_InitStruct.Pin = KEYPAD_C1_Pin|KEYPAD_C2_Pin|KEYPAD_C3_Pin|KEYPAD_C4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
extern void touchgfxSignalVSync(void);

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	static String_t rxQueueSend;
	if(huart->Instance == USART3){
		//GPSUSARTLength = Size;
		osMessageQueuePut(gpsLengthQHandle, &Size, 0U, 0U);
		osThreadFlagsSet(gpsTaskHandle, 0x01);
	}
	else if(huart->Instance == USART2){
		rxQueueSend.message = rxBuffer;
		rxQueueSend.message[Size] = '\0';
		rxQueueSend.length = Size;
		osMessageQueuePut(receiveQHandle, &rxQueueSend, 0U, 0U);

		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxBuffer, LORA_RX_BUF_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	}
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  MX_TouchGFX_Process();
  /* Infinite loop */
  for(;;)
  {
    osDelay(osWaitForever);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartBlinkTask */
/**
* @brief Function implementing the blinkTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBlinkTask */
void StartBlinkTask(void *argument)
{
  /* USER CODE BEGIN StartBlinkTask */
	uint16_t count = 0;
	uint8_t message[32];
  /* Infinite loop */
  for(;;)
  {
	osMessageQueuePut(testQHandle, &count, 0, 0);
	count++;
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartBlinkTask */
}

/* USER CODE BEGIN Header_StartGPSTask */
/**
* @brief Function implementing the gpsTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGPSTask */
void StartGPSTask(void *argument)
{
  /* USER CODE BEGIN StartGPSTask */
	uint8_t USART3Buffer[GPS_BUF_SIZE + 20];
	uint8_t GPSBuffer[GPS_BUF_SIZE];
	int8_t lat_dd;
	float lat_mm ;
	int8_t lon_dd ;
	float lon_mm;
	float velocity;
	uint16_t gpsLength = 0;

	memset(GPSBuffer, 0, GPS_BUF_SIZE);
	memset(USART3Buffer, 0, GPS_BUF_SIZE);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, USART3Buffer, GPS_BUF_SIZE);
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);

  /* Infinite loop */
  for(;;)
  {
	osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
	if(osMessageQueueGet(gpsLengthQHandle, &gpsLength, 0U, 0U) != osOK) continue;
	uint8_t *ch_in_str;
	uint8_t *cek_valid;

	memcpy((uint8_t*)GPSBuffer, (uint8_t*)USART3Buffer, gpsLength);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, USART3Buffer, GPS_BUF_SIZE);
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);

	if ((ch_in_str = (uint8_t*)strstr((char*)GPSBuffer, "RMC")) != NULL) {
		ch_in_str = (uint8_t*)strchr((char*)++ch_in_str, ',');
		ch_in_str = (uint8_t*)strchr((char*)++ch_in_str, ',');

		if(ch_in_str[1] != 'A');
		//if (strchr((char*)ch_in_str, 'A') == NULL);
		else {
			//Mencari koma
			if ((ch_in_str = (uint8_t*)strchr((char*)++ch_in_str, ',')) != NULL) {

				//Parsing Nilai latitude dengan membagi 100 (integer)
				lat_dd = (short) strtoul((char*)++ch_in_str, NULL, 10) / 100;

				//Mengarah ke nilai mm
				++ch_in_str;
				lat_mm = strtof((char*)++ch_in_str, &ch_in_str);

				if (*(++ch_in_str) == 'S') {
					lat_dd *= -1;
					lat_mm *= -1;
				}

				latitude = lat_dd + (lat_mm / 60);

				//Mencari koma longitude
				++ch_in_str;
				++ch_in_str;
				lon_dd = (short) strtoul((char*)ch_in_str, NULL, 10) / 100;

				//Mengacu pada nilai mm
				++ch_in_str;
				++ch_in_str;
				lon_mm = strtof((char*)++ch_in_str, &ch_in_str);

				//Apabila longitude W
				if (*(++ch_in_str) == 'W') {
					lon_dd *= -1;
					lon_mm *= -1;
				}

				longitude = lon_dd + (lon_mm / 60);

				//Mengacu pada nilai velocity
				++ch_in_str;
				velocity = strtof((char*)++ch_in_str, NULL);
				//printf("GNRMC: latitude: %f\tlongitude: %f\tvelo: %f\r\n", latitude, longitude, velocity);
				//bufferLength = sprintf((char*)buffer, "GNRMC = latitude: %f\tlongitude: %f\tvelo: %f\r\n", latitude, longitude, velocity);
				//CDC_Transmit_FS(buffer, bufferLength);
				osMessageQueuePut(latitudeQHandle, &latitude, 0U, 0U);
				osMessageQueuePut(longitudeQHandle, &longitude, 0U, 0U);
			}
		}
	}

	/*if((ch_in_str = (uint8_t*)strstr((char*)GPSBuffer, "GGA")) != NULL){
		//$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F

		//cek validity
		cek_valid = (uint8_t*)strchr((char*)ch_in_str, ',');
		cek_valid = (uint8_t*)strchr((char*)++cek_valid, ',');
		cek_valid = (uint8_t*)strchr((char*)++cek_valid, ',');

		cek_valid = (uint8_t*)strchr((char*)++cek_valid, ',');
		cek_valid = (uint8_t*)strchr((char*)++cek_valid, ',');
		cek_valid = (uint8_t*)strchr((char*)++cek_valid, ',');
		++cek_valid;


		if(*cek_valid != '1' && *cek_valid != '2' && *cek_valid != '6');
		else {
			ch_in_str = (uint8_t*)strchr((char*)++ch_in_str, ',');
			ch_in_str = (uint8_t*)strchr((char*)++ch_in_str, ',');

			//Parsing Nilai latitude dengan membagi 100 (integer)
			lat_dd = strtoul((char*)++ch_in_str, NULL, 10) / 100;

			//Mengarah ke nilai mm
			++ch_in_str;
			lat_mm = strtof((char*)++ch_in_str, &ch_in_str);

			if (*(++ch_in_str) == 'S') {
				lat_dd *= -1;
				lat_mm *= -1;
			}

			lat_gps = lat_dd + (lat_mm / 60);

			//Mencari koma longitude
			++ch_in_str;
			++ch_in_str;
			lon_dd = strtoul((char*)ch_in_str, NULL, 10) / 100;

			//Mengacu pada nilai mm
			++ch_in_str;
			++ch_in_str;
			lon_mm = strtof((char*)++ch_in_str, &ch_in_str);

			//Apabila longitude W
			if (*(++ch_in_str) == 'W') {
				lon_dd *= -1;
				lon_mm *= -1;
			}

			lon_gps = lon_dd + (lon_mm / 60);

			// skip fix indicator
			ch_in_str = ch_in_str + 3;

			// cari koma dua kali
			ch_in_str = (uint8_t*)strchr((char*)++ch_in_str, ',');
			ch_in_str = (uint8_t*)strchr((char*)++ch_in_str, ',');

			// ambil nilai altitude
			alti = strtof((char*)++ch_in_str, &ch_in_str);
			//printf("GNGGA: latitude: %f\tlongitude: %f\talti: %f\r\n", lat_gps, lon_gps, alti);
		}

	}
	//printf("vel: %f\tlat: %f\tlon: %f\r\n", velocity, latitude, longitude);*/
	memset(GPSBuffer, 0, GPS_BUF_SIZE);
  }
  /* USER CODE END StartGPSTask */
}

/* USER CODE BEGIN Header_StartKeypadScanTask */
/**
* @brief Function implementing the keypadScanTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeypadScanTask */
void StartKeypadScanTask(void *argument)
{
  /* USER CODE BEGIN StartKeypadScanTask */
	uint8_t numberPressed = 11;
	uint8_t prevNumberPressed = 11;
	uint32_t sameKeyTick = 0, prevKeyTick = 0;
	uint16_t RPin[4] = {KEYPAD_R1_Pin, KEYPAD_R2_Pin, KEYPAD_R3_Pin, KEYPAD_R4_Pin};
	uint16_t CPin[4] = {KEYPAD_C1_Pin, KEYPAD_C2_Pin, KEYPAD_C3_Pin, KEYPAD_C4_Pin};
  /* Infinite loop */
  for(;;)
  {
		uint8_t buttonPressed[2] = {255, 255};

		for(int i = 0; i < 4; i++){
			HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_Pin | KEYPAD_R2_Pin | KEYPAD_R3_Pin | KEYPAD_R4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, RPin[i], GPIO_PIN_RESET);

			for(int j = 0; j < 4; j++){
				if(HAL_GPIO_ReadPin(GPIOB, CPin[j]) == GPIO_PIN_RESET){
					buttonPressed[1] = j + 1;
					buttonPressed[0] = i + 1;
					break;
				}
			}
			if(buttonPressed[0] != 255 && buttonPressed[1] != 255) break;
		}

		if(buttonPressed[0] != 255 && buttonPressed[1] != 255){
			prevNumberPressed = numberPressed;
			sameKeyTick = osKernelGetTickCount() - prevKeyTick;
			if(buttonPressed[0] == 4 && buttonPressed[1] == 4) numberPressed = 1;
			else if(buttonPressed[0] == 3 && buttonPressed[1] == 4) numberPressed = 2;
			else if(buttonPressed[0] == 2 && buttonPressed[1] == 4) numberPressed = 3;
			else if(buttonPressed[0] == 1 && buttonPressed[1] == 4) numberPressed = 'A';
			else if(buttonPressed[0] == 4 && buttonPressed[1] == 3) numberPressed = 4;
			else if(buttonPressed[0] == 3 && buttonPressed[1] == 3) numberPressed = 5;
			else if(buttonPressed[0] == 2 && buttonPressed[1] == 3) numberPressed = 6;
			else if(buttonPressed[0] == 1 && buttonPressed[1] == 3) numberPressed = 'B';
			else if(buttonPressed[0] == 4 && buttonPressed[1] == 2) numberPressed = 7;
			else if(buttonPressed[0] == 3 && buttonPressed[1] == 2) numberPressed = 8;
			else if(buttonPressed[0] == 2 && buttonPressed[1] == 2) numberPressed = 9;
			else if(buttonPressed[0] == 1 && buttonPressed[1] == 2) numberPressed = 'C';
			else if(buttonPressed[0] == 4 && buttonPressed[1] == 1) numberPressed = 'X';
			else if(buttonPressed[0] == 3 && buttonPressed[1] == 1) numberPressed = 0;
			else if(buttonPressed[0] == 2 && buttonPressed[1] == 1) numberPressed = 'Y';
			else if(buttonPressed[0] == 1 && buttonPressed[1] == 1) numberPressed = 'D';
			prevKeyTick = osKernelGetTickCount();

			if(prevNumberPressed == numberPressed && sameKeyTick <= 100);
			else {
				osMessageQueuePut(keypadQHandle, &numberPressed, 0U, 0U);
				if(numberPressed == 'Y') osDelay(1000);
			}
		}

		osDelay(10);
  }
  /* USER CODE END StartKeypadScanTask */
}

/* USER CODE BEGIN Header_StartINATask */
/**
* @brief Function implementing the INA219Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartINATask */
void StartINATask(void *argument)
{
  /* USER CODE BEGIN StartINATask */
  INA219_t ina219;
  uint16_t vbus, vshunt, current;

  while(!INA219_Init(&ina219, &hi2c1, INA219_ADDRESS)){
	  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
	  osDelay(2000);
  }
  /* Infinite loop */
  for(;;)
  {
	vbus = INA219_ReadBusVoltage(&ina219);
	vshunt = INA219_ReadShuntVolage(&ina219);
	current = INA219_ReadCurrent(&ina219);
	osMessageQueuePut(currentQHandle, &current, 0U, 0U);
	osMessageQueuePut(voltageQHandle, &vbus, 0U, 0U);
    osDelay(1000);
  }
  /* USER CODE END StartINATask */
}

/* USER CODE BEGIN Header_StartLoRaTask */
/**
* @brief Function implementing the loraTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLoRaTask */
void StartLoRaTask(void *argument)
{
  /* USER CODE BEGIN StartLoRaTask */
	//uint8_t txBuffer[LORA_TX_BUF_SIZE];
  /* Infinite loop */
  for(;;)
  {
	  osDelay(osWaitForever);
//	osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
//
//	if(osMessageQueueGet(rxLengthQHandle, &rxLength, 0U, 0U) != osOK) continue;
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxBuffer, LORA_RX_BUF_SIZE);
//	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
//	for(int i = 0; i < rxLength; i++) rxQueueSend.message[i] = rxBuffer[i];
//	rxQueueSend.message[rxLength] = 0;
//	rxQueueSend.length = rxLength;
//	osMessageQueuePut(receiveQHandle, &rxQueueSend, 0U, 0U);

  }
  /* USER CODE END StartLoRaTask */
}

/* USER CODE BEGIN Header_StartLoraSendTask */
/**
* @brief Function implementing the loraSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLoraSendTask */
void StartLoraSendTask(void *argument)
{
  /* USER CODE BEGIN StartLoraSendTask */
	uint8_t message[128];
	String_t sendMessageString;
	uint16_t messageLength = 0;
	uint32_t idMessage = 0;

  /* Infinite loop */
	for (;;) {
		if(osMessageQueueGet(transmitQHandle, &sendMessageString, 0U, 10) == osOK){
			for(int i = 0; i < sendMessageString.length; i++) idMessage += sendMessageString.message[i];
			idMessage += (DEVICE_ADDRESS + (TIM10->CNT * sendMessageString.length));
			messageLength = sprintf((char*)message, "%c,%f,%f,%s,%lu", DEVICE_ADDRESS, latitude, longitude, sendMessageString.message, idMessage);
			HAL_UART_Transmit(&huart2, message, messageLength, 100);
		}

/*		uint8_t lengthMessage = 0;
		osStatus_t statusTransmitQ;
		message[0] = 0;
		do {
			statusTransmitQ = osMessageQueueGet(transmitQHandle, &message[lengthMessage], 0U, 0U);
			lengthMessage++;
		} while (statusTransmitQ != osOK);

		if(message[0] != 0){
			for(int i = 0; i < lengthMessage; i++){
				osMessageQueuePut(receiveQHandle, &message[i], 0U, 0U);
			}
		}

		osDelay(20);*/

//		uint16_t lengthMessage = 0;
//
//		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
//		lengthMessage = sprintf((char*)message, "Hello: %lu\r\n", osKernelGetTickCount());
//		HAL_UART_Transmit(&huart2, message, lengthMessage, HAL_MAX_DELAY);
//		osDelay(2000);
	}
  /* USER CODE END StartLoraSendTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM10 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM10) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  else if(htim->Instance == TIM7){
	  touchgfxSignalVSync();
		//bufferLength = sprintf((char*)buffer, "Hello World!\r\n");
		//CDC_Transmit_FS(buffer, bufferLength);
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
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
