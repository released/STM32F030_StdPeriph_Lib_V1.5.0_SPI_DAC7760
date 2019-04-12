/* Includes ------------------------------------------------------------------*/
#include "spi_DAC7760.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*SPI variable*/
uint8_t sDummy=0x5A;
//uint8_t Rx_Buffer1 = 0;

uint16_t Dataswap(uint16_t data)
{
    uint16_t temp=0;

		temp=data;
	
		temp=(temp>>8) | (data<<8);
	
		return temp;

}



void DAC7760_Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

uint8_t DAC7760_SPI_SendByte(uint8_t byte)
{
	/*!< Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	/*!< Send byte through the sFLASH_SPI peripheral */
	
//	SPI_I2S_SendData(sFLASH_SPI, byte);
//	SPI_I2S_SendData16(sFLASH_SPI, byte);
	SPI_SendData8(SPI1, byte);

	/*!< Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	/*!< Return the byte read from the SPI bus */
//	return SPI_I2S_ReceiveData(sFLASH_SPI);
//	return SPI_I2S_ReceiveData16(sFLASH_SPI);
	return SPI_ReceiveData8(SPI1);
}

void DAC7760_SPI_DMAReadBufferStart(uint32_t ReadAddr,uint16_t* pBuffer,uint16_t NumByteToWrite)
{
	DMA_InitTypeDef DMA_InitStructure;
	uint32_t Dummy = 0x5A ;	

	/*!< Send "Read from Memory " instruction */
	DAC7760_SPI_SendByte(DAC7760_READ_OPERATION);

//	/*!< Send ReadAddr high nibble address byte to read from */
//	DAC7760_SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
	/*!< Send ReadAddr medium nibble address byte to read from */
	DAC7760_SPI_SendByte((ReadAddr& 0xFF00) >> 8);
	/*!< Send ReadAddr low nibble address byte to read from */
	DAC7760_SPI_SendByte(ReadAddr & 0xFF);

	/* DMA configuration -------------------------------------------------------*/
	/* Deinitialize DMA Streams */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	DMA_DeInit(DMA1_Channel3);
	DMA_DeInit(DMA1_Channel2);

	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = NumByteToWrite ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPI1->DR)) ;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	/* Configure TX DMA */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t) &Dummy ;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = NumByteToWrite ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPI1->DR)) ;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Configure RX DMA */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC ;
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t) pBuffer ; 
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	/* Enable DMA SPI TX Stream */
	DMA_Cmd(DMA1_Channel3,ENABLE);
	/* Enable DMA SPI RX Stream */
	DMA_Cmd(DMA1_Channel2,ENABLE);
	/* Enable SPI DMA TX Requsts */
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	/* Enable SPI DMA RX Requsts */
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
	/* The Data transfer is performed in the SPI using Direct Memory Access */

}

void DAC7760_SPI_DMAWriteBufferStart(uint32_t WriteAddr,uint16_t pBuffer,uint16_t NumByteToWrite)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*!< Send "Read from Memory " instruction */
	DAC7760_SPI_SendByte(WriteAddr);

//	/*!< Send ReadAddr high nibble address byte to read from */
//	DAC7760_SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
//	/*!< Send ReadAddr medium nibble address byte to read from */
//	DAC7760_SPI_SendByte((ReadAddr& 0xFF00) >> 8);
//	/*!< Send ReadAddr low nibble address byte to read from */
//	DAC7760_SPI_SendByte(ReadAddr & 0xFF);

	/* DMA configuration -------------------------------------------------------*/
	/* Deinitialize DMA Streams */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	DMA_DeInit(DMA1_Channel3);
	DMA_DeInit(DMA1_Channel2);

	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = NumByteToWrite ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPI1->DR)) ;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	/* Configure TX DMA */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t) &pBuffer ;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = NumByteToWrite ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPI1->DR)) ;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Configure RX DMA */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC ;
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)&sDummy ; 
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	/* Enable DMA SPI TX Stream */
	DMA_Cmd(DMA1_Channel3,ENABLE);
	/* Enable DMA SPI RX Stream */
	DMA_Cmd(DMA1_Channel2,ENABLE);
	/* Enable SPI DMA TX Requsts */
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	/* Enable SPI DMA RX Requsts */
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
	/* The Data transfer is performed in the SPI using Direct Memory Access */

}

