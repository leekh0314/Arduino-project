#!/usr/bin/env python3

#RTrobot CCS811 Test
#http://rtrobot.org

import time
import fcntl
import array
import math

I2C_SLAVE=0x0703

class RTrobot_CCS811:
	CCS811_I2CADDR		=	(0x5A)
	CCS811_STATUS		=	(0x00)
	CCS811_MEAS_MODE        =  	(0x01)
	CCS811_ALG_RESULT_DATA	=	(0x02)
	CCS811_RAW_DATA		=	(0x03)
	CCS811_ENV_DATA		=	(0X05)
	CCS811_NTC		=	(0X06)
	CCS811_THRESHOLDS	=	(0X10)
	CCS811_BASELINE		=	(0X11)
	CCS811_HW_ID		=	(0X20)
	CCS811_HW_ID_COD	=	(0x81)
	CCS811_HW_VERSIO	=	(0X21)
	CCS811_FW_BOOT_VERSION	=	(0X23)
	CCS811_FW_APP_VERSION	=	(0X24)
	CCS811_ERROR_ID		=	(0xEA)
	CCS811_APP_ERASE	=	(0xF1)
	CCS811_APP_DATA 	=	(0xF2)
	CCS811_APP_VERIFY	=	(0xF3)
	CCS811_APP_START	=	(0xF4)
	CCS811_SW_RESET		=	(0xFF)

	CCS811_MEAS_DRIVEMODE_IDLE	=	(0x00)
	CCS811_MEAS_DRIVEMODE_1SEC	=	(0x10)
	CCS811_MEAS_DRIVEMODE_10SEC	=	(0x20)
	CCS811_MEAS_DRIVEMODE_60SEC	=	(0x30)
	CCS811_MEAS_DRIVEMODE_250MS	=	(0x40)

	CCS811_MEAS_INTERRUPT_DISABLE	=	(0xF7)
	CCS811_MEAS_INTERRUPT_ENABLE	=	(0X08)

	CCS811_MEAS_THRESH_DISABLE	=	(0xFB)
	CCS811_MEAS_THRESH_ENABLE	=	(0x04)

	CCS811_tempoffset	=	(0.0)

	def __init__(self, i2c_no=1 ,i2c_addr=CCS811_I2CADDR):
		global CCS811_rb , CCS811_wb
		CCS811_rb = open("/dev/i2c-"+str(i2c_no),"rb",buffering=0)
		CCS811_wb = open("/dev/i2c-"+str(i2c_no),"wb",buffering=0)
		fcntl.ioctl(CCS811_rb, I2C_SLAVE, i2c_addr)
		fcntl.ioctl(CCS811_wb, I2C_SLAVE, i2c_addr)
		RTrobot_CCS811.CCS811_SoftRest(self)
		time.sleep(0.015)

	#CCS811 SoftRest
	def CCS811_SoftRest(self):
		buf = [RTrobot_CCS811.CCS811_SW_RESET, 0x11, 0xE5, 0x72, 0x8A]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)

	#CCS811 Initialization
	def CCS811_Init(self):
		#Read HW ID
		buf = [RTrobot_CCS811.CCS811_HW_ID]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		tmp = CCS811_rb.read(1)
		data = array.array('B' , tmp)
		if data[0] != RTrobot_CCS811.CCS811_HW_ID_COD:
			return False
		#Start application mode
		buf = [RTrobot_CCS811.CCS811_APP_START]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		time.sleep(0.001)
		buf = [RTrobot_CCS811.CCS811_STATUS]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		tmp = CCS811_rb.read(1)
		data = array.array('B' , tmp)
		if (data[0]>>7)&0x01 != 1 :
			return False

		#Set MEAS mode:1 second mode| disable interrupt | normally
		buf = [RTrobot_CCS811.CCS811_MEAS_MODE, RTrobot_CCS811.CCS811_MEAS_DRIVEMODE_1SEC]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		RTrobot_CCS811.CCS811_tempoffset=0.0
		return True

	#CCS811 Data Ready Test
	def CCS811_DataReady(self):
		buf = [RTrobot_CCS811.CCS811_STATUS]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		tmp = CCS811_rb.read(1)
		data = array.array('B' , tmp)
		if (data[0]>>3)&0x01 == 1:
			return True
		else:
			return False

	#CCS811's Temperature
	def CCS811_Temperature(self):
		buf = [RTrobot_CCS811.CCS811_NTC]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		tmp = CCS811_rb.read(4)
		data = array.array('B' , tmp)

		vref = (data[0]<<8)|data[1]
		vntc = (data[2]<<8)|data[3]
		rntc = vntc * 100000.0 / vref
		temp = math.log(rntc / 100000.0)
		temp /= 3380.0
		temp += 1.0 / (25.0 + 273.15)
		temp = 1.0 / temp
		temp -= 273.15
		return temp-RTrobot_CCS811.CCS811_tempoffset

	#CCS811 Read Result
	def CCS811_ReadResult(self):
		result=[0,0]
		if RTrobot_CCS811.CCS811_DataReady(self)==False:
			return False
		buf = [RTrobot_CCS811.CCS811_ALG_RESULT_DATA]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		tmp = CCS811_rb.read(4)
		data = array.array('B' , tmp)
		
		result[0] = data[0]<<8|data[1]
		result[1] = data[2]<<8|data[3]
		return result

	#CCS811 Read Result
	def CCS811_Settempoffset(self, temp):
		RTrobot_CCS811.CCS811_tempoffset=temp

	#CCS811 Set Interrupts mode
	def CCS811_SetInterrupts(self,flag):
		buf = [RTrobot_CCS811.CCS811_MEAS_MODE]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		tmp = CCS811_rb.read(1)
		data = array.array('B' , tmp)
		if flag == True:
			data[0] = data[0] | RTrobot_CCS811.CCS811_MEAS_INTERRUPT_ENABLE
		else :
			data[0] = data[0] & RTrobot_CCS811.CCS811_MEAS_INTERRUPT_DISABLE
		buf = [RTrobot_CCS811.CCS811_MEAS_MODE , data ]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)

	#CCS811 Set Thresh mode
	def CCS811_SetThresh(self, flag, low, high, hysteresis):
		buf = [RTrobot_CCS811.CCS811_MEAS_MODE]
		buf_binary = bytearray(buf)
		CCS811_wb.write(buf_binary)
		tmp = CCS811_rb.read(1)
		data = array.array('B' , tmp)
		if flag == True:
			data[0] = data[0] | RTrobot_CCS811.CCS811_MEAS_THRESH_ENABLE
			buf=[RTrobot_CCS811.CCS811_MEAS_MODE , data ]
			buf_binary = bytearray(buf)
			CCS811_wb.write(buf_binary)
			buf=[low,high,hysteresis]
			buf_binary = bytearray(buf)
			CCS811_wb.write(buf_binary)
		else :
			data[0] = data[0] & RTrobot_CCS811.CCS811_MEAS_THRESH_DISABLE
			buf=[RTrobot_CCS811.CCS811_MEAS_MODE , data ]
			buf_binary = bytearray(buf)
			CCS811_wb.write(buf_binary)
