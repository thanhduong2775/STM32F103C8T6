#include "stm32f10x.h"                  // Device header
#include "gpio_drive.h"
#include "spi_drive.h"
/*
SPI_1 pin
PA4 SS
PA5 SCK
PA6 MISO
PA7 MOSI


SPI_2 pin
PB12 SS
PB13 SCK
PB14 MISO
PB15 MOSI

*/
void spi_init(unsigned short spi)
{
	if(spi==1)
	{
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
	}
	else if(spi==2)
	{
	//Active SPI-1 peripheral/AF function
	RCC->APB1ENR |= 0x4000;// Enable SPI	
	//Set up the pin
	init_GP(PB,12,OUT50, O_GP_PP);
	init_GP(PB,13,OUT50,O_AF_PP);
	init_GP(PB,14,IN, I_PP);
	init_GP(PB,15,OUT50,O_AF_PP);
	
	// Setup SPI register
	SPI2->CR1 |= 0x04;//Master mode
	SPI2->CR1 |= 0x38;//Fcl/256
	SPI2->CR1 |= 0x40;//SPI enable
	SPI2->CR2 |= 0x04;//Enable SS 
	W_GP(PA, 12, HIGH);
	}
}
void spi_tx(unsigned short spi, char tx_char)
{
	if(spi==1)
	{
		W_GP(PA, 4, LOW); //SS
		SPI1->DR = tx_char; // MOSI
		while(SPI1->SR & 0x80); 
		W_GP(PA, 4, HIGH);//SS
	}
	
		if(spi==2)
	{
		W_GP(PB, 12, LOW); //SS
		SPI2->DR = tx_char; // MOSI
		while(SPI2->SR & 0x80); 
		W_GP(PB, 12, HIGH);//SS
	}
}
void spi_msg(unsigned short spi, const char *str)
{
		while( *str!='\0')
		{
			spi_tx(spi, *str);
			str++;
		}
}
