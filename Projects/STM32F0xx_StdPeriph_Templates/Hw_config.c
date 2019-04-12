/* Includes ------------------------------------------------------------------*/
#include "Hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;

void GPIO_Test(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_5);

	GPIO_SetBits(GPIOA,GPIO_Pin_6);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//	
//	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);

//	GPIO_SetBits(GPIOA,GPIO_Pin_8);	
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);	

	printf("PA5:0x%x\r\n",GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5));
	printf("PA6:0x%x\r\n",GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6));
	printf("PA7:0x%x\r\n",GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7));
	printf("PA8:0x%x\r\n",GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8));	
	
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void USART_TEST(void)
{
	__IO uint8_t temp;
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
			temp = USART_ReceiveData(USART1);
			printf("Press KEY : %c \n\r",temp);

			switch (temp)
			{
				case '1': 

					break;

				case '2': 

					break;	

				case '3':

					break;
					
				default : 
					printf("INPUT CMD not support !\r\n");
					break;
			}
	}
}

void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Clock configuration ---------------------------------------------------*/
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* GPIO configuration ----------------------------------------------------*/
	GPIO_DeInit(GPIOA);
//	GPIO_DeInit(GPIOB);
	/* Connect PXx to USARTx_Tx */
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	/* Configure USARTx_Tx,USARTx_Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* USART configuration ---------------------------------------------------*/
	USART_DeInit(USART1);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* Enable USARTy Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	/* The software must wait until TC=1. The TC flag remains cleared during all data
	transfers and it is set by hardware at the last frame’s end of transmission*/	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	/* NVIC configuration */
	/* Enable the USARRx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 

	/* Enable the USARRx */
	USART_Cmd(USART1, ENABLE);
}

void SysTickTimer_Config(void)
{
	RCC_ClocksTypeDef 	RCC_ClockFreq;

	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	#if 1 //debug
	printf("===========================\r\n");
	printf("SYSCLK_Frequency = %d \r\n" , 		RCC_ClockFreq.SYSCLK_Frequency);
	printf("HCLK_Frequency = %d \r\n" , 			RCC_ClockFreq.HCLK_Frequency);
	printf("PCLK_Frequency = %d \r\n" , 			RCC_ClockFreq.PCLK_Frequency);
	printf("ADCCLK_Frequency= %d \r\n" , 		RCC_ClockFreq.ADCCLK_Frequency);
	printf("CECCLK_Frequency = %d \r\n" , 		RCC_ClockFreq.CECCLK_Frequency);
	printf("I2C1CLK_Frequency = %d \r\n" , 		RCC_ClockFreq.I2C1CLK_Frequency);
	printf("USART1CLK_Frequency = %d \r\n" , 	RCC_ClockFreq.USART1CLK_Frequency); 
	#endif /*debug*/
	
	/* Setup SysTick Timer for 1ms interrupts  */
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1);
	}
	
	/* Configure the SysTick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x01);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t Time)
{ 
	uwTimingDelay = Time;
	while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

//currently not use
/*

void SystemClkDelay(void)
{
	uint32_t i;
	i = 0xffff;
	while(i--);
}

void wtPutChar(uint8_t ccc)
{
	UART1_SendData8(ccc);
	// Loop until the end of transmission 
	while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);	
}

u16 GetAbsTime(u16 a,u16 b)
{
	u16 c;
	if(a>=b) c=(a-b);
	else c=65535-(b-a);	
	
	return c;
}
*/

void UART_SendByte(uint8_t Data)
{
	USART_SendData(USART1 , (unsigned char)Data);
	while (USART_GetFlagStatus(USART1 , USART_FLAG_TXE)==RESET);
	{
	}
}

void UART_SendString(uint8_t* Data,uint16_t len)
{
	uint16_t i=0;
	for(i=0;i<len;i++ )
	{
		UART_SendByte(Data[i]);
	}
}

void SystemClkDelay(uint32_t u32Delay)
{
	//uint32_t i;
	//i = 0xffff;
	while(u32Delay--);
}


