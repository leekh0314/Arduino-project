/*****************************************************************************
RTrobot.org
禁止用于商业行为                              
*****************************************************************************/
#include <math.h>
/*****************************************************************************
定义类型及变量
*****************************************************************************/

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

unsigned short CCS811_tempoffset=0;

/***************************************************************************************************************
CCS811 SoftRest
****************************************************************************************************************/
void CCS811_SoftRest(void)
{
    unsigned char buf[4] = {0x11, 0xE5, 0x72, 0x8A};
	Single_WriteI2C(CCS811_SW_RESET, buf, 4);
}

/***************************************************************************************************************
CCS811 Data Ready Test
****************************************************************************************************************/
unsigned char CCS811_DataReady(void)
{
    unsigned char buf[1];
    Single_ReadI2C(CCS811_STATUS ,buf ,1);
	return CCS811_STATUS_DATA_READY(buf[0]);
}

/***************************************************************************************************************
CCS811's Temperature
****************************************************************************************************************/
float CCS811_Temperature(void)
{
    unsigned char buf[4];
    unsigned short int vref;
    unsigned short int vntc;
    unsigned long int rntc;
    double temp;
    
    Single_ReadI2C(CCS811_NTC ,buf ,4);
    
	vref = (unsigned short int)buf[0]<<8|buf[1];
    
	vntc = (unsigned short int)buf[2]<<8|buf[3];
	
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
int CCS811_ReadResult(int* eCO2, int* TVOC)
{
    unsigned char buf[4];
    
    if(!CCS811_DataReady())
		return -1;
        
    Single_ReadI2C(CCS811_ALG_RESULT_DATA ,buf ,4);
    
	*eCO2 = (int)buf[0]<<8|buf[1];
	*TVOC = (int)buf[2]<<8|buf[3];
	
	return 0;
}

/*****************************************************************************
CCS811初始化 
/****************************************************************************/
int CCS811_Init(unsigned char i2c_addr)
{
    unsigned char buf[1];
    SlaveAddress=i2c_addr;
    
    
    //soft reset
    CCS811_SoftRest();
    
     Delay_Ms(1);
    
	//Read HW ID
    Single_ReadI2C(CCS811_HW_ID ,buf ,1);
	if( buf[0]!= CCS811_HW_ID_CODE)
        return -1;
    
    //Start application mode
	Single_WriteI2C(CCS811_APP_START,0,0);
	Delay_Ms(1);
    
    buf[0]=0;
    Single_ReadI2C(CCS811_STATUS ,buf ,1);
	
	if(!CCS811_STATUS_FW_MODE(buf[0]))
		return -1;
        
    //Set MEAS mode:1 second mode| disable interrupt | normally
	buf[0] = CCS811_MEAS_DRIVEMODE_1SEC;
	Single_WriteI2C(CCS811_MEAS_MODE, buf, 1);
        
    CCS811_tempoffset=0;
    return 0;
}
