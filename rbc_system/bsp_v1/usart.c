/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "usart.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef hlpuart1;
IRDA_HandleTypeDef hirda2;

/* LPUART1 init function */

void MX_LPUART1_UART_Init(void)
{

  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 209700;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_7B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* USART2 init function */

void MX_USART2_IRDA_Init(void)
{

  hirda2.Instance = USART2;
  hirda2.Init.BaudRate = 115200;
  hirda2.Init.WordLength = IRDA_WORDLENGTH_8B;
  hirda2.Init.Parity = IRDA_PARITY_NONE;
  hirda2.Init.Mode = IRDA_MODE_TX_RX;
  hirda2.Init.Prescaler = 10;
  hirda2.Init.PowerMode = IRDA_POWERMODE_NORMAL;
  if (HAL_IRDA_Init(&hirda2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspInit 0 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
  /* USER CODE END LPUART1_MspInit 0 */
    /* LPUART1 clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();
  
    /**LPUART1 GPIO Configuration    
    PB6     ------> LPUART1_TX
    PB7     ------> LPUART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_LPUART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN LPUART1_MspInit 1 */
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(LPUART1_IRQn);				//Ê¹ÄÜUSART1ÖÐ¶ÏÍ¨µÀ
		HAL_NVIC_SetPriority(LPUART1_IRQn,5,0);			//ÇÀÕ¼ÓÅÏÈ¼¶3£¬×ÓÓÅÏÈ¼¶3
#endif	
  /* USER CODE END LPUART1_MspInit 1 */
  }
}

void HAL_IRDA_MspInit(IRDA_HandleTypeDef* irdaHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(irdaHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */
    __HAL_RCC_GPIOA_CLK_ENABLE();	
		__HAL_RCC_GPIOB_CLK_ENABLE();	
  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA9     ------> USART2_TX
    PA10     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;     //#### should be OUTPUT_PP mode, AF means alternating function,(see GPIO.h)
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;   //#### DO NOT need to set the AF function for PB4, as PB4 is a common control pin connected to ir module
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
#if EN_USART2_RX
		HAL_NVIC_EnableIRQ(USART2_IRQn);				
		HAL_NVIC_SetPriority(LPUART1_IRQn,4,0);			
#endif	
  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspDeInit 0 */

  /* USER CODE END LPUART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();
  
    /**LPUART1 GPIO Configuration    
    PB6     ------> LPUART1_TX
    PB7     ------> LPUART1_RX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN LPUART1_MspDeInit 1 */

  /* USER CODE END LPUART1_MspDeInit 1 */
  }
}

void HAL_IRDA_MspDeInit(IRDA_HandleTypeDef* irdaHandle)
{

  if(irdaHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA9     ------> USART2_TX
    PA10     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void MX_LPUART1_UART_DeInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
  /* USER CODE BEGIN LPUART1_MspInit 0 */
    __HAL_RCC_GPIOB_CLK_DISABLE();    //#### RTC for GPIO should not be disabled, if so, the function of all the GPIO pins will disabled
  /* USER CODE END LPUART1_MspInit 0 */
    /* LPUART1 clock enable */
    __HAL_RCC_LPUART1_CLK_DISABLE();
  
    /**LPUART1 GPIO Configuration    
    PB6     ------> LPUART1_TX
    PB7     ------> LPUART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN LPUART1_MspInit 1 */
		
  /* USER CODE END LPUART1_MspInit 1 */
}

void MX_USART2_IRDA_DeInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/* USER CODE BEGIN USART2_MspInit 0 */
    __HAL_RCC_GPIOA_CLK_DISABLE();	//#### RTC for GPIO should not be disabled, if so, the function of all the GPIO pins will disabled
		__HAL_RCC_GPIOB_CLK_DISABLE();	
  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_DISABLE();
	  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		

  /* USER CODE BEGIN USART2_MspInit 1 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
