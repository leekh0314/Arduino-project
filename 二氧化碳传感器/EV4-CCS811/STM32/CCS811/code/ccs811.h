/*
 * ccs811.h:
 * Copyright (c) 2014-2018 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
#ifndef __CCS811_H_
#define __CCS811_H_

#include "main.h"
#include "stm32f10x.h"
#include "i2c.h"
#include "delay.h"
#include "usart1.h"

#define CCS811_STATUS       		0x00
#define CCS811_MEAS_MODE          	0x01
#define CCS811_ALG_RESULT_DATA		0x02
#define CCS811_RAW_DATA				0x03
#define CCS811_ENV_DATA				0X05
#define CCS811_NTC					0X06
#define CCS811_THRESHOLDS			0X10
#define CCS811_BASELINE				0X11
#define CCS811_HW_ID				0X20
#define CCS811_HW_ID_CODE			0x81
#define CCS811_HW_VERSION			0X21
#define CCS811_FW_BOOT_VERSION		0X23
#define CCS811_FW_APP_VERSION		0X24
#define CCS811_ERROR_ID				0xEA
#define CCS811_APP_ERASE			0xF1
#define CCS811_APP_DATA  			0xF2
#define CCS811_APP_VERIFY			0xF3
#define CCS811_APP_START  			0xF4
#define CCS811_SW_RESET				0xFF

#define CCS811_STATUS_FW_MODE(x)	(x>>7)&0x01
#define CCS811_STATUS_APP_VALID(x)	(x>>4)&0x01
#define CCS811_STATUS_DATA_READY(x)	(x>>3)&0x01
#define CCS811_STATUS_ERROR(x)		x&&0x01

#define CCS811_MEAS_DRIVEMODE_IDLE	0x00
#define CCS811_MEAS_DRIVEMODE_1SEC	0x10
#define CCS811_MEAS_DRIVEMODE_10SEC	0x20
#define CCS811_MEAS_DRIVEMODE_60SEC	0x30
#define CCS811_MEAS_DRIVEMODE_250MS	0x40

#define	CCS811_MEAS_INTERRUPT_DISABLE	0xF7
#define CCS811_MEAS_INTERRUPT_ENABLE	0X08

#define CCS811_MEAS_THRESH_DISABLE		0xFB
#define CCS811_MEAS_THRESH_ENABLE		0x04

extern uint8_t CCS811_tempoffset;

bool CCS811_Init(uint8_t i2c_addr);
void CCS811_SoftRest(void);
uint8_t CCS811_DataReady(void);
double CCS811_Temperature(void);
bool CCS811_ReadResult(uint16_t* eCO2, uint16_t* TVOC);
void CCS811_SetInterrupts( bool flag );
void CCS811_SetThresh( bool flag, uint16_t low, uint16_t high, uint8_t hysteresis );


#endif /* __CCS811_H_ */
