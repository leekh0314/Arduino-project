/*
 * delay.c:
 * Copyright (c) 2014-2018 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 * use keil5 for arm.
 */
#include "delay.h"
static __IO u32 TimingDelay;

void SysTick_Init(void)
{	
	while(SysTick_Config(SystemCoreClock/1000000));	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;	
}

void DELAY_US(__IO u32 Time)
{ 
	while(SysTick_Config(SystemCoreClock/1000000));	
	TimingDelay = Time;	
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
}

void DELAY_MS(__IO u32 Time)
{ 
	while(SysTick_Config(SystemCoreClock/1000));	
	TimingDelay = Time;	
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;		
	while(TimingDelay != 0);	
}


void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{ 
		TimingDelay--;
	}
}
