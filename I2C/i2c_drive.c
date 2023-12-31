#include "i2c_drive.h"

/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA
I2C1 and I2C2 -> APB1
*/
void i2c_init(char i2c,unsigned short speed_mode)
{
	RCC->APB2ENR |= 1; 
	
	if(i2c==1)
	{
		RCC->APB1ENR |= 0x200000;
		// Pin enable 
		init_GP(PB,6,OUT50,O_AF_OD);
		init_GP(PB,7,OUT50,O_AF_OD);
		I2C1->CR1 |= 0x8000;
		I2C1->CR1 &= ~0x8000;
		I2C1->CR2 =0x8;
		I2C1->CCR = speed_mode;
		I2C1->TRISE = 0x9;
		I2C1->CR1 |= 1;
	}
	else if(i2c==2)
	{
		RCC->APB1ENR |= 0x400000;
		// Pin enable 
		init_GP(PB,10,OUT50,O_AF_OD);
		init_GP(PB,11,OUT50,O_AF_OD);
		I2C2->CR1 |= 0x8000;
		I2C2->CR1 &= ~0x8000;
		I2C2->CR2 =0x8;
		I2C2->CCR = speed_mode;
		I2C2->TRISE = 0x9;
		I2C2->CR1 |= 1;
	}

}
void i2c_start(char i2c){
	if(i2c == 1){
		I2C1->CR1 |=0x100; //Start Generation CR1[8]
		while(!(I2C1->SR1&1));// SB: Start bit (Master mode)
	}
	else if (i2c==2){
		I2C2->CR1 |=0x100; //Start Generation CR1[8]
		while(!(I2C2->SR1&1));// SB: Start bit (Master mode)
	}
}
// Sending the address + R/W
void i2c_add(char i2c, char address,char R_W)
{
	volatile int tmp;
	if(i2c==1)
	{
		I2C1->DR = (address|R_W);
		while((I2C1->SR1 & 2)==0){};
		while((I2C1->SR1 & 2)){
			tmp = I2C1->SR1;
			tmp = I2C1->SR2;
			if((I2C1->SR1 & 2)==0)
			{
				break;
			}
		}
	}
	else if(i2c==2)
	{
		I2C2->DR = ((address<<0)|R_W);
		while((I2C2->SR1 & 2)==0){};
		while((I2C2->SR1 & 2)){
			tmp = I2C2->SR1;
			tmp = I2C2->SR2;
			if((I2C2->SR1 & 2)==0)
			{
				break;
			}
		}
	}

}

void i2c_data(char i2c, char data)
{
	if(i2c==1)
	{
		while(!(I2C1->SR1 & 0x80)); //empty?
		I2C1->DR = data;
		while(!(I2C1->SR1 & 0x80));
	}
	else if(i2c==2)
	{
		while((I2C2->SR1 & 0x80) == 0){}
		I2C2->DR = data;
		while(!(I2C2->SR1 & 0x80));
	}	
}

void i2c_stop(char i2c)
{
	volatile int tmp;
	if(i2c==1)
	{
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		I2C1->CR1 |= 0x200;//STOP bit
	}
	else if(i2c==2)
	{
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		I2C1->CR1 |= 0x200;//STOP bit
	}
}

void i2c_write(char i2c, char address, char data[])
{
	int i = 0;
	i2c_start(i2c);
	i2c_add(i2c, address, 0);
	while (data[i]!='\0')
	{
		i2c_data(i2c, data[i]);
		i++;
	}
	i2c_stop(i2c);
}
//receiving data
char i2c_rx(char i2c, char ACK_NACK)
{
	char temp;
	if(i2c==1)
	{
		I2C1->CR1 |=0x0400; //ACK return(match address or data)
		while(!(I2C1->SR1 & 0x40));
		temp = I2C1->DR;
		if(ACK_NACK)
			I2C1->CR1&=~0x0400;//No ACK return
	}
	else if(i2c==2)
	{
		I2C2->CR1 |=0x0400; //ACK return(match address or data)
		while(!(I2C2->SR1 & 0x40));
		temp = I2C2->DR;
		if(ACK_NACK)
			I2C2->CR1 &=~0x0400;//No ACK return
	}		
	return temp;
}
