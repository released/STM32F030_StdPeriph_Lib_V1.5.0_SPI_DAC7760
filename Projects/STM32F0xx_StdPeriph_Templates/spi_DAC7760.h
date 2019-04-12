/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_DAC7760_H
#define __SPI_DAC7760_H

/* Platform config -----------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "string.h"

#include "stm32f0xx.h"

/* Define config -------------------------------------------------------------*/
#define DAC7760_NCS_Low()       					GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define DAC7760_NCS_High()      					GPIO_SetBits(GPIOA, GPIO_Pin_8) 

//Table 3. Write Address Functions
#define 	DAC7760_NOP								((uint8_t)0x00)

//DAC7760 (12-bit version) register contents are located in DB15:DB4. For
//DAC7760, DB3:DB0 are don't care bits when writing and zeros when reading
#define 	DAC7760_WRITE_DATA						((uint8_t)0x01)

#define 	DAC7760_READ_OPERATION					((uint8_t)0x02)
#define 	DAC7760_WRITE_CONTROL						((uint8_t)0x55)
#define 	DAC7760_WRITE_RESET						((uint8_t)0x56)
#define 	DAC7760_WRITE_CONFIG						((uint8_t)0x57)

//DAC7760 (12-bit version) register contents are located in DB15:DB4. For
//DAC7760, DB3:DB0 are don't care bits when writing and zeros when reading
#define 	DAC7760_WRITE_GAIN						((uint8_t)0x58)
#define 	DAC7760_WRITE_ZERO						((uint8_t)0x59)

#define 	DAC7760_WRITE_WDGRESET					((uint8_t)0x95)

//Table 5. Register Read Address Functions
#define 	DAC7760_READ_STATUS						((uint8_t)0x00)
#define 	DAC7760_READ_DAC							((uint8_t)0x01)
#define 	DAC7760_READ_CONTROL						((uint8_t)0x02)
#define 	DAC7760_READ_CONFIG						((uint8_t)0x0B)
#define 	DAC7760_READ_GAIN							((uint8_t)0x13)
#define 	DAC7760_READ_ZERO							((uint8_t)0x17)

/********************************************************************************/

//	Table 7. Control Register	

/*
	VOUT clear value select bit.
	When bit = '0', VOUT is 0 V in DAC CLEAR mode or after reset.
	When bit = '1', VOUT is midscale in unipolar output and negative-full-scale in bipolar output
	in DAC CLEAR mode or after reset.

	DB15 CLRSEL
	
*/
#define DAC7760_CONTROL_CLRSEL_ON					((uint16_t)(1<<15))
#define DAC7760_CONTROL_CLRSEL_OFF				((uint16_t)(0<<15))

#define DAC7760_CONTROL_CLRSEL_MASK      		((uint16_t)DAC7760_CONTROL_CLRSEL_ON)

/*
	Table 18. CLR-SEL Options
	
								OUTPUT VALUE
	CLR-SEL		UNIPOLAR OUTPUT RANGE 		BIPOLAR OUTPUT RANGE
	0 			0 V 						0 V
	1 			Midscale 					Negative full-scale
*/

/*
	Setting the bit increases the voltage output range by 10%.

	DB14 OVR
	
*/
#define DAC7760_CONTROL_OVR_ON					((uint16_t)(1<<14))
#define DAC7760_CONTROL_OVR_OFF					((uint16_t)(0<<14))

#define DAC7760_CONTROL_OVR_MASK      			((uint16_t)DAC7760_CONTROL_OVR_ON)

/*
	External current setting resistor enable.

	DB13 REXT
	
*/
#define DAC7760_CONTROL_REXT_ON					((uint16_t)(1<<13))
#define DAC7760_CONTROL_REXT_OFF					((uint16_t)(0<<13))

#define DAC7760_CONTROL_REXT_MASK      			((uint16_t)DAC7760_CONTROL_REXT_ON)

/*
	Output enable.
	Bit = '1': Output is determined by RANGE bits.
	Bit = '0': Output is disabled. IOUT and VOUT are Hi-Z.

	DB12 OUTEN
	
*/
#define DAC7760_CONTROL_OUTEN_ON				((uint16_t)(1<<12))
#define DAC7760_CONTROL_OUTEN_OFF				((uint16_t)(0<<12))

