#include "CCS811.h"
#if defined(__AVR__)
  #include <util/delay.h>
#endif

CCS811::CCS811() 
{
}

boolean CCS811::begin(uint8_t addr) {

	uint8_t buf[1];

	_i2caddr = addr;

	Wire.begin();
  
	Serial.println("INIT SENSOR...");

	//Software Reset
	SoftRest();

	delay(1);
  
	//Read HW ID
	if(read8(CCS811_HW_ID) != CCS811_HW_ID_CODE)
		return false;
	
	//Start application mode
	writeCommand(CCS811_APP_START,NULL,0);
	delay(1);
	
	if(!CCS811_STATUS_FW_MODE(read8(CCS811_STATUS)))
		return false;
	
	//Set MEAS mode:1 second mode| disable interrupt | normally
	buf[0] = CCS811_MEAS_DRIVEMODE_1SEC;
	writeCommand(CCS811_MEAS_MODE,buf,1);
  
	Serial.println("Found the CCS811");
	tempoffset=0;
	return true;
}

void CCS811::SoftRest()
{
	uint8_t buf[] = {0x11, 0xE5, 0x72, 0x8A};
	writeCommand(CCS811_SW_RESET, buf, 4);
}

uint8_t CCS811::DataReady()
{
	return CCS811_STATUS_DATA_READY(read8(CCS811_STATUS));
}

void CCS811::SetInterrupts( boolean flag )
{
	uint8_t buf[1];
	buf[0] = read8(CCS811_MEAS_MODE);
	
	if(flag==true)
		buf[0] = buf[0] | CCS811_MEAS_INTERRUPT_ENABLE;
	
	if(flag==false)
		buf[0] = buf[0] & CCS811_MEAS_INTERRUPT_DISABLE;

	writeCommand(CCS811_MEAS_MODE,buf,1);
}

void CCS811::SetThresh( boolean flag, uint16_t low, uint16_t high, uint8_t hysteresis )
{
	uint8_t buf[1];
	buf[0] = read8(CCS811_MEAS_MODE);
	
	if(flag==true)
	{
		buf[0] = buf[0] | CCS811_MEAS_THRESH_ENABLE;
		writeCommand(CCS811_MEAS_MODE,buf,1);
		
		uint8_t tmp[] = {	(uint8_t)((low >> 8) & 0xF), \
							(uint8_t)(low & 0xF),\
							(uint8_t)((high >> 8) & 0xF),\
							(uint8_t)(high & 0xF), \
							hysteresis};
		writeCommand(CCS811_THRESHOLDS, tmp, 5);
	}
	
	if(flag==false)
	{
		buf[0] = buf[0] & CCS811_MEAS_THRESH_DISABLE;
		writeCommand(CCS811_MEAS_MODE,buf,1);
	}
}

boolean CCS811::SetEnvironmental(float temperature, float humidity)
{
	if((temperature < -25)||(temperature > 50)) 
		return false;
	
	if((humidity < 0)||(humidity > 100))
		return false;
	
	uint32_t hum = humidity * 1000;
	uint32_t temp = temperature * 1000;
	
	
	uint8_t buf[4];
	
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

	writeCommand(CCS811_ENV_DATA, buf, 4);
	
	return true;
}

double CCS811::Temperature()
{
	uint32_t buf = read32(CCS811_NTC);
	uint16_t vref = buf >>16;
	uint16_t vntc = buf & 0xFFFF;
	
	//RNTC = VNTC * RREF(10KΩ) / VREF
	uint32_t rntc = vntc * 100000 / vref;
	
	double temp;
	temp = log((double)rntc / 100000);
	temp /= 3380;
	temp += 1.0 / (25 + 273.15);
	temp = 1.0 / temp;
	temp -= 273.15;
	return temp-tempoffset;
}

boolean CCS811::ReadResult(uint16_t* eCO2, uint16_t* TVOC)
{
	if(!DataReady())
		return false;

	uint32_t buf;
	buf=read32(CCS811_ALG_RESULT_DATA);
		
	*eCO2 = buf >>16;
	*TVOC = buf & 0xFFFF;
	
	return true;
}

uint8_t CCS811::read8(uint8_t reg) 
{
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    while (Wire.requestFrom(_i2caddr, (uint8_t)1) != 1);
    return ( Wire.read());
}

uint16_t CCS811::read16(uint8_t reg)
{
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    while (Wire.requestFrom(_i2caddr, (uint8_t)2) != 2);
	uint32_t buf = Wire.read();
	buf <<= 8;
	buf |= Wire.read();
    return buf;
}

uint32_t CCS811::read32(uint8_t reg)
{
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    while (Wire.requestFrom(_i2caddr, (uint8_t)4) != 4);
	uint32_t buf = Wire.read();
	buf <<= 8;
	buf |= Wire.read();
	buf <<= 8;
	buf |= Wire.read();
	buf <<= 8;
	buf |= Wire.read();
    return buf;
}

void CCS811::writeCommand(uint8_t reg, uint8_t *cmd, uint8_t length)
{
	Wire.beginTransmission(_i2caddr);
	Wire.write(reg);
	Wire.write(cmd,length);
	Wire.endTransmission();
}
