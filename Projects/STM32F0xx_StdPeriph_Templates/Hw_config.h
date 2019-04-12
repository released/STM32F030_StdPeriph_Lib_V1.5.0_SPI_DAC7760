/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Platform config -----------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <stdio.h>
#include "string.h"

#include "spi_DAC7760.h"

/* Define config -------------------------------------------------------------*/
#define TRUE			1
#define FALSE           0
#define 	ON				1
#define 	OFF				0
typedef unsigned char   BOOL;
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define NCS_Low()       							GPIO_ResetBits(GPIOC, GPIO_Pin_6)
#define NCS_High()      							GPIO_SetBits(GPIOC, GPIO_Pin_6)  
/* Macro ---------------------------------------------------------------------*/
/*
#define UartTxPutChar(x)		\
{	\
     UART1_SendData8(x);	\
     while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);	\
}*/

/* Exported types ------------------------------------------------------------*/
void GPIO_Test(void);
void GPIO_Config(void);

void USART_TEST(void);
void USART_Config(void);  

void SysTickTimer_Config(void);

void Delay(__IO uint32_t Time);

void TimingDelay_Decrement(void);
void UART_SendByte(uint8_t Data);
void UART_SendString(uint8_t* Data,uint16_t len);
void SystemClkDelay(uint32_t u32Delay);
/* Exported constants --------------------------------------------------------*/

#endif  /* __HW_CONFIG_H */