#define DAC7760_CONTROL_OUTEN_MASK      		((uint16_t)DAC7760_CONTROL_OUTEN_ON)

/*
	Slew rate clock control. Ignored when bit SREN = '0'
	
	DB11:DB8 SRCLK[3:0]

	Table 22. Slew Rate Update Clock (SRCLK) Options
	SRCLK		DAC UPDATE FREQUENCY (Hz)
	0000 		258,065
	0001 		200,000
	0010 		153,845
	0011 		131,145
	0100 		115,940
	0101 		69,565
	0110 		37,560
	0111 		25,805
	1000 		20,150
	1001 		16,030
	1010 		10,295
	1011 		8,280
	1100 		6,900
	1101 		5,530
	1110 		4,240
	1111 		3,300	
*/
#define DAC7760_CONTROL_SRCLK_258065				((uint16_t)((0x00)<<8))
#define DAC7760_CONTROL_SRCLK_200000				((uint16_t)((0x01)<<8))
#define DAC7760_CONTROL_SRCLK_153845				((uint16_t)((0x02)<<8))
#define DAC7760_CONTROL_SRCLK_131145				((uint16_t)((0x03)<<8))
#define DAC7760_CONTROL_SRCLK_115940				((uint16_t)((0x04)<<8))
#define DAC7760_CONTROL_SRCLK_69565				((uint16_t)((0x05)<<)8)
#define DAC7760_CONTROL_SRCLK_37560				((uint16_t)((0x06)<<)8)
#define DAC7760_CONTROL_SRCLK_25805				((uint16_t)((0x07)<<)8)
#define DAC7760_CONTROL_SRCLK_20150				((uint16_t)((0x08)<<)8)
#define DAC7760_CONTROL_SRCLK_16030				((uint16_t)((0x09)<<8))
#define DAC7760_CONTROL_SRCLK_10295				((uint16_t)((0x0A)<<8))
#define DAC7760_CONTROL_SRCLK_8280				((uint16_t)((0x0B)<<8))
#define DAC7760_CONTROL_SRCLK_6900				((uint16_t)((0x0C)<<8))
#define DAC7760_CONTROL_SRCLK_5530				((uint16_t)((0x0D)<<8))
#define DAC7760_CONTROL_SRCLK_4240				((uint16_t)((0x0E)<<8))
#define DAC7760_CONTROL_SRCLK_3300				((uint16_t)((0x0F)<<8))

#define DAC7760_CONTROL_SRCLK_MASK      			((uint16_t)DAC7760_CONTROL_SRCLK_3300)

/*
	Slew rate step size control. Ignored when bit SREN = '0'
	
	DB7:DB5 SRSTEP[2:0]

	Table 21. Slew Rate Step-Size (SRSTEP) Options
	SRSTEP 	DAC7760 	
	000 	0.0625 	
	001 	0.125 	
	010 	0.125 	
	011 	0.5 	
	100 	1 		
	101 	2 		
	110 	4 		
	111 	8	

*/
#define DAC7760_CONTROL_SRSTEP_00625				((uint16_t)(0<<5))
#define DAC7760_CONTROL_SRSTEP_0125				((uint16_t)(1<<5))
#define DAC7760_CONTROL_SRSTEP_0125_2				((uint16_t)(2<<5))
#define DAC7760_CONTROL_SRSTEP_05					((uint16_t)(3<<5))
#define DAC7760_CONTROL_SRSTEP_1					((uint16_t)(4<<5))
#define DAC7760_CONTROL_SRSTEP_2					((uint16_t)(5<<5))
#define DAC7760_CONTROL_SRSTEP_4					((uint16_t)(6<<5))
#define DAC7760_CONTROL_SRSTEP_8					((uint16_t)(7<<5))

#define DAC7760_CONTROL_SRSTEP_MASK      		((uint16_t)DAC7760_CONTROL_SRSTEP_8)

