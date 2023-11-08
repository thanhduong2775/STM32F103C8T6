#include "stm32f10x.h"                  // Device header
#include "gpio_drive.h"
#include "systick_time.h"


/*
SPI 1 pin

PA4 SS
PA5 SCK
PA6 MISO
PA7 MOSI
*/

int main (void)
{
	systick_init();
	//Active SPI-1 peripheral/AF function
	RCC->APB2ENR |= 1;// Enable AF
	RCC->APB2ENR |= 0x1000;//Enable SPI - 1
	
	//Set up the pin
	init_GP(PA,4,OUT50, O_GP_PP);
	init_GP(PA,5,OUT50,O_AF_PP);
	init_GP(PA,6,IN, I_PP);
	init_GP(PA,7,OUT50,O_AF_PP);
	
	// Setup SPI register
	SPI1->CR1 |= 0x04;//Master mode
	SPI1->CR1 |= 0x38;//Fcl/256
	SPI1->CR1 |= 0x40;//SPI enable
	SPI1->CR2 |= 0x04;//Enable SS 
	W_GP(PA, 4, HIGH);
	while(1)
	{
		DelayMs(50);
		W_GP(PA, 4, LOW);
		SPI1->DR = 0x55;
		while(SPI1->SR & 0x80);
		W_GP(PA, 4, HIGH);
	}
	
}

