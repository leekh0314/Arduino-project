/*****************************************************************************
编写人：Yayi　         
网店:rtrobot.taobao.com                                       
*****************************************************************************/
// 包含头文件
#include <stc12c5a60s2.h>
#include <INTRINS.H>
#include <Delay.h>
#include <IIC.h>
#include <ccs811.h>
#include <UART.h>
#include <stdio.h>

/*****************************************************************************
//主程序
*****************************************************************************/
void main()
{
    int result;
    float temperature;
    //Delay_Ms(500);
	Uart_Init();	
    //初始化CCS811
    result=CCS811_Init(0x5A);
    
    if(result==-1)
        Send_String("CS811 initialize error!\r\n");
    else
        Send_String("CS811 initialize ok!\r\n");
        
    while(!CCS811_DataReady());
    temperature=CCS811_Temperature();
    CCS811_tempoffset=temperature - 25.0;
    
    while(1)
    {
        if(CCS811_DataReady())
		{
			unsigned char error;
			int eCO2,TVOC;
			
			error=CCS811_ReadResult(&eCO2,&TVOC);
            
            if(error==-1)
				printf("Error.\r\n"); 
            else
            {
                printf("CO2:%dppm\r\n",eCO2);
                printf("TVOC:%dppb\r\n",TVOC);
                temperature=CCS811_Temperature();
                printf("Temperature:%.2fC\r\n",temperature);
            }
        }
        Delay_Ms(500);
    }
}