/*
	Slew Rate Enable.
	Bit = '1': Slew rate control is enabled, and the ramp speed of the output change is
	determined by SRCLK and SRSTEP.
	Bit = '0': Slew rate control is disabled. Bits SRCLK and SRSTEP are ignored. The output
	changes to the new level immediately.

	DB4 SREN
	
*/
#define DAC7760_CONTROL_SREN_ON					((uint16_t)(1<<4))
#define DAC7760_CONTROL_SREN_OFF					((uint16_t)(0<<4))

#define DAC7760_CONTROL_SREN_MASK      			((uint16_t)DAC7760_CONTROL_SREN_ON)

/*
	Daisy-chain enable.

	DB3 DCEN	
*/
#define DAC7760_CONTROL_DCEN_ON					((uint16_t)(1<<3))
#define DAC7760_CONTROL_DCEN_OFF					((uint16_t)(0<<3))

#define DAC7760_CONTROL_DCEN_MASK      			((uint16_t)DAC7760_CONTROL_DCEN_ON)

/*
	Output range bits.
	
	DB2:DB0 RANGE[2:0]

	Table 14. RANGE Bits vs Output Range
	RANGE 		OUTPUT RANGE
	000 		0 V to +5 V
	001 		0 V to +10 V
	010 		¡Ó5 V
	011 		¡Ó10 V
	100 		Not allowed(1)
	101 		4 mA to 20 mA
	110 		0 mA to 20 mA
	111 		0 mA to 24 mA	
*/
#define DAC7760_CONTROL_RANGE_0V_5V				((uint16_t)(0<<0))
#define DAC7760_CONTROL_RANGE_0V_10V				((uint16_t)(1<<0))
#define DAC7760_CONTROL_RANGE_5V					((uint16_t)(2<<0))
#define DAC7760_CONTROL_RANGE_10V				((uint16_t)(3<<0))
#define DAC7760_CONTROL_RANGE_NA				((uint16_t)(4<<0))
#define DAC7760_CONTROL_RANGE_4mA_20mA			((uint16_t)(5<<0))
#define DAC7760_CONTROL_RANGE_0mA_20mA			((uint16_t)(6<<0))
#define DAC7760_CONTROL_RANGE_0mA_24mA			((uint16_t)(7<<0))

#define DAC7760_CONTROL_RANGE_MASK      		((uint16_t)DAC7760_CONTROL_RANGE_0mA_24mA)

/********************************************************************************/

//Table 8. Configuration Register

/*
	Reserved. User must not write any value other than zero to these bits.

	DB15:DB11
*/

/*
	IOUT range. These bits are only used if both voltage and current outputs are
	simultaneously enabled via bit 8 (DUAL OUTEN). The voltage output range is
	still controlled by bits 2:0 of the Control Register (RANGE bits). The current
	range is controlled by these bits and has similar behavior to RANGE[1:0] when
	RANGE[2] = '1'. However, unlike the RANGE bits, a change to this field does
	not make the DAC data register go to its default value.

	DB10:DB9 IOUT RANGE

	Table 17. IOUT RANGE Bits versus Current Output Range in Dual-Output Mode
	RANGE 	OUTPUT RANGE
	00 		Disabled
	01 		4 mA to 20 mA
	10 		0 mA to 20 mA
	11 		0 mA to 24 mA

*/
#define DAC7760_CONFIG_IOUTRANGE_DISABLE			((uint16_t)(0<<9))
#define DAC7760_CONFIG_IOUTRANGE_4mA_20mA		((uint16_t)(1<<9))
#define DAC7760_CONFIG_IOUTRANGE_0mA_20mA		((uint16_t)(2<<9))
#define DAC7760_CONFIG_IOUTRANGE_0mA_22mA		((uint16_t)(3<<9))

#define DAC7760_CONFIG_IOUTRANGE_MASK      		((uint16_t)DAC7760_CONFIG_IOUTRANGE_0mA_22mA)

/*
	DAC dual output enable. This bit controls if the voltage and current outputs are
	enabled simultaneously. Both are enabled when this bit is high. However, both
	outputs are controlled by the same DAC data register.

	DB8 DUAL OUTEN
*/
#define DAC7760_CONFIG_DUALOUTEN_ON				((uint16_t)(1<<8))
#define DAC7760_CONFIG_DUALOUTEN_OFF			((uint16_t)(0<<8))

