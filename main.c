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
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "joy.h"
#include "key.h"
#include "NRF24L01.h"
#include "motor.h"

#include "math.h"
#include "OLED.h"
//#include "MPU6050.h"
#include "NRF24L01.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h"
//#include "dmpKey.h"
//#include "dmpmap.h"
#include "MPU6050.h"

float acc_z;
int16_t S,Out,Out2;
int16_t l,s2,s3,s4,s5;
int16_t ax,ay,az,gx,gy,gz;
uint8_t m1;
uint8_t m2;
extern I2C_HandleTypeDef hi2c1;
//BMP280_HandleTypeDef hbmp280;
uint8_t RunFlag = 0;
uint8_t Key;

uint8_t Run;


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint32_t Duty;
uint32_t capture;
uint8_t  Key;
int16_t m,n,e1,e2,e3,s2,s3,s4,s5;
uint16_t Angle=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
{
	if(htim->Instance == TIM2)
	{
		m++;
		n++;
		s2++;
		s3++;
		s4++;
		s5++;



	}

}


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

 NRF24L01_Init();
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//电机
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1500);
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,900);
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1500);
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1500);
HAL_Delay(2000);
HAL_TIM_Base_Start_IT(&htim2);
Angle = 90;
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Angle * (htim1.Init.Period+1)/1800+(htim1.Init.Period+1)/40);
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,Angle * (htim1.Init.Period+1)/1800+(htim1.Init.Period+1)/40);
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,Angle * (htim1.Init.Period+1)/1800+(htim1.Init.Period+1)/40);
//__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1200);//最大2000

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

if(RunFlag)
	{

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, Out);
	}
if(m>5)
{
	m = 0;


		if(NRF24L01_Receive() == 1)
		{

			Key = NRF24L01_RxPacket[5];
			float e3 = (NRF24L01_RxPacket[2]-100);


			e1 = (NRF24L01_RxPacket[1]-100);
			e2= (NRF24L01_RxPacket[4]-100);
			if((e1<5)&&(e1>-5))
			{
				e1 = 0;
			}
			if((e2<5)&&(e2>-5))
			{
				e2 = 0;
			}

             if((e3<5)&&(e3>-5))
             			{
             				e3 = 0;
             			}
             if(e2 < 0) e2 = 0;

             // ★ 映射 -100~100 到 900~2000
             Out = 900 + e2 * 11;
             Out2 = 1000 + e2 * 10;
             // 限幅保护（防止异常输入）
             if (Out > 2000) Out = 2000;
             if (Out < 900)  Out = 900;
             if (Out2 > 2000) Out2 = 2000;
                          if (Out2 < 900)  Out2 = 900;
             // 输出到电机
             int16_t angle1 = 90 - e1 / 3;             // e1 控制 CH1 舵机
             int16_t angle2 = 90 - e1 / 3;
             int16_t angle3 = 90 - e3 / 2;   // e3 控制 CH3 舵机

             // 限幅保护
             if(angle1 > 120) angle1 = 120;
             if(angle1 < 30)  angle1 = 30;
             if(angle2 > 150) angle2 = 140;
             if(angle2 < 60)  angle2 = 60;
             if(angle3 > 120) angle3 = 120;
             if(angle3 < 60)  angle3 = 60;
             // 输出到舵机 PWM（假设 htim1 的 Period = 19999）
              __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,
                  angle1 * (htim1.Init.Period+1) / 1800 + (htim1.Init.Period+1) / 40);
              __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4,
                  angle2 * (htim1.Init.Period+1) / 1800 + (htim1.Init.Period+1) / 40);
              __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,
                  angle3 * (htim1.Init.Period+1) / 1800 + (htim1.Init.Period+1) / 40);

             // 输出到舵机 PWM（假设 htim1 的 Period = 19999）


      		  if(NRF24L01_RxPacket[0] == 0x00 ||NRF24L01_RxPacket[0] == 0x01)
      		  {
      		  if(NRF24L01_RxPacket[0] == 0x01)
      		  {


      			NRF24L01_TxPacket[0] = 0x02;
//      			*(float *)&NRF24L01_TxPacket[1] = ya.Out;//p_angle;
//      			*(float *)&NRF24L01_TxPacket[5] = ya_r.Out;//r_angle;
//      			*(float *)&NRF24L01_TxPacket[9] = smooth_vel*100;//y_angle;
//      			*(float *)&NRF24L01_TxPacket[13] = kf_height.x[0];

      			NRF24L01_Send();

      		  }

 /*     		  if(Key == 1)
      		  {

      			  HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0);



      		  }*/
      		  if(Key == 2)
      		  {
      			  RunFlag = 1;


      		  }
     		  if(Key == 6)
      		  {


      		  }
           if(Key == 5)
           {
        	   RunFlag = 0;

        	   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,900);
           }
           if(Key == 1)
           {

           }

		}
}

}
  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