void DAC7760_SPI_DMABufferWait(void)
{
	/* Waiting the end of Data transfer */
	while (DMA_GetFlagStatus(DMA1_FLAG_TC3)==RESET);
	while (DMA_GetFlagStatus(DMA1_FLAG_TC2)==RESET);
	
	/* Clear DMA Transfer Complete Flags */
	DMA_ClearFlag(DMA1_FLAG_GL3);//DMA1_FLAG_TC3
	DMA_ClearFlag(DMA1_FLAG_GL2);//DMA1_FLAG_TC2  
	
	/* Disable DMA SPI TX Stream */
	DMA_Cmd(DMA1_Channel3,DISABLE);
	/* Disable DMA SPI RX Stream */
	DMA_Cmd(DMA1_Channel2,DISABLE);  
	
	/* Disable SPI DMA TX Requsts */
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
	/* Disable SPI DMA RX Requsts */
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);

}

void DAC7760_SPI_WriteResetRegister(uint16_t Data)
{
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAWriteBufferStart(DAC7760_WRITE_RESET,(uint16_t)(Data),1);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();		
}


//void DAC7760_SPI_WriteControlRegister(uint16_t Data)
//{
//	DAC7760_NCS_Low();	
//	
//	DAC7760_SPI_DMAWriteBufferStart(DAC7760_WRITE_CONTROL,(uint16_t)(Data),2);
//	DAC7760_SPI_DMABufferWait();	

//	DAC7760_NCS_High();		
//}

//void DAC7760_SPI_WriteConfigRegister(uint16_t Data)
//{
//	DAC7760_NCS_Low();	
//	
//	DAC7760_SPI_DMAWriteBufferStart(DAC7760_WRITE_CONFIG,(uint16_t)(Data),2);
//	DAC7760_SPI_DMABufferWait();	

//	DAC7760_NCS_High();		
//}


