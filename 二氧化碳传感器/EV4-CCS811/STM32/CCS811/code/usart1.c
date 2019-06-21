/*
 * usart1.c:
 * Copyright (c) 2014-2018 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
 
#include "usart1.h"
#include "stm32f10x.h"

void Usart1_Send(unsigned char *str)
{
	while(*str != '\0')
	{		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		USART_SendData(USART1 , *str);
		str++;
	}
}

void Usart1_Config(void)
{		
	GPIO_InitTypeDef GPIO_Uart;
	USART_InitTypeDef USART_Uart;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	
	//TX
 	GPIO_Uart.GPIO_Pin = GPIO_Pin_9 ;
  	GPIO_Uart.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Uart.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_Uart);
	
	//RX
	GPIO_Uart.GPIO_Pin = GPIO_Pin_10;
	GPIO_Uart.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_Uart);
	
	USART_Uart.USART_BaudRate = 9600;
	USART_Uart.USART_WordLength = USART_WordLength_8b;
	USART_Uart.USART_StopBits = USART_StopBits_1;
	USART_Uart.USART_Parity = USART_Parity_No ;
	USART_Uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_Uart); 
	USART_Cmd(USART1, ENABLE);
}

void Nvic_Usart1_Config(void)
{
	NVIC_InitTypeDef NVIC_Uart1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
	NVIC_Uart1.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Uart1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Uart1.NVIC_IRQChannelSubPriority = 1;
	NVIC_Uart1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Uart1);
}

void USART1_IRQHandler(void)
{
		//NULL
}
