#include "stm32f10x.h"                  // Device header
#include "gpio_drive.h"
#include "systick_time.h"
#include "uart_drive.h"
char chat = 'C';

char mgs[30] = " Dit m e    m an  d  an   ";
/*
0 - count
1 - signal
2 - bridge
3 - Terminator should 1: Terminator / 0: Interrupt
4 - Terminator char
5 - Time constant
6 - Time counter
*/
/*
char USART_1_msg[250];
unsigned short USART_1_cnt = 0;
unsigned short USART_1_sig = 0;
unsigned short USART_1_bdg = 0;
*/
unsigned short uart_1_mgr[7]={0, 0, 0, 0, 0, 0 ,0};

char USART_2_msg[250];
unsigned short USART_2_cnt = 0;
unsigned short USART_2_sig = 0;
unsigned short USART_2_bdg = 0;
unsigned short uart_2_mgr[7]={0, 0, 0, 1, '0', 0 ,0};

char USART_3_msg[250];
unsigned short USART_3_cnt = 0;
unsigned short USART_3_sig = 0;
unsigned short USART_3_bdg = 0;
unsigned short uart_3_mgr[7]={0, 0, 0, 1, '0', 0 ,0};

int main(void)
{
	systick_init();
	UART_init(2, 256000);
	UART_init (3, 9600);
	DelayMs(1000);
	
	UART_SEND(2, mgs);
	UART_SEND(3, mgs);
	
	while(1)
	{
		if(uart_2_mgr[1] == 1)
		{
			UART_SEND(3, USART_2_msg);
			uart_2_mgr[1] = 0;
			str_empty(USART_2_msg);
		}
		if(uart_3_mgr[1] == 1)
		{
			UART_SEND(2, USART_3_msg);
			uart_3_mgr[1] = 0;
			str_empty(USART_3_msg);
		}
		
		
		//Transmit char
		//while ( (USART2->SR & (1<<6))==0x80)
		//{}; // waiting for the next symbol written on SR
		//USART2->DR = chat; 
	//	UART_TX(2, chat);
		
		// Reiceive char
		//	while ((USART2->SR & 0x20) == 0)
		//	{}; // waiting for the next symbol received 
		//	chat = USART2-> DR; // write the Data Register on variable chat
		
		//chat = UART_RX( 2 );	
	}
	
	
}


void USART2_IRQHandler()
{
	UART_ISR(2, uart_2_mgr, USART_2_msg); //Receiving data from uart 2 and Transfering data to uart3 
}

void USART3_IRQHandler()
{
	UART_ISR(3, uart_3_mgr, USART_3_msg); //Receiving data from uart 3 and Transfering data to uart2
}
void Systick_Handler(void)
{
	systick_int_interrupt(uart_1_mgr, uart_2_mgr, uart_3_mgr);
}