void DAC7760_SPI_WriteDataRegister(uint16_t Data)	//data input : 0 ~ 0xFFF
{
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAWriteBufferStart(DAC7760_WRITE_DATA,(uint16_t)(Data<<4),2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();		
}

void DAC7760_SPI_WriteGainRegister(uint16_t Data)
{
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAWriteBufferStart(DAC7760_WRITE_GAIN,(uint16_t)(Data<<4),2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();		
}

void DAC7760_SPI_WriteZeroRegister(uint16_t Data)
{
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAWriteBufferStart(DAC7760_WRITE_ZERO,(uint16_t)(Data<<4),2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();		
}

void DAC7760_SPI_WriteRegister(uint8_t Address , uint16_t Data)
{
#if 1	//use DMA to transfer data
//	Delay_ms(1);
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAWriteBufferStart(Address,Data,2);
	DAC7760_SPI_DMABufferWait();	

//	Delay_ms(1);	
	DAC7760_NCS_High();	
	
#else	//use regular SPI method to transfer data
//	Delay_ms(1);
	DAC7760_NCS_Low();	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{};
//	SPI_SendData8(SPI1,Data);
	SPI_SendData8(SPI1, ~(Data));
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{};
	SPI_ReceiveData8(SPI1);

//	Delay_ms(1);	
	DAC7760_NCS_High();
#endif
}


void DAC7760_SPI_WriteControlRegister(uint8_t Address , uint16_t Data)
{
#if 1	//use DMA to transfer data
	DAC7760_Delay(1);
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAWriteBufferStart(Address,Data,2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_Delay(1);	
	DAC7760_NCS_High();	
	
#else	//use regular SPI method to transfer data
//	Delay_ms(1);
	DAC7760_NCS_Low();	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{};
//	SPI_SendData8(SPI1,Data);
	SPI_SendData8(SPI1, ~(Data));
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{};
	SPI_ReceiveData8(SPI1);

//	Delay_ms(1);	
	DAC7760_NCS_High();
#endif
}

void DAC7760_SPI_WriteConfigRegister(uint8_t Address , uint16_t Data)
{
#if 1	//use DMA to transfer data
//	Delay_ms(1);
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAWriteBufferStart(Address,Data,2);
	DAC7760_SPI_DMABufferWait();	

//	Delay_ms(1);	
	DAC7760_NCS_High();	
	
#else	//use regular SPI method to transfer data
//	Delay_ms(1);
	DAC7760_NCS_Low();	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{};
//	SPI_SendData8(SPI1,Data);
	SPI_SendData8(SPI1, ~(Data));
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{};
	SPI_ReceiveData8(SPI1);

//	Delay_ms(1);	
	DAC7760_NCS_High();
#endif
}


uint16_t DAC7760_SPI_ReadControlRegister(void)
{
	uint16_t Data = 0 ;


	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAReadBufferStart(DAC7760_READ_CONTROL,&Data,2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();	

	return (Data);	
}

uint16_t DAC7760_SPI_ReadConfigRegister(void)
{
	uint16_t Data = 0 ;


	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAReadBufferStart(DAC7760_READ_CONFIG,&Data,2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();	

	return (Data);	
}

uint16_t DAC7760_SPI_ReadStatusRegister(void)
{
	uint16_t Data = 0 ;


	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAReadBufferStart(DAC7760_READ_STATUS,&Data,2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();	

	return (Data);	
}

uint16_t DAC7760_SPI_ReadDataRegister(void)
{
	uint16_t Data = 0 ;


	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAReadBufferStart(DAC7760_READ_DAC,&Data,2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();	

	return (Data>>4);	
}

uint16_t DAC7760_SPI_ReadGainRegister(void)
{
	uint16_t Data = 0 ;


	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAReadBufferStart(DAC7760_READ_GAIN,&Data,2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();	

	return (Data>>4);	
}

uint16_t DAC7760_SPI_ReadZeroRegister(void)
{
	uint16_t Data = 0 ;


	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAReadBufferStart(DAC7760_READ_ZERO,&Data,2);
	DAC7760_SPI_DMABufferWait();	

	DAC7760_NCS_High();	

	return (Data>>4);	
}

uint16_t DAC7760_SPI_ReadRegister(uint8_t Address)
{
#if 1	//use DMA to transfer data
	uint16_t Data = 0 ;

//	Delay_ms(1);
	DAC7760_NCS_Low();	
	
	DAC7760_SPI_DMAReadBufferStart(Address,&Data,2);
	DAC7760_SPI_DMABufferWait();	

//	Delay_ms(1);	
	DAC7760_NCS_High();	

	return Data;
	
#else	//use regular SPI method to transfer data
//	Delay_ms(1);
	DAC7760_NCS_Low();	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{};
//	SPI_SendData8(SPI1,Data);
	SPI_SendData8(SPI1, ~(Data));
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{};
	SPI_ReceiveData8(SPI1);

//	Delay_ms(1);	
	DAC7760_NCS_High();
#endif
}


void DAC7760_SPI_Read(uint8_t *data1,uint8_t *data2,uint8_t *data3)
{
	#if 1
	DAC7760_Delay(1);
	DAC7760_NCS_High();	
	*data1=DAC7760_SPI_SendByte(DAC7760_NOP);	
	*data2=DAC7760_SPI_SendByte(DAC7760_NOP);	
	*data3=DAC7760_SPI_SendByte(DAC7760_NOP);
	
	DAC7760_NCS_Low();	
	DAC7760_Delay(1);	
	DAC7760_NCS_High();
	DAC7760_Delay(1);	
	DAC7760_NCS_Low();	
	
	#else
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	printf("\r\n-----\r\n");
	
	*data1=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	printf("data1=0x%2X\r\n",*data1);
	
	*data2=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	printf("data2=0x%2X\r\n",*data2);
	
	*data3=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	printf("data3=0x%2X\r\n",*data3);
	printf("-----\r\n");
	
	DAC7760_Delay(50);	
	DAC7760_NCS_High();	
	#endif
}

void DAC7760_SPI_Write(uint8_t cmd,uint8_t data1,uint8_t data2)
{
	#if 1
	DAC7760_Delay(1);
	DAC7760_NCS_High();	
	DAC7760_SPI_SendByte(cmd);
	DAC7760_SPI_SendByte((uint8_t)data1);
	DAC7760_SPI_SendByte((uint8_t)data2);

	DAC7760_NCS_Low();
	DAC7760_Delay(1);
	DAC7760_NCS_High();
	DAC7760_Delay(1);	
	DAC7760_NCS_Low();	
	#else
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	DAC7760_SPI_SendByte(cmd);//DAC7760_Delay(25);
	DAC7760_SPI_SendByte((uint8_t)data1);//DAC7760_Delay(25);
	DAC7760_SPI_SendByte((uint8_t)data2);//DAC7760_Delay(25);
	DAC7760_Delay(50);
	DAC7760_NCS_High();	
	#endif
}


void DAC7760_SPI_LowLevel_Initial(DAC7760_Command_InitTypeDef *Command)
{
	uint16_t tmp1 = 0;

	#if 1
	DAC7760_SPI_WriteResetRegister(DAC7760_RESET_ON);

	tmp1 = DAC7760_SPI_ReadControlRegister();

	/*Control*/
    tmp1 &= ~(DAC7760_CONTROL_CLRSEL_MASK);
    tmp1 |= Command->Control.CLRSEL;

    tmp1 &= ~(DAC7760_CONTROL_OVR_MASK);
    tmp1 |= Command->Control.OVR;

    tmp1 &= ~(DAC7760_CONTROL_REXT_MASK);
    tmp1 |= Command->Control.REXT;			

    tmp1 &= ~(DAC7760_CONTROL_OUTEN_MASK);
    tmp1 |= Command->Control.OUTEN;		

    tmp1 &= ~(DAC7760_CONTROL_SRCLK_MASK);
    tmp1 |= Command->Control.SRCLK;

    tmp1 &= ~(DAC7760_CONTROL_SRSTEP_MASK);
    tmp1 |= Command->Control.SRSTEP;	

    tmp1 &= ~(DAC7760_CONTROL_SREN_MASK);
    tmp1 |= Command->Control.SREN;		

    tmp1 &= ~(DAC7760_CONTROL_DCEN_MASK);
    tmp1 |= Command->Control.DCEN;

    tmp1 &= ~(DAC7760_CONTROL_RANGE_MASK);
    tmp1 |= Command->Control.RANGE;	

	printf("WriteControl = 0x%2X\r\n",tmp1);//DEBUG
//	DAC7760_SPI_WriteControlRegister(Dataswap(tmp1));
	DAC7760_SPI_WriteControlRegister(0x55,tmp1);
	
	tmp1 = DAC7760_SPI_ReadConfigRegister();

		tmp1=0;

	/*Config*/
    tmp1 &= ~(DAC7760_CONFIG_IOUTRANGE_MASK);
    tmp1 |= Command->Config.IOUTRANGE;		

    tmp1 &= ~(DAC7760_CONFIG_DUALOUTEN_MASK);
    tmp1 |= Command->Config.DUALOUTEN;		

    tmp1 &= ~(DAC7760_CONFIG_APD_MASK);
    tmp1 |= Command->Config.APD;

    tmp1 &= ~(DAC7760_CONFIG_CALEN_MASK);
    tmp1 |= Command->Config.CALEN;

    tmp1 &= ~(DAC7760_CONFIG_HARTEN_MASK);
    tmp1 |= Command->Config.HARTEN;	

    tmp1 &= ~(DAC7760_CONFIG_CRCEN_MASK);
    tmp1 |= Command->Config.CRCEN;

    tmp1 &= ~(DAC7760_CONFIG_WDEN_MASK);
    tmp1 |= Command->Config.WDEN;

    tmp1 &= ~(DAC7760_CONFIG_WDPD_MASK);
    tmp1 |= Command->Config.WDPD;	

	printf("WriteConfig = 0x%2X\r\n",tmp1);//DEBUG	
//	DAC7760_SPI_WriteConfigRegister(Dataswap(tmp1));
		DAC7760_SPI_WriteConfigRegister(0x57,tmp1);
	#endif
}

void DAC7760_SPI_InitialSetting(void)
{
	#if 1
	uint8_t control1 = 0;
	uint8_t control2 = 0;	
	uint8_t config1 = 0;
	uint8_t config2 = 0;
	
	control1 = (uint8_t)((DAC7760_CONTROL_CLRSEL_OFF|	\
							DAC7760_CONTROL_OVR_OFF|	\
							DAC7760_CONTROL_REXT_OFF|	\
							DAC7760_CONTROL_OUTEN_ON|	\
							DAC7760_CONTROL_SRCLK_258065)>>8);
	control2 = (uint8_t)(DAC7760_CONTROL_SRSTEP_00625|	\
						DAC7760_CONTROL_SREN_OFF|	\
						DAC7760_CONTROL_DCEN_OFF|	\
						DAC7760_CONTROL_RANGE_0V_5V);
	printf("control:0x%2X,0x%2X\r\n",control1,control2);
	DAC7760_SPI_Write(DAC7760_WRITE_CONTROL,control1,control2);
	
	config1 = (uint8_t)((DAC7760_CONFIG_IOUTRANGE_DISABLE|	\
						DAC7760_CONFIG_DUALOUTEN_OFF)>>8);
	config2 = (uint8_t)(DAC7760_CONFIG_APD_OFF|	\
						DAC7760_CONFIG_CALEN_OFF|	\
						DAC7760_CONFIG_HARTEN_OFF|	\
						DAC7760_CONFIG_CRCEN_OFF|	\
						DAC7760_CONFIG_WDEN_OFF|DAC7760_CONFIG_WDPD_10ms);	
	printf("config:0x%2X,0x%2X\r\n",config1,config2);
	DAC7760_SPI_Write(DAC7760_WRITE_CONFIG,config1,config2);
	
	#else
	DAC7760_Command_InitTypeDef Command;

//	Command.Control.CLRSEL 		= DAC7760_CONTROL_CLRSEL_ON;
	Command.Control.CLRSEL 		= DAC7760_CONTROL_CLRSEL_OFF;
	Command.Control.OVR 		= DAC7760_CONTROL_OVR_OFF;
	Command.Control.REXT 		= DAC7760_CONTROL_REXT_OFF;
	Command.Control.OUTEN 		= DAC7760_CONTROL_OUTEN_ON;	
	Command.Control.SRCLK 		= DAC7760_CONTROL_SRCLK_258065;
	Command.Control.SRSTEP 	= DAC7760_CONTROL_SRSTEP_1;
	Command.Control.SREN 		= DAC7760_CONTROL_SREN_OFF;
	Command.Control.DCEN 		= DAC7760_CONTROL_DCEN_OFF;
	Command.Control.RANGE 		= DAC7760_CONTROL_RANGE_0V_5V;

	Command.Config.IOUTRANGE 	= DAC7760_CONFIG_IOUTRANGE_DISABLE;
	Command.Config.DUALOUTEN 	= DAC7760_CONFIG_DUALOUTEN_OFF;
	Command.Config.APD 		= DAC7760_CONFIG_APD_OFF;
	Command.Config.CALEN 		= DAC7760_CONFIG_CALEN_OFF;		
	Command.Config.HARTEN 	= DAC7760_CONFIG_HARTEN_OFF;
	Command.Config.CRCEN 		= DAC7760_CONFIG_CRCEN_OFF;
	Command.Config.WDEN 		= DAC7760_CONFIG_WDEN_OFF;
	Command.Config.WDPD 		= DAC7760_CONFIG_WDPD_204ms;	

	DAC7760_SPI_LowLevel_Initial(&Command);
	#endif
}

void DAC7760_SPI_Command_Template(void)
{
	uint8_t res1 =0;
	uint8_t res2 =0;		
	uint8_t res3 =0;
	
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	DAC7760_SPI_SendByte(DAC7760_WRITE_DATA);DAC7760_Delay(25);
	DAC7760_SPI_SendByte((uint8_t)0x26);DAC7760_Delay(25);
	DAC7760_SPI_SendByte((uint8_t)0x74);DAC7760_Delay(25);
	DAC7760_Delay(50);
	DAC7760_NCS_High();	
	
	DAC7760_Delay(50);	
	DAC7760_NCS_Low();	
	DAC7760_SPI_SendByte(0x02);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x00);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(DAC7760_READ_DAC);DAC7760_Delay(25);//DAC7760_READ_CONFIG
	DAC7760_Delay(50);	
	DAC7760_NCS_High();		
	
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	res1=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	res2=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	res3=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	DAC7760_Delay(50);	
	DAC7760_NCS_High();		
	printf("res1: 0x%2X,0x%2X,0x%2X\r\n",res1,res2,res3);
}

void DAC7760_SPI_Command_Example(void)
{
//	uint8_t i = 0;
//	uint8_t j = 0;
	uint8_t res1 =0xFF;
	uint8_t res2 =0xFF;		
	uint8_t res3 =0xFF;
	#if 1

//	DAC7760_SPI_Write(DAC7760_READ_OPERATION,0x00,DAC7760_READ_CONTROL);	
//	DAC7760_SPI_Read(&res1,&res2,&res3);	
//	DAC7760_SPI_Write(DAC7760_READ_OPERATION,0x00,DAC7760_READ_CONFIG);	
//	DAC7760_SPI_Read(&res1,&res2,&res3);	

	DAC7760_SPI_Write(DAC7760_WRITE_DATA,0x3F,0xFF);
	DAC7760_SPI_Write(DAC7760_READ_OPERATION,0x00,DAC7760_READ_DAC);	
	DAC7760_SPI_Read(&res1,&res2,&res3);

	#if 1
	printf("\r\n-----\r\n");
	printf("data1=0x%2X\r\n",res1);	
	printf("data2=0x%2X\r\n",res2);
	printf("data3=0x%2X\r\n",res3);
	printf("-----\r\n");
	#endif

	#else
//	DAC7760_SPI_WriteRegister(0x55,Dataswap(0xFE02));
//		DAC7760_SPI_WriteRegister(0x57,Dataswap(0xFE02));
	
//	DAC7760_SPI_WriteControlRegister(0x55,0xFE02);
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	DAC7760_SPI_SendByte(DAC7760_WRITE_CONTROL);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x10);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x00);DAC7760_Delay(25);
	DAC7760_Delay(50);
	DAC7760_NCS_High();	
	
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	DAC7760_SPI_SendByte(DAC7760_WRITE_CONFIG);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x00);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x00);DAC7760_Delay(25);
	DAC7760_Delay(50);
	DAC7760_NCS_High();		
	
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	DAC7760_SPI_SendByte(DAC7760_WRITE_DATA);DAC7760_Delay(25);
//	DAC7760_SPI_SendByte(0x1F);DAC7760_Delay(25);
//	DAC7760_SPI_SendByte(0x90);DAC7760_Delay(25);
//	DAC7760_SPI_SendByte(0xFF);DAC7760_Delay(25);
//	DAC7760_SPI_SendByte(0xFF);DAC7760_Delay(25);
//	DAC7760_SPI_SendByte(0x55);DAC7760_Delay(25);
//	DAC7760_SPI_SendByte(0x99);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x00);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x00);DAC7760_Delay(25);
	DAC7760_Delay(50);
	DAC7760_NCS_High();	
	
	DAC7760_Delay(50);	
	DAC7760_NCS_Low();	
	DAC7760_SPI_SendByte(DAC7760_READ_OPERATION);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(0x00);DAC7760_Delay(25);
//		DAC7760_SPI_SendByte(0x01);DAC7760_Delay(25);
	DAC7760_SPI_SendByte(DAC7760_READ_DAC);DAC7760_Delay(25);//DAC7760_READ_CONFIG
//		DAC7760_SPI_SendByte(0x03);DAC7760_Delay(25);//DAC7760_READ_CONFIG
	DAC7760_Delay(50);	
	DAC7760_NCS_High();		
	
	DAC7760_Delay(50);
	DAC7760_NCS_Low();	
	res1=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	res2=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	res3=DAC7760_SPI_SendByte(DAC7760_NOP);DAC7760_Delay(25);
	DAC7760_Delay(50);	
	DAC7760_NCS_High();		
	printf("res1:0x%2X,res2:0x%2X,res3:0x%2X\r\n",res1,res2,res3);
	
	
//	DAC7760_SPI_WriteConfigRegister(0x57,0xFE02);
//	res = DAC7760_SPI_ReadConfigRegister();
//	printf("res 2  : 0x%8X\r\n",res);	
	
//	DAC7760_SPI_WriteControlRegister(0x55,0x1000);
//	DAC7760_SPI_WriteConfigRegister(0x57,0x0000);

	
	
//	DAC7760_SPI_WriteDataRegister(0xFFF);
	DAC7760_Delay(0xFF);

//	DAC7760_SPI_WriteDataRegister(0x0FF);
//	DAC7760_Delay(0xFFFF);	

//	DAC7760_SPI_WriteDataRegister(0x07F);
//	DAC7760_Delay(0xFFFF);		
	#endif
}

/*====================================================================
PCB					DAC7760							MCU
----------------------------------------------------------------------			
*			MR#   : master reset (active LOW)			=>VCC
SCLOCK		SHCP  : shift register clock input			=>SPI1_CLK , PA5
LATCH		STCP  : storage register clock input		=>SPI1_NSS , PA8
DIN			DS    : serial data input					=>SPI1_MOSI , PA7
SDO			DS    : serial data out						=>SPI1_MISO , PA6

====================================================================*/

void DAC7760_SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	/* Enable the SPI periph */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Enable SCK, MOSI, MISO and NSS GPIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_0);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//NSS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	DAC7760_NCS_High();
	
	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Initialize the FIFO threshold */
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);

	/* Enable the SPI peripheral */
	SPI_Cmd(SPI1, ENABLE);

}


