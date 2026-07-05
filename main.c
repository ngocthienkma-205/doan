
	/* USER CODE BEGIN Header */
	/**
		******************************************************************************
		* @file           : main.c
		* @brief          : Main program body
		******************************************************************************
		* @attention
		*
		* Copyright (c) 2026 STMicroelectronics.
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
	#include "tcs34725.h"
	#include "i2c_lcd.h"
	#include "stdio.h"
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
	I2C_HandleTypeDef hi2c1;
	I2C_HandleTypeDef hi2c2;

	TIM_HandleTypeDef htim2;

	/* USER CODE BEGIN PV */
	int red, green, blue;

	char lcd_buffer[20];
	/* USER CODE END PV */

	/* Private function prototypes -----------------------------------------------*/
	void SystemClock_Config(void);
	static void MX_GPIO_Init(void);
	static void MX_I2C1_Init(void);
	static void MX_I2C2_Init(void);
	static void MX_TIM2_Init(void);
	/* USER CODE BEGIN PFP */

	/* USER CODE END PFP */

	/* Private user code ---------------------------------------------------------*/
	/* USER CODE BEGIN 0 */

	#define RED     1
	#define GREEN   2
	#define BLUE    3

	uint8_t currentColor = 0;
	uint8_t statetcs = 0;

	uint16_t redCount = 0;
	uint16_t greenCount = 0;
	uint16_t blueCount = 0;

	void Servo_Init(void)
	{
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

	}

	void Servo_Write(uint8_t channel, uint8_t goc)
	{
		
		uint16_t pulse = ((2000 * goc) / 180) + 500;

		switch(channel){
		
			case 1:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
			break;
			
			case 2:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
			break;
			
			case 3:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pulse);
			break;

		}

	}

	/* servo gat*/
	void Servo_GatR(uint8_t channel)
	{
		Servo_Write(channel, 0);
	}

	/*servo quay ve vi tri ban dau*/
	void Servo_veR(uint8_t channel)
	{
		Servo_Write(channel, 90);
	}

	void Servo_gatG(uint8_t channel)
	{
		Servo_Write(channel, 90);
	}
	
	void Servo_veG(uint8_t channel)
	{
		Servo_Write(channel, 0);
	}

	void LED_OFF(void)
	{
		HAL_GPIO_WritePin(GPIOA,
				RLED_Pin | GLED_Pin | BLED_Pin,
				GPIO_PIN_RESET);
	}

	void LED_RED(void)
	{
		LED_OFF();
		HAL_GPIO_WritePin(GPIOA, RLED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, BLED_Pin | GLED_Pin, GPIO_PIN_RESET);
		
	}

	void LED_GREEN(void)
	{
		LED_OFF();
		HAL_GPIO_WritePin(GPIOA, GLED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, BLED_Pin | RLED_Pin, GPIO_PIN_RESET);
	}

	void LED_BLUE(void)
	{
		LED_OFF();
		HAL_GPIO_WritePin(GPIOA, BLED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GLED_Pin | RLED_Pin, GPIO_PIN_RESET);
	}

	void LCD_Update(void)
	{
			sprintf(lcd_buffer, "R:%d  G:%d       ",
							redCount,
							greenCount);

			lcd_gotoxy(0,0);
			lcd_send_string(lcd_buffer);

			sprintf(lcd_buffer, "B:%d      ",
							blueCount);

			lcd_gotoxy(1,0);
			lcd_send_string(lcd_buffer);
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
		MX_I2C1_Init();
		MX_I2C2_Init();
		MX_TIM2_Init();
		/* USER CODE BEGIN 2 */

		Servo_Init();
		
		Servo_Write(1, 0);
		Servo_Write(2, 0);
		Servo_Write(3, 0); 
		
		LED_OFF();
		lcd_init();
		TCS34725_Init(&hi2c1);

		/* USER CODE END 2 */

		/* Infinite loop */
		/* USER CODE BEGIN WHILE */
		while (1)
		{
			/* USER CODE END WHILE */

			/* USER CODE BEGIN 3 */

		/* 	   DOC CAM BIEN MAU  */
			
	/* luu trang thai tcs*/
			
		if( statetcs == 0){
			Servo_veR(1);
			Servo_veG(2);
			
			TCS34725_GetRGB(&red , &green, &blue);

				if(red == 0 && green == 0 && blue == 0)
				{
				currentColor = 0;
				statetcs = 0;
				LED_OFF();
				}
			
			/* RED */
			else if(red > green  && red > blue )
			{
				currentColor = RED;
				statetcs = 1;
				LED_RED();
			}

			/* GREEN */
			else if(green > red  && green > blue )
			{
				currentColor = GREEN;
				statetcs = 1;
				LED_GREEN();
			}

			/* BLUE */
			else if(blue > red && blue > green)
			{
				currentColor = BLUE;
				statetcs = 1;
				LED_BLUE();
			}
	}
			/* IR + SERVO */

		if(statetcs == 1){
		/* RED */
		if(currentColor == RED){
			if(HAL_GPIO_ReadPin(GPIOA, IR_RED_Pin) == GPIO_PIN_RESET)
		{		
				Servo_GatR(1);
				redCount++;
				HAL_Delay(5000);
			
				currentColor = 0;
				statetcs = 0;
				LED_OFF();
			}
		}
		/* GREEN */
		else if(currentColor == GREEN){
		 if(HAL_GPIO_ReadPin(GPIOA, IR_GREEN_Pin) == GPIO_PIN_RESET)
		{

				Servo_gatG(2);
				greenCount++;
				HAL_Delay(5000);
			
				currentColor = 0;
				statetcs = 0;
				LED_OFF();
			}
		}

		/* BLUE */
		
		else if(currentColor == BLUE){
				blueCount++;
				HAL_Delay(6000);
				statetcs = 0;
				currentColor = 0;
				LED_OFF();
		}
		}
		HAL_Delay(50);  
		LCD_Update();
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

		/** Initializes the RCC Oscillators according to the specified parameters
		* in the RCC_OscInitTypeDef structure.
		*/
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
		RCC_OscInitStruct.HSIState = RCC_HSI_ON;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
		RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
		RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

		if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
		{
			Error_Handler();
		}
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
		hi2c1.Init.ClockSpeed = 400000;
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
		hi2c2.Init.ClockSpeed = 100000;
		hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
		hi2c2.Init.OwnAddress1 = 0;
		hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		hi2c2.Init.OwnAddress2 = 0;
		hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		if (HAL_I2C_Init(&hi2c2) != HAL_OK)
		{
			Error_Handler();
		}
		/* USER CODE BEGIN I2C2_Init 2 */

		/* USER CODE END I2C2_Init 2 */

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
		TIM_OC_InitTypeDef sConfigOC = {0};

		/* USER CODE BEGIN TIM2_Init 1 */

		/* USER CODE END TIM2_Init 1 */
		htim2.Instance = TIM2;
		htim2.Init.Prescaler = 71;
		htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim2.Init.Period = 19999;
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
		if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
		{
			Error_Handler();
		}
		if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
		{
			Error_Handler();
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
		{
			Error_Handler();
		}
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = 0;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		{
			Error_Handler();
		}
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
		{
			Error_Handler();
		}
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
		{
			Error_Handler();
		}
		/* USER CODE BEGIN TIM2_Init 2 */

		/* USER CODE END TIM2_Init 2 */
		HAL_TIM_MspPostInit(&htim2);

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
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOA, RLED_Pin|GLED_Pin|BLED_Pin, GPIO_PIN_RESET);

		/*Configure GPIO pins : RLED_Pin GLED_Pin BLED_Pin */
		GPIO_InitStruct.Pin = RLED_Pin|GLED_Pin|BLED_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/*Configure GPIO pins : IR_RED_Pin IR_GREEN_Pin IR_BLUE_Pin */
		GPIO_InitStruct.Pin = IR_RED_Pin|IR_GREEN_Pin|IR_BLUE_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

