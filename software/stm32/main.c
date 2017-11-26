/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "utick.h"
#include "i2l_slave.h"

#define AM_ADDR 0xB8
//#define AM_ADDR 0x5C
#define I2C_TIMEOUT 500
#define _usec(x) x/10


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t am2320_f3_answ[8];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
uint8_t scl_high(void){
  GPIO_PinState  scl;
  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
  scl = HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin);
  return scl;
}
uint8_t sda_high(void){
  GPIO_PinState  sda;
  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
  sda = HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin);
  return sda;
}
uint8_t scl_low(void){
  GPIO_PinState  scl;
  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
  scl = HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin);
  return scl;
}

uint8_t sda_low(void){
  GPIO_PinState  sda;
  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
  sda = HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin);
  return sda;
}

uint8_t sda_read(void){
  GPIO_PinState  sda;
  sda = HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin);
  return sda;
}

uint8_t am2320_wakeup(void){
  uint8_t ack;

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /* i2c init */
  sda_high();
  scl_high();
  usleep(_usec(2000000));
  /* start:*/ 
  sda_low();
  usleep(_usec(100));
  scl_low();
  usleep(_usec(100));
  
  /* address: */   
  for (int i = 7; i >= 0; i --){
    if (AM_ADDR & (1 << i)){
      sda_high();
    }else{
      sda_low();
    }      
    usleep(_usec(100));
    scl_high();
    usleep(_usec(100));
    scl_low();
  }    
  /* ack */
  sda_high();
  usleep(_usec(100));
  scl_high();
  usleep(_usec(100));
  ack = sda_read();  
  scl_low();
  sda_low();
  usleep(_usec(1000)); // min 800us!!!
  scl_high();
  usleep(_usec(100));
  sda_high();
  usleep(_usec(100)); // wait for waking up
  return 0; 
}

uint8_t am2320_address(uint8_t rd_bit){
  uint8_t ack;
  uint8_t addr = AM_ADDR | rd_bit;
  /* start:*/ 
  sda_low();
  usleep(_usec(100));
  scl_low();
  usleep(_usec(100));
  
  /* address: */   
  for (int i = 7; i >= 0; i --){
    if (addr & (1 << i)){
      sda_high();
    }else{
      sda_low();
    }      
    usleep(_usec(100));
    scl_high();
    usleep(_usec(100));
    scl_low();
  }    
  /* ack */
  sda_high();
  usleep(_usec(100));
  scl_high();
  usleep(_usec(100));
  ack = sda_read();
  if (ack == 0){
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//    sda_low();
 }else{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }
  scl_low();
//  usleep(_usec(1000));
//  sda_high();
  usleep(_usec(100));
  return ack;
}

uint8_t am2320_command(uint8_t cmd){
  uint8_t ack;

  for (int i = 7; i >= 0; i --){
    if (cmd & (1 << i)){
      sda_high();
    }else{
      sda_low();
    }      
    usleep(_usec(100));
    scl_high();
    usleep(_usec(100));
    scl_low();
  }    
  /* ack */
  sda_high();
  usleep(_usec(100));
  scl_high();
  usleep(_usec(100));
  ack = sda_read();
  if (ack == 0){
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    sda_low();
  }else{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }
  scl_low();
//  usleep(_usec(1000));
//  sda_high();
  usleep(_usec(100));
  return ack;
}

uint8_t am2320_answer(uint8_t last_byte){
  uint8_t ack = 0;
  uint8_t answer = 0;
  /* SDA is already HIGH */
  for (int i = 7; i >= 0; i --){
    scl_high();
    usleep(_usec(100));
    ack = sda_read();
    answer |= ((ack & 0x1) << i);
    scl_low();
    usleep(_usec(100));
  }    
  if (!last_byte)
    sda_low(); //master ACK
  scl_high();
  usleep(_usec(100));
#if 0  
  ack = sda_read();
  if (ack == 0){
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//    sda_low();
  }else{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }
#endif  
  scl_low();
//  usleep(_usec(1000));
  if (!last_byte){
    sda_high();
  } 
  usleep(_usec(100));
  return answer;
}

uint8_t sda_stop(void){
  scl_high();
  usleep(_usec(100));
  sda_high();
  usleep(_usec(100));
  return 0;
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint16_t crc16(uint8_t *ptr, uint8_t len){
  uint16_t crc =0xFFFF;
  uint8_t i;
  while(len--){
    crc ^=*ptr++;
    for(i=0;i<8;i++){
      if(crc & 0x01){
        crc >>= 1;
        crc ^= 0xA001;
      }else{
        crc>>=1;
      }
    }
  }
  return crc;
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  HAL_StatusTypeDef rc;
  //uint8_t cmd;
  //uint8_t val;
  //uint8_t ack;
  //uint8_t hum_hi, hum_lo, temp_hi, temp_lo, fun_num, answ_len, crc_code1, crc_code2;
  uint16_t crc;
  int8_t tr_buffer[24];
  int tr_size; 
  //GPIO_PinState  pb5;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  reset_utick();
  i2l_init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
  rc = HAL_TIM_Base_Start_IT(&htim2);

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  while (1) {
#if 0
    while (i2l_get_state()!= e_i2l_start){
      /* wait for master */
    }
#endif    

    /* am2320 wakeup:  */
    am2320_wakeup();
  
    /* workong: */
    am2320_address(0x00); // WR
    am2320_command(0x03); // function code
    am2320_command(0x00); // staring address 
    am2320_command(0x04); // register length
    sda_stop();
    usleep(_usec(2000)); //wait for collecting data min 1.5ms
    am2320_address(0x01); // RD
    am2320_f3_answ[0] = am2320_answer(0);
    am2320_f3_answ[1] = am2320_answer(0);
    am2320_f3_answ[2] = am2320_answer(0);
    am2320_f3_answ[3] = am2320_answer(0);
    am2320_f3_answ[4] = am2320_answer(0);
    am2320_f3_answ[5] = am2320_answer(0);
    am2320_f3_answ[6] = am2320_answer(0);
    am2320_f3_answ[7] = am2320_answer(1);
    sda_stop();
    //usleep(_usec(6000));
    
    i2l_set_ready(); /* set SDA low and wait master for data read */
#if 0
    while (i2l_get_state()!= e_i2l_stop){
      /* wait for master done read data */
    }
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    crc = crc16(am2320_f3_answ, 6);

    if ((am2320_f3_answ[6] == (crc & 0xFF)) && (am2320_f3_answ[7] == ((crc >> 8) & 0xFF))){
      tr_size = sprintf((char*)tr_buffer, "hum:%.2d temp:%.2d\r\n", 
                         (am2320_f3_answ[2]*256 + am2320_f3_answ[3])/10, 
                         (am2320_f3_answ[4]*256 + am2320_f3_answ[5])/10);
      rc = HAL_UART_Transmit(&huart3,  (uint8_t*)tr_buffer, tr_size, 100);
    }  
    while (1){
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
      
      //HAL_UART_Receive(&huart3, (uint8_t*)tr_buffer, tr_size, 10000);
      usleep(_usec(300000));
    }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
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
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 719;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, I2L_SDA_Pin|SCL_Pin|SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : I2L_SDA_Pin SCL_Pin SDA_Pin */
  GPIO_InitStruct.Pin = I2L_SDA_Pin|SCL_Pin|SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : I2L_SCL_Pin */
  GPIO_InitStruct.Pin = I2L_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(I2L_SCL_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
