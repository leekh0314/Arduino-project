#!/usr/bin/env python3

#RTrobot SHT3XD Sensor Test
#http://rtrobot.org

import RTrobot_CCS811
import sys
import time

ccs = RTrobot_CCS811.RTrobot_CCS811()
buf = ccs.CCS811_Init()
if buf==False:
	print("CCS811 initialize error.")
else :
	print("CCS811 initialize register finished.")

time.sleep(1)
while (ccs.CCS811_DataReady()==False):pass

temp = ccs.CCS811_Temperature()
ccs.CCS811_Settempoffset(temp - 25.0)

#ccs.CCS811_SetInterrupts(False)

while True:
	if ccs.CCS811_DataReady()==True:
		result=ccs.CCS811_ReadResult()
		temperature=ccs.CCS811_Temperature()
		if result != False:
			print("CO2:%dppm\r\nTVOC:%dppb\r\nTemperature:%.2fC" %(result[0],result[1],temperature))
	else:
		time.sleep(0.5)
