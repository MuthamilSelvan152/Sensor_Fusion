/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "RCFilter.h"
#include "FIRFilter.h"
#include "IIRFilter.h"
#include <math.h>

#include "FirstOrderIIRFilter.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SAMPLE_TIME_FILTER_MS	10
#define SIN_FREQ_HZ				1.59f

#define IIR_ALPHA	1.0f
#define IIR_BETA	0.9f

#define FO_IIR_ALPHA	0.9f
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

Accelerometer myAcc = {0};
Gyroscope myGyro = {0};
Complementary_Filter Comp_Filter = {0};

uint32_t sampleTime = 0;

uint8_t DemoIndex = 0;
BSP_DemoTypedef  BSP_examples[]={
  {ACCELERO_MEMS_Test, "LSM303DLHC/LSM303AGR", 0},
  {GYRO_MEMS_Test, "L3GD20/I3G4250D", 1},
};

__IO uint8_t UserPressButton = 0;

float filterInput = 0;
uint32_t timerFILTER_MS = 0;
/* Counter for User button presses*/
__IO uint32_t PressCount = 0;

RCFilter Acc_RC_LPF;
RCFilter Gyro_RC_LPF;

FIRFilter Acc_FIR_LPF;
IIRFilter Acc_IIR_LPF;
FirstOrderIIR Acc_FO_IIR[3];
FirstOrderIIR Gyro_FO_IIR[3];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void IIR_Check();
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  IIRFilter_Init(&Acc_IIR_LPF, IIR_ALPHA, IIR_BETA);

  Comp_Filter.alpha = 0.05f;

  for(uint8_t i = 0; i < 3; i++)
  {
	  FirstOrderIIR_Init(&Acc_FO_IIR[i], FO_IIR_ALPHA);
	  FirstOrderIIR_Init(&Gyro_FO_IIR[i], FO_IIR_ALPHA);
  }

  /* Init Accelerometer Mems */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
	/* Initialization Error */
	Error_Handler();
  }

  /* Init Gyroscope Mems */
  if(BSP_GYRO_Init() != HAL_OK)
  {
	/* Initialization Error */
	Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  /* Read Acceleration*/
	  BSP_ACCELERO_GetXYZ(myAcc.buffer);

	  /* Init Gyroscope Mems */
	  if(BSP_GYRO_Init() != HAL_OK)
	  {
		/* Initialization Error */
		Error_Handler();
	  }

	  /* Read Gyro Angular data */
	  BSP_GYRO_GetXYZ(myGyro.buffer);

	  /* Apply LPF */
	  for(uint8_t i=0; i<3; i++)
	   {
	 	  FirstOrderIIR_Update(&Acc_FO_IIR[i], myAcc.buffer[i]);
	 	  myAcc.Filt[i] = 0.061035f*Acc_FO_IIR[i].out*G/1000.0f;

	 	  FirstOrderIIR_Update(&Gyro_FO_IIR[i], myGyro.buffer[i]);
	 	  myGyro.Filt[i] = DEG_TO_RAD * L3GD20_SENSITIVITY_250DPS*Gyro_FO_IIR[i].out/1000;
	   }

	  if((HAL_GetTick() - sampleTime) >= SAMPLE_TIME)
	  {


		  /* Pitch Ang = asin(Ax / g) */
		  myAcc.pitch_Theta = asinf(myAcc.Filt[x] / G) ; //* RAD_TO_DEG;

		  /* Roll Ang = atan(Ay / Az) */
		  myAcc.roll_Phi = atanf(myAcc.Filt[y] / myAcc.Filt[z]);  //* RAD_TO_DEG;

		  //printf(" %.3f, %.3f, %.3f", acc[0], acc[1], acc[2]);
		  //printf(" %.3f, %.3f\r\n", myAcc.pitch_Theta, myAcc.roll_Phi);

		  /* Body rates to Euler Rates */
		  myGyro.roll_Phi_dot = myGyro.Filt[x] + tanf(Comp_Filter.pitch_Theta) *
				  	  	  	  	  (sinf(Comp_Filter.roll_Phi) * myGyro.Filt[y] +
				  	  	  	  			  cosf(Comp_Filter.roll_Phi) * myGyro.Filt[z]);

		  myGyro.pitch_Theta_dot =	cosf(Comp_Filter.roll_Phi) * myGyro.Filt[y] - sinf(Comp_Filter.roll_Phi) * myGyro.Filt[z];

		  //myGyro.pitch_Theta += (SAMPLE_TIME / 1000.0f) * myGyro.pitch_Theta_dot;
		  //myGyro.roll_Phi += (SAMPLE_TIME / 1000.0f) * myGyro.roll_Phi_dot;

		  //printf(" %.3f, %.3f\r\n", myGyro.pitch_Theta, myGyro.roll_Phi);

		  Comp_Filter.pitch_Theta = Comp_Filter.alpha * myAcc.pitch_Theta
				  	  	  	  	  	+ (1.0f - Comp_Filter.alpha) *
									(Comp_Filter.pitch_Theta + (SAMPLE_TIME / 1000.0f) * myGyro.pitch_Theta_dot); // * RAD_TO_DEG);

		  Comp_Filter.roll_Phi = Comp_Filter.alpha * myAcc.roll_Phi
		  				  	  	  	+ (1.0f - Comp_Filter.alpha) *
		  							(Comp_Filter.roll_Phi + (SAMPLE_TIME / 1000.0f) * myGyro.roll_Phi_dot); // * RAD_TO_DEG);

		  printf(" %.3f, %.3f\r\n", Comp_Filter.pitch_Theta * RAD_TO_DEG, Comp_Filter.roll_Phi * RAD_TO_DEG);

		  sampleTime = HAL_GetTick();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (USER_BUTTON_PIN == GPIO_Pin)
  {
    while (BSP_PB_GetState(BUTTON_USER) != RESET);
    UserPressButton = 1;
  }
}

/**
  * @brief Toggle Leds
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
    BSP_LED_Toggle(LED4);
    HAL_Delay(100);
    BSP_LED_Toggle(LED6);
    HAL_Delay(100);
    BSP_LED_Toggle(LED8);
    HAL_Delay(100);
    BSP_LED_Toggle(LED10);
    HAL_Delay(100);
    BSP_LED_Toggle(LED9);
    HAL_Delay(100);
    BSP_LED_Toggle(LED7);
    HAL_Delay(100);
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
}

/* IIR Filter Check */
void IIR_Check()
{
	if((HAL_GetTick() - timerFILTER_MS) >= SAMPLE_TIME_FILTER_MS)
	{
		filterInput = 10.0f * sinf(0.0062831853f * SIN_FREQ_HZ * timerFILTER_MS);

		IIRFilter_Update(&Acc_IIR_LPF, filterInput);

		printf(" %.3f, %.3f\r\n",
				filterInput, Acc_IIR_LPF.out);

		timerFILTER_MS += SAMPLE_TIME_FILTER_MS;

	}
}

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart1 , (uint8_t *)&ch , 1 , 0xFFFF);
	return ch;
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
    /* Turn LED10/3 (RED) on */
    BSP_LED_On(LED10);
    BSP_LED_On(LED3);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
