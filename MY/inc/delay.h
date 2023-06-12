#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);	  //��λ10us
void TimingDelay_Decrement(void);
#define Delay_ms(x) Delay_us(1000*x)//��λms

#endif 
/*----------------------�·��� ������̳��www.doflye.net--------------------------*/
