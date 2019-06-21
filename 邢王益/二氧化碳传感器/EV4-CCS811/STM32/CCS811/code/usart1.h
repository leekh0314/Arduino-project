/*
 * usart.h:
 * Copyright (c) 2014-2017 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
 
#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"

void Usart1_Config(void);
void Nvic_Usart1_Config(void);
void Usart1_Send(unsigned char *str);


#endif /* __USART1_H */
