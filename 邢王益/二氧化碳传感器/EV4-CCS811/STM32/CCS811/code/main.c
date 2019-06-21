/*
 * main.c:
 * Copyright (c) 2014-2018 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
 
//GND	GND
//VCC	VCC 
//SCL	GPIOB10
//SDA	GPIOB11
//WAKE  GND
 
#include "stm32f10x.h"
#include "main.h"
#include "i2c.h"
#include "usart1.h"
#include "delay.h"
#include "ccs811.h"
#include <stdio.h>

unsigned char outbuf[50];

int main(void)
{
    double temp;
    SysTick_Init();
    Usart1_Config();
    Nvic_Usart1_Config();
    I2C_Config();
    
    if(CCS811_Init(0x5A)==false)
        Usart1_Send("CCS811 initialize error.\r\n");
        
    while(!CCS811_DataReady());
    temp=CCS811_Temperature();
    CCS811_tempoffset=temp - 25.0;
    while(1)    
    {
		if(CCS811_DataReady())
		{
			float temperature;
			bool error;
			uint16_t eCO2,TVOC;
			
			error=CCS811_ReadResult(&eCO2,&TVOC);
			temperature=CCS811_Temperature();
	  
			if(error==false)
				Usart1_Send("Error.\r\n"); 
			else
			{
				sprintf((char *)outbuf,"CO2:%dppm\r\nTVOC:%dppb\r\nTemperature:%.2fC",eCO2,TVOC,temperature);
				Usart1_Send(outbuf);
			}
		}
        DELAY_MS(500);
    }
}
