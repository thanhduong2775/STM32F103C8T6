#include "uart_drive.h"
#include "gpio_drive.h"
#include "systick_time.h"

void UART_init (unsigned short usart, unsigned long BR)
{
	/// If using USART1 clock speed 72 Mhz, else USART2,3 clock speed 36Mhz
	///USART1 PA9 (Tx) and PA10 (Rx)
	///USART2 PA2 (Tx) and PA3 (Rx)
	///USART3 PB10(Tx) and PB11(Rx)
	unsigned long BRR_Cal;
	BRR_Cal = USART_BRR(usart, BR);
	RCC->APB2ENR |= 1;        //1:Alternate Function I/O clock enabled
	if(usart ==1)
	{
		__disable_irq();
		RCC->APB2ENR |= 0x4000; // Enable UART1 in APB2
		init_GP(PA, 9, OUT50, O_AF_PP); 	// Tx
		init_GP(PA, 10, IN, I_PP);				// Rx
		
		USART1->BRR |= BRR_Cal; // 72 MHz/ 9600(bps) = 7500; setup  the baud rate for 9600
		USART1->CR1 |= 8; // Active Tx
		USART1->CR1 |= 4; // Active Rx
		USART1->CR1 |= 0x2000; // Active Enable
		
		USART1->CR1 |= 0x20; // bit5: RXNEIE interrupt enable 
		NVIC_EnableIRQ(USART1_IRQn);
		__enable_irq();
	}	
	
	else if(usart == 2)
	{
		__disable_irq();
		RCC->APB1ENR |= 0x20000; // Enable UART2 in APB1
		init_GP(PA, 2, OUT50, O_AF_PP); // Tx
		init_GP(PA, 3, IN, I_PP);				// Rx
		
		USART2->BRR = BRR_Cal; // 36 MHz/ 9600(bps) = 3750; setup  the baud rate for 9600
		USART2->CR1 |= 8; // Active Tx
		USART2->CR1 |= 4; // Active Rx
		USART2->CR1 |= 0x2000; // Active Enable	
		
		USART2->CR1 |= 0x20; // bit5: RXNEIE interrupt enable 
		NVIC_EnableIRQ(USART2_IRQn);
		__enable_irq();
	}
	 if(usart == 3)
	{
		__disable_irq();
		RCC->APB1ENR |= 0x40000 ; // Enable UART3 in APB1
		init_GP(PB, 10, OUT50, O_AF_PP); // Tx
		init_GP(PB, 11, IN, I_PP);			 // Rx
		
		USART3->BRR = BRR_Cal; // 36 MHz/ 9600(bps) = 3750; setup  the baud rate for 9600
		USART3->CR1 |= 8; // Active Tx
		USART3->CR1 |= 4; // Active Rx
		USART3->CR1 |= 0x2000; // Active Enable	
		
		USART3->CR1 |= 0x20; // bit5: RXNEIE interrupt enable 
		NVIC_EnableIRQ(USART3_IRQn);
		__enable_irq();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
unsigned long USART_BRR(unsigned short usart, unsigned long BR)
{
	unsigned long div = 36000000UL;
	unsigned long dec;
	unsigned long final;
	double frac = 36000000.00;
	double frac2 = 1.00;
	
	if(usart == 1)
	{
	div = 72000000UL;
	frac = 72000000.00;
	}
	div = div / (BR*16);
	frac = 16*((frac / (BR*16))-div);
	dec = frac;
	frac2 = 100*(frac-dec);
	if(frac2>50)
	{
		dec ++;
		if(dec == 16)
		{
			dec = 0;
			div ++;
		}
	}
	
	final = (div<<4);
	final += dec;
	
	return final;
	
}

char UART_RX( unsigned short uart) /// Receiving function
{
	char c;
	if(uart ==1 )
	{
		while( (USART1->SR &= 0x20) == 0)
		{};                 /// waiting for reveived
		c = USART1->DR;
	}
		else if(uart ==2 )
	{
		while( (USART2->SR &= 0x20) == 0)
		{};                 /// waiting for reveived
		c = USART2->DR;
	}
	 if(uart ==3 )
	{
		while( (USART3->SR &= 0x20) == 0)
		{};                 /// waiting for reveived
		c = USART3->DR;
	}
	return c;
}
//////////////////////////////////////////////
void UART_TX( unsigned short uart, char c) /// Transfering function
{
	if(uart ==1 )
	{
		while ( (USART1->SR & (1<<6))==0x80)
		{}; // waiting for the next symbol written on SR
		USART1->DR = c; 
	}
		else if(uart ==2 )
	{
		while ( (USART2->SR & (1<<6))==0x80)
		{}; // waiting for the next symbol written on SR
		USART2->DR = c; 
	}
	 if(uart ==3 )
	{
		while ( (USART3->SR & (1<<6))==0x80)
		{}; // waiting for the next symbol written on SR
		USART3->DR = c; 
	}

}

/*
1- Define uart
2- Is it Process, Bridge or Both
If process: string to fulfil, signal
*/
void UART_ISR(unsigned short uart, unsigned short uart_mgr[], char str[])
{
	if(uart_mgr[2]==0)
	{
		str[uart_mgr[0]]=UART_RX(uart);
		if(uart_mgr[3])
		{
			if(str[uart_mgr[0]]==uart_mgr[4])
			{
				uart_mgr[0] = 0;
				uart_mgr[1] = 1;
			}
			else
			{
				uart_mgr[0]++;
			}
		}
		else
		{
			//Timer stratergy
			uart_mgr[0]++;
			uart_mgr[6]=uart_mgr[5];
			systick_int_start();
		}
			
	}
	else
	{
		UART_TX(uart_mgr[2], UART_RX(uart));
	}
}

void UART_SEND(unsigned short uart, char str[])
{
	int i = 0;
	while(str[i] != '\0')
	{
		UART_TX(uart, str[i]);
		i++;
		DelayMs(1);
	}
}

void UART_msg(unsigned short uart, char str[], unsigned short str_msg[])
{
	unsigned long timeOut=720000000;
	UART_SEND(uart,str);
	while(str_msg[1] == 0 & timeOut!=0)
	{}
	str_msg[1] = 0;
}