#define DAC7760_CONFIG_DUALOUTEN_MASK      	((uint16_t)DAC7760_CONFIG_DUALOUTEN_ON)

/*
	Alternate power down. On power-up, +VSENSE is connected to the internal
	VOUT amplifier inverting terminal. Diodes exist at this node to REFIN and
	GND. Setting this bit connects this node to ground through a resistor. When
	set, the equivalent resistance seen from +VSENSE to GND is 70 k£[. This is
	useful in applications where the VOUT and IOUT terminals are tied together.

	DB7 APD
*/
#define DAC7760_CONFIG_APD_ON					((uint16_t)(1<<7))
#define DAC7760_CONFIG_APD_OFF					((uint16_t)(0<<7))

#define DAC7760_CONFIG_APD_MASK      			((uint16_t)DAC7760_CONFIG_APD_ON)

/*
	DB6 0 Reserved. Do not write any value other than zero to these bits.
*/

/*
	User calibration enable. When user calibration is enabled, the DAC data are
	adjusted according to the contents of the gain and zero calibration registers.
	See the USER CALIBRATION section.

	DB5 CALEN
*/
#define DAC7760_CONFIG_CALEN_ON					((uint16_t)(1<<5))
#define DAC7760_CONFIG_CALEN_OFF					((uint16_t)(0<<5))

#define DAC7760_CONFIG_CALEN_MASK      			((uint16_t)DAC7760_CONFIG_CALEN_ON)

/*
	Enable interface through HART-IN pin (only valid for IOUT set to 4-mA to 20-
	mA range via RANGE bits).
	Bit = '1': HART signal is connected through internal resistor and modulates
	output current.
	Bit = '0': HART interface is disabled.

	DB4 HARTEN
*/
#define DAC7760_CONFIG_HARTEN_ON					((uint16_t)(1<<4))
#define DAC7760_CONFIG_HARTEN_OFF				((uint16_t)(0<<4))

#define DAC7760_CONFIG_HARTEN_MASK      		((uint16_t)DAC7760_CONFIG_HARTEN_ON)

/*
	Enable frame error checking.

	DB3 CRCEN
*/
#define DAC7760_CONFIG_CRCEN_ON					((uint16_t)(1<<3))
#define DAC7760_CONFIG_CRCEN_OFF					((uint16_t)(0<<3))

#define DAC7760_CONFIG_CRCEN_MASK      			((uint16_t)DAC7760_CONFIG_CRCEN_ON)

/*
	Watchdog timer enable.

	DB2 WDEN
*/
#define DAC7760_CONFIG_WDEN_ON					((uint16_t)(1<<2))
#define DAC7760_CONFIG_WDEN_OFF					((uint16_t)(0<<2))

#define DAC7760_CONFIG_WDEN_MASK      			((uint16_t)DAC7760_CONFIG_WDEN_ON)

/*
	Watchdog timeout period.

	DB1:DB0 WDPD[1:0]

	WDPD 	BITS WATCHDOG TIMEOUT PERIOD (Typical, ms)
	00 		10 ms
	01 		51 ms
	10 		102 ms
	11 		204 ms
	
*/
#define DAC7760_CONFIG_WDPD_10ms					((uint16_t)(0<<0))
#define DAC7760_CONFIG_WDPD_51ms					((uint16_t)(1<<0))
#define DAC7760_CONFIG_WDPD_102ms				((uint16_t)(2<<0))
#define DAC7760_CONFIG_WDPD_204ms				((uint16_t)(3<<0))

#define DAC7760_CONFIG_WDPD_MASK      			((uint16_t)DAC7760_CONFIG_WDPD_204ms)

/********************************************************************************/

/*
	Reserved	Writing to these bits does not cause any change.
	
	DB15:DB1 0000h 

	Software reset bit. Writing 1 to the bit performs a software reset to reset all
	0 registers and the ALARM status to the respective power-on reset default value.
	After reset completes the RESET bit clears itself.

	DB0 RESET
*/
#define DAC7760_RESET_ON							((uint16_t)(1<<0))
#define DAC7760_RESET_OFF							((uint16_t)(0<<0))

/********************************************************************************/

/*Table 13. Status Register*/
/*
	DB15:DB5 000h Reserved. Reading these bits returns 0.
*/

/*
	Bit = '1' indicates CRC error on SPI frame.
	Bit = '0' indicates normal operation.

	DB4 CRC-FLT
*/
#define DAC7760_STATUS_CRC_FLT						((uint16_t)(1<<4))

/*
	Bit = '1' indicates watchdog timer timeout.
	Bit = '0' indicates normal operation.

	DB3 WD-FLT	
*/
#define DAC7760_STATUS_WD_FLT						((uint16_t)(1<<3))

/*
	Bit = '1' indicates Open Circuit or Compliance Voltage Violation in IOUT
	loading.
	Bit = '0' indicates IOUT load is at normal condition.

	DB2 I-FLT
*/
#define DAC7760_STATUS_I_FLT						((uint16_t)(1<<2))

/*
	Bit = '1' when DAC code is slewing as determined by SRCLK and SRSTEP.
	Bit = '0' when DAC code is not slewing.

	DB1 SR-ON
*/
#define DAC7760_STATUS_SR_ON						((uint16_t)(1<<1))

/*
	Bit = '1' indicates die temperature is over +142¢XC.
	Bit = '0' indicates die temperature is not over +142¢XC.

	DB0 T-FLT
*/
#define DAC7760_STATUS_T_FLT						((uint16_t)(1<<0))

typedef struct
{
    uint16_t CLRSEL;
    uint16_t OVR;
    uint16_t REXT;
    uint16_t OUTEN;	
    uint16_t SRCLK;
    uint16_t SRSTEP;
    uint16_t SREN;
    uint16_t DCEN;
    uint16_t RANGE;
}DAC7760_Control_InitTypeDef;

typedef struct
{
    uint16_t IOUTRANGE;
    uint16_t DUALOUTEN;
    uint16_t APD;
    uint16_t CALEN;		
    uint16_t HARTEN;
    uint16_t CRCEN;
    uint16_t WDEN;
    uint16_t WDPD;
}DAC7760_Config_InitTypeDef;

typedef struct
{
    DAC7760_Control_InitTypeDef Control;
    DAC7760_Config_InitTypeDef Config;

}DAC7760_Command_InitTypeDef;


/* Macro ---------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
void DAC7760_SPI_WriteResetRegister(uint16_t Data);
//void DAC7760_SPI_WriteControlRegister(uint16_t Data);
//void DAC7760_SPI_WriteConfigRegister(uint16_t Data);

void DAC7760_SPI_WriteControlRegister(uint8_t Address , uint16_t Data);
void DAC7760_SPI_WriteConfigRegister(uint8_t Address , uint16_t Data);

void DAC7760_SPI_WriteDataRegister(uint16_t Data);
void DAC7760_SPI_WriteGainRegister(uint16_t Data);
void DAC7760_SPI_WriteZeroRegister(uint16_t Data);
void DAC7760_SPI_WriteRegister(uint8_t Address , uint16_t Data);
uint16_t DAC7760_SPI_ReadControlRegister(void);
uint16_t DAC7760_SPI_ReadConfigRegister(void);
uint16_t DAC7760_SPI_ReadStatusRegister(void);
uint16_t DAC7760_SPI_ReadDataRegister(void);
uint16_t DAC7760_SPI_ReadGainRegister(void);
uint16_t DAC7760_SPI_ReadZeroRegister(void);
uint16_t DAC7760_SPI_ReadRegister(uint8_t Address);

void DAC7760_SPI_Read(uint8_t *data1,uint8_t *data2,uint8_t *data3);
void DAC7760_SPI_Write(uint8_t cmd,uint8_t data1,uint8_t data2);

void DAC7760_SPI_InitialSetting(void);

void DAC7760_SPI_Command_Example(void);
void DAC7760_SPI_Command_Template(void);

void DAC7760_SPI_Config(void);

/* Exported constants --------------------------------------------------------*/

#endif  /* __SPI_DAC7760_H */

