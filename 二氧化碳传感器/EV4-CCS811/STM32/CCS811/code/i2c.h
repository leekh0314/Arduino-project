/*
 * i2c.h:
 * Copyright (c) 2014-2017 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
#ifndef __I2C_H_
#define __I2C_H_

#include "main.h"
#include "stm32f10x.h"

#define SDA_GPIO    GPIO_Pin_11
#define SDA_PORT    GPIOB
#define SDA_RCC     RCC_APB2Periph_GPIOB
#define SCL_GPIO    GPIO_Pin_10
#define SCL_PORT    GPIOB
#define SCL_RCC     RCC_APB2Periph_GPIOB

void I2C_Config(void);
void SDA_SetGpioMode(bool mode);
static void IIC_delay(uint16_t n);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(uint8_t data);
uint8_t IIC_RecvByte(void);
int8_t IIC_RecvACK(void);
void IIC_SendACK(uint8_t ack);
void IIC_WriteReg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *pdata, uint8_t len);
void IIC_ReadData(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *pdata, uint8_t len);

#endif /* __I2C_H */
