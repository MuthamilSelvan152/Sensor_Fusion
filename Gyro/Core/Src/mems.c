/**
  ******************************************************************************
  * @file    BSP/Src/mems.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use MEMS features.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mems.h"

#include "RCFilter.h"
#include "FIRFilter.h"
#include "IIRFilter.h"

/** @addtogroup BSP_Examples
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t UserPressButton;
extern RCFilter Acc_RC_LPF;
extern FIRFilter Acc_FIR_LPF;
extern IIRFilter Acc_IIR_LPF;;
/* Init af threahold to detect acceleration on MEMS */
int16_t ThresholdHigh = 1000;
int16_t ThresholdLow = -1000;
/* Private function prototypes -----------------------------------------------*/
static void ACCELERO_ReadAcc(void);
static void GYRO_ReadAng(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test ACCELERATOR MEMS Hardware.
  *   The main objective of this test is to check acceleration on 2 axis X and Y
  * @param  None
  * @retval None
  */
void ACCELERO_MEMS_Test(void)
  {
  /* Init Accelerometer Mems */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  UserPressButton = 0;
  while(!UserPressButton)
  {
    ACCELERO_ReadAcc();
  }
}  

static void ACCELERO_ReadAcc(void)
{
  int16_t buffer[3] = {0};
  int16_t xval, yval = 0x00;
  
  /* Read Acceleration*/
  BSP_ACCELERO_GetXYZ(buffer);
 
  /* Update autoreload and capture compare registers value*/
  xval = buffer[0];
  yval = buffer[1];

  //printf("x: %6d \t y: %6d \t z: %6d \r\n",buffer[0],buffer[1],buffer[2]);
  //printf("x: %.3f m/s^2 \t y: %.3f m/s^2 \t z: %.3f m/s^2 \r\n",
		  //0.061035*buffer[0]*9.81/1000, 0.061035*buffer[1]*9.81/1000, 0.061035*buffer[2]*9.81/1000);

 // RCFilter_Update(&Acc_RC_LPF,xval);
 // FIRFilter_Update(&Acc_FIR_LPF, xval);
  IIRFilter_Update(&Acc_IIR_LPF, xval);
  //printf(" %.3f, %.3f , %.3f \r\n",
  		 // 0.061035*buffer[0]*9.81/1000, 0.061035*buffer[1]*9.81/1000, 0.061035*buffer[2]*9.81/1000);

  printf(" %.3f, %.3f\r\n",
    		  0.061035*buffer[0]*9.81/1000, 0.061035*Acc_IIR_LPF.out*9.81/1000);


  //HAL_Delay(500);

  if((ABS(xval))>(ABS(yval)))
  {
    if(xval > ThresholdHigh)
    { 
      /* LED10 On */
      BSP_LED_On(LED10);
      HAL_Delay(10);
    }
    else if(xval < ThresholdLow)
    { 
      /* LED3 On */
      BSP_LED_On(LED3);
      HAL_Delay(10);
    }
    else
    { 
      HAL_Delay(10);
    }
  }
  else
  {
    if(yval < ThresholdLow)
    {
      /* LED6 On */
      BSP_LED_On(LED6);
      HAL_Delay(10);
    }
    else if(yval > ThresholdHigh)
    {
      /* LED7 On */
      BSP_LED_On(LED7);
      HAL_Delay(10);
    } 
    else
  {	    
      HAL_Delay(10);
    }
  } 
  
     BSP_LED_Off(LED3);
     BSP_LED_Off(LED6);
     BSP_LED_Off(LED7);
     BSP_LED_Off(LED4);
     BSP_LED_Off(LED10);
     BSP_LED_Off(LED8);
     BSP_LED_Off(LED9);
     BSP_LED_Off(LED5);
}

/**
  * @brief Test GYROSCOPE MEMS Hardware.
  *   The main objectif of this test is to check the hardware connection of the 
  *   MEMS peripheral.
  * @param None
  * @retval None
  */
void GYRO_MEMS_Test(void)
{
  /* Init Accelerometer Mems */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  UserPressButton = 0;
  while(!UserPressButton)
  {
    GYRO_ReadAng();
  }
}  

static void GYRO_ReadAng(void)
{
  /* Gyroscope variable */
  float Buffer[3];
  float Xval,Yval,Zval = 0x00;

  /* Init Gyroscope Mems */
  if(BSP_GYRO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }

  /* Read Gyro Angular data */
  BSP_GYRO_GetXYZ(Buffer);
     
  /* Update autoreload and capture compare registers value*/
  Xval = Buffer[0];
  Yval = Buffer[1];
  Zval = Buffer[2];
  /* dps */
  printf("%.3f, %.3f, %.3f \r\n",
		  L3GD20_SENSITIVITY_250DPS*Xval/1000, L3GD20_SENSITIVITY_250DPS*Yval/1000,
		  L3GD20_SENSITIVITY_250DPS*Zval/1000);
  HAL_Delay(20);
      
  if(Xval>Yval)
   {
    if(Buffer[0] > 5000.0f)
     { 
        /* LD10 On */
        BSP_LED_On(LED10);
        HAL_Delay(10);
     }
     else if(Buffer[0] < -5000.0f)
     { 
        /* LED3 On */
        BSP_LED_On(LED3);
        HAL_Delay(10);
     }      
    else
    { 
      HAL_Delay(10);
    }
   }
  else
   {
    if(Buffer[1] < -5000.0f)
     {
        /* LD6 on */
        BSP_LED_On(LED6);           
        HAL_Delay(10);
     }
    else if(Buffer[1] > 5000.0f)
     {
        /* LD7 On */
        BSP_LED_On(LED7);        
	HAL_Delay(10);
     }     
        else
        { 
            HAL_Delay(10);
        }  	
      } 
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED6);
    BSP_LED_Off(LED7);
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED10);
    BSP_LED_Off(LED8);
    BSP_LED_Off(LED9);
    BSP_LED_Off(LED5);
}
/**
  * @}
  */ 


/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
