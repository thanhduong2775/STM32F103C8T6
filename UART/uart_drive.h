#include "stm32f10x.h" 
#include "gpio_drive.h"
void UART_init (unsigned short usart, unsigned long BR);
unsigned long USART_BRR(unsigned short usart, unsigned long BR);

char UART_RX( unsigned short uart);         /// Receiving function
void UART_TX( unsigned short uart, char c); /// Transfering function

void UART_ISR(unsigned short uart, unsigned short uart_2_mgr[], char str[]);
void UART_SEND(unsigned short uart, char str[]);
void str_empty(char str[]);
void systick_int_interrupt(unsigned short uart_1_mgr[],unsigned short uart_2_mgr[],unsigned short uart_3_mgr[]);