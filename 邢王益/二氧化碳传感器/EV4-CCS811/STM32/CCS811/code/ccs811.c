/*
 * ccs811.c:
 * Copyright (c) 2014-2018 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
#include "ccs811.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

uint8_t CCS811_I2CADDR;
uint8_t CCS811_tempoffset;

/***************************************************************************************************************
CCS811 Initialization
****************************************************************************************************************/
bool CCS811_Init(uint8_t i2c_addr)
{
    uint8_t buf[1];
    CCS811_I2CADDR=i2c_addr;
    
    //soft reset
    CCS811_SoftRest();
    
    DELAY_MS(1);
    
	//Read HW ID
    IIC_ReadData(CCS811_I2CADDR, CCS811_HW_ID ,buf ,1);
	if( buf[0]!= CCS811_HW_ID_CODE)
		return false;
        
    //Start application mode
	IIC_WriteReg(CCS811_I2CADDR,CCS811_APP_START,NULL,0);
	DELAY_MS(1);
    
    buf[0]=0;
    IIC_ReadData(CCS811_I2CADDR ,CCS811_STATUS ,buf ,1);
	
	if(!CCS811_STATUS_FW_MODE(buf[0]))
		return false;
        
    //Set MEAS mode:1 second mode| disable interrupt | normally
	buf[0] = CCS811_MEAS_DRIVEMODE_1SEC;
	IIC_WriteReg(CCS811_I2CADDR, CCS811_MEAS_MODE, buf, 1);
        
    Usart1_Send("CCS811 initialize register finished.\r\n");
    CCS811_tempoffset=0;
    return true;
}

/***************************************************************************************************************
CCS811 SoftRest
****************************************************************************************************************/
void CCS811_SoftRest(void)
{
    uint8_t buf[4] = {0x11, 0xE5, 0x72, 0x8A};
	IIC_WriteReg(CCS811_I2CADDR, CCS811_SW_RESET, buf, 4);
}

/***************************************************************************************************************
CCS811 Data Ready Test
****************************************************************************************************************/
uint8_t CCS811_DataReady(void)
{
    uint8_t buf[1];
    IIC_ReadData(CCS811_I2CADDR ,CCS811_STATUS ,buf ,1);
	return CCS811_STATUS_DATA_READY(buf[0]);
}

/***************************************************************************************************************
CCS811's Temperature
****************************************************************************************************************/
double CCS811_Temperature(void)
{
    uint8_t buf[4];
    uint16_t vref;
    uint16_t vntc;
    uint32_t rntc;
    double temp;
    
    IIC_ReadData(CCS811_I2CADDR ,CCS811_NTC ,buf ,4);
    
	vref = (uint16_t)buf[0]<<8|buf[1];
    
	vntc = (uint16_t)buf[2]<<8|buf[3];
	
	//RNTC = VNTC * RREF(10KO) / VREF
	rntc = vntc * 100000 / vref;
	
	temp = log((double)rntc / 100000);
	//temp /= 3380;
	temp /= 3950;
	temp += 1.0 / (25 + 273.15);
	temp = 1.0 / temp;
	temp -= 273.15;
	return temp-CCS811_tempoffset;
}

/***************************************************************************************************************
CCS811 Read Result
****************************************************************************************************************/
bool CCS811_ReadResult(uint16_t* eCO2, uint16_t* TVOC)
{
    uint8_t buf[4];
    
	if(!CCS811_DataReady())
		return false;
        
    IIC_ReadData(CCS811_I2CADDR ,CCS811_ALG_RESULT_DATA ,buf ,4);
		
	*eCO2 = ((uint16_t)buf[0])<<8|buf[1];
	*TVOC = (uint16_t)buf[2]<<8|buf[3];
	
	return true;
}

/***************************************************************************************************************
CCS811 Set Interrupts mode
****************************************************************************************************************/
void CCS811_SetInterrupts( bool flag )
{
	uint8_t buf[1];
    
    IIC_ReadData(CCS811_I2CADDR ,CCS811_MEAS_MODE ,buf ,1);
	
	if(flag==true)
		buf[0] = buf[0] | CCS811_MEAS_INTERRUPT_ENABLE;
	
	if(flag==false)
		buf[0] = buf[0] & CCS811_MEAS_INTERRUPT_DISABLE;

    IIC_WriteReg(CCS811_I2CADDR, CCS811_MEAS_MODE, buf, 1);
}

/***************************************************************************************************************
CCS811 Set Thresh mode
****************************************************************************************************************/
void CCS811_SetThresh( bool flag, uint16_t low, uint16_t high, uint8_t hysteresis )
{
	uint8_t buf[1];
	IIC_ReadData(CCS811_I2CADDR ,CCS811_MEAS_MODE ,buf ,1);
	
	if(flag==true)
	{
        uint8_t tmp[5];
        tmp[0]=(uint8_t)((low >> 8) & 0xF);
        tmp[1]=(uint8_t)(low & 0xF);
        tmp[2]=(uint8_t)((high >> 8) & 0xF);
        tmp[3]=(uint8_t)(high & 0xF);
        tmp[4]=hysteresis;
                            
		buf[0] = buf[0] | CCS811_MEAS_THRESH_ENABLE;
		IIC_WriteReg(CCS811_I2CADDR, CCS811_MEAS_MODE, buf, 1);
        IIC_WriteReg(CCS811_I2CADDR, CCS811_THRESHOLDS, tmp, 5);
	}
	
	if(flag==false)
	{
		buf[0] = buf[0] & CCS811_MEAS_THRESH_DISABLE;
		IIC_WriteReg(CCS811_I2CADDR, CCS811_MEAS_MODE, buf, 1);
	}
}

/***************************************************************************************************************
CCS811 Set Environmental
****************************************************************************************************************/
bool CCS811_SetEnvironmental(float temperature, float humidity)
{
    uint32_t hum;
    uint32_t temp;
    uint8_t buf[4];
    
	if((temperature < -25)||(temperature > 50)) 
		return false;
	
	if((humidity < 0)||(humidity > 100))
		return false;
	
	hum = humidity * 1000;
	temp = temperature * 1000;
	
	if(((hum%1000)/100)>7)
		buf[0]=(hum / 1000 + 1) << 1;
	else
		buf[0]=(hum / 1000) << 1;
	buf[1]=0;
	if (((hum % 1000) / 100) > 2 && (((hum % 1000) / 100) < 8))
		buf[0] |= 1;
	
	temp += 25000;
	if(((temp%100)/100)>7)
		buf[2]=(hum / 1000 + 1) << 1;
	else
		buf[0]=(hum / 1000) << 1;
	buf[3]=0;
	if (((temp % 1000) / 100) > 2 && (((temp % 1000) / 100) < 8))
		buf[2] |= 1;

	IIC_WriteReg(CCS811_I2CADDR, CCS811_ENV_DATA, buf, 4);
	
	return true;
}
