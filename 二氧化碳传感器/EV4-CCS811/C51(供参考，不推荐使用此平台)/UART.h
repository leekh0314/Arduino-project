/*****************************************************************************
RTrobot.org
禁止用于商业行为                              
*****************************************************************************/

/*****************************************************************************
UART初始化
*****************************************************************************/
void Uart_Init(void)		
{
	EA=1;
	ES=1;
	SCON = 0x50;			 
	TMOD = 0x20;
	PCON=0x00; 	
	TH1 = 0xFD;	
	TL1 = 0xfd;
	TR1 = 1;	
	RI = 0;
}

/*****************************************************************************
单字符发送函数
/****************************************************************************/
void Send_Char (unsigned char UART_data)		
{ 		
	SBUF = UART_data;			
	while(TI == 0);			
	TI = 0;					  
}

/*****************************************************************************
字符串发送函数
/****************************************************************************/
void Send_String(unsigned char *str)		
{
	while(*str != '\0')		
	{
		Send_Char(*str);			
		*str=*str++;		
	}
	*str = 0;				  
}

//重写putchar函数
char putchar(char c)
{
    Send_Char(c);
    return c;
}
