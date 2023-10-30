#include "stm32f10x.h"                  // Device header
#include "systick_time.h"
#include "uart_drive.h"
void systick_init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL |= 5; /// 101-> Souce clock 1: AHB, 0: no exception request, 1: Enable
	
}

void DelayMillis(void)
{
	SysTick->LOAD = 0x1193F;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0); /// AND count flag bit 
}
void DelayMicros(void)
{
	SysTick->LOAD = 0x47;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0); /// AND count flag bit 
}

void DelayMs(unsigned long t)
{
	for(;t>0;t--)
		{
			DelayMillis();
		}
}

void Delay_us(unsigned long t)
{
	for(;t>0;t--)
		{
			DelayMicros();
		}
}


void systick_int_start(void)
{
	__disable_irq();
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x1193F; // 72000-1: 1 ms
	SysTick->VAL = 0;
	SysTick->CTRL |= 7; /// 111-> Souce clock 1: AHB, 1: have exception request, 1: Enable
	__enable_irq();
}
void systick_int_interrupt(unsigned short uart_1_mgr[],unsigned short uart_2_mgr[],unsigned short uart_3_mgr[])
{
	if(uart_1_mgr[0]!=0)
	{
		if(uart_1_mgr[6]==0)
		{
			uart_1_mgr[0]=0;
			uart_1_mgr[1]=1;
			systick_init();
		}
		else
		{
			uart_1_mgr[6]--;
		}
	}
	else if(uart_2_mgr[0]!=0)
	{
		if(uart_2_mgr[6]==0)
		{
			uart_2_mgr[0]=0;
			uart_2_mgr[1]=1;
			systick_init();
		}
		else
		{
			uart_2_mgr[6]--;
		}
	}
	else if(uart_3_mgr[0]!=0)
	{
		if(uart_3_mgr[6]==0)
		{
			uart_3_mgr[0]=0;
			uart_3_mgr[1]=1;
			systick_init();
		}
		else
		{
			uart_3_mgr[6]--;
		}
	}

}

