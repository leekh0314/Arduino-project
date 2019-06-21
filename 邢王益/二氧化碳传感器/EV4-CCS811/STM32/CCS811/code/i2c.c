/*
 * i2c.c:
 * Copyright (c) 2014-2018 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
 
#include "i2c.h"
#include "delay.h"

#define SDA_HIGH GPIO_SetBits(SDA_PORT,SDA_GPIO)
#define SDA_LOW GPIO_ResetBits(SDA_PORT,SDA_GPIO)
#define SCL_HIGH GPIO_SetBits(SCL_PORT,SCL_GPIO)
#define SCL_LOW GPIO_ResetBits(SCL_PORT,SCL_GPIO)

#define SDA_INPUT GPIO_ReadInputDataBit(SDA_PORT,SDA_GPIO)

void I2C_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(SCL_RCC|SDA_RCC,ENABLE);
    
    //SDA
    GPIO_InitStructure.GPIO_Pin=SDA_GPIO;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(SDA_PORT,&GPIO_InitStructure);
    GPIO_SetBits(SDA_PORT,SDA_GPIO); 
    
    //SCL
    GPIO_InitStructure.GPIO_Pin=SCL_GPIO;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(SCL_PORT,&GPIO_InitStructure);
    GPIO_SetBits(SCL_PORT,SCL_GPIO);    
}

/***************************************************************************************************************
*switch SDA GPIO input or output
****************************************************************************************************************/
void SDA_SetGpioMode(bool mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin=SDA_GPIO;
    
    if(mode==1)
    {
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    }
    else
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(SDA_PORT,&GPIO_InitStructure);
}

/***************************************************************************************************************
*Simulated I2C delay
****************************************************************************************************************/
static void IIC_delay(uint16_t n)
{
    DELAY_US(30);
}


/***************************************************************************************************************
*Simulated I2C start
****************************************************************************************************************/
void IIC_Start(void)
{
    SDA_HIGH;                    
    SCL_HIGH;
    IIC_delay(1);
    SDA_LOW;
    IIC_delay(1);
    SCL_LOW;
}

/***************************************************************************************************************
*Simulated I2C STOP
****************************************************************************************************************/
void IIC_Stop(void)
{
    SCL_LOW;
    SDA_LOW;
    IIC_delay(1);
    SCL_HIGH;
    SDA_HIGH;
    IIC_delay(1);
}

/***************************************************************************************************************
* send one byte
****************************************************************************************************************/
void IIC_SendByte(uint8_t data)
{
    uint8_t i=0;
    for (i=0; i<8; i++)
    {
        if(data&0x80)
            SDA_HIGH;
        else
            SDA_LOW;
        SCL_HIGH;
        IIC_delay(1);
        SCL_LOW;
        IIC_delay(1);
        data<<=1;
    }
}

/***************************************************************************************************************
* receive one byte
****************************************************************************************************************/
uint8_t IIC_RecvByte(void)
{
    uint8_t i;
    uint8_t temp = 0;
    SDA_SetGpioMode(0);//set sda gpio input
    for (i=0; i<8; i++)
    {
        SCL_LOW;
        temp <<= 1;
        SCL_HIGH;
        IIC_delay(1);
        temp|= SDA_INPUT;
        SCL_LOW;
        IIC_delay(1);
    }
    SDA_SetGpioMode(1);//set sda gpio output 
    return temp;
}

/***************************************************************************************************************
* wait for a response
****************************************************************************************************************/
int8_t IIC_RecvACK(void)
{
    uint8_t wait=0xff;
    SDA_SetGpioMode(0);//set sda gpio input
    SCL_HIGH;
    while (SDA_INPUT&&wait--);
    if(wait<=0)
    {
        IIC_Stop();
        return -1;
    }
    SDA_INPUT;
    SCL_LOW;
    IIC_delay(1);
    SDA_SetGpioMode(1);//set sda gpio output
    return 0;
}

/***************************************************************************************************************
*response ACK; ack==1,response; ack==0,none; 
****************************************************************************************************************/
void IIC_SendACK(uint8_t ack)
{
    SCL_LOW;
    if (ack)
        SDA_HIGH;
    else
        SDA_LOW;   
    IIC_delay(1);
    SCL_HIGH;
    IIC_delay(1);                 
    SCL_LOW;             
}

/***************************************************************************************************************
* I2C write data
****************************************************************************************************************/
void IIC_WriteReg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *pdata, uint8_t len)
{
    IIC_Start();
    IIC_SendByte((i2c_addr<<1)|0x00);
    IIC_RecvACK(); 
    IIC_SendByte(reg_addr);
    IIC_RecvACK();
    while(len)
    {
        IIC_SendByte(*pdata);
        IIC_RecvACK();
        pdata++;
        len--;
    }
    IIC_Stop();
}

/***************************************************************************************************************
*receive len bytes to 'pdata'
****************************************************************************************************************/
void IIC_ReadData(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *pdata, uint8_t len)
{
    IIC_Start();
    IIC_SendByte((i2c_addr<<1)|0x00);
    IIC_RecvACK();
    IIC_SendByte(reg_addr);
    IIC_RecvACK();
    IIC_Start();
    DELAY_MS(15);
    IIC_SendByte((i2c_addr<<1)|0x01);
    IIC_RecvACK();
    while(len)
    {
        *pdata=IIC_RecvByte();
        if (len==1)
            IIC_SendACK(1);
        else
            IIC_SendACK(0);
        pdata++;    
        len--;
    }
    IIC_Stop();
}
