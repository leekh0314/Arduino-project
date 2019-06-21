/*
 * dalye.h:
 * Copyright (c) 2014-2017 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"

void TimingDelay_Decrement(void);
void DELAY_MS(__IO u32 Time);
void SysTick_Init(void);
void DELAY_US(__IO u32 Time);
#endif
