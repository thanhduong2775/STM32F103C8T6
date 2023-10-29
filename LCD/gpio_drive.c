#include "gpio_drive.h"
void init_GP(unsigned short port, unsigned short pin, unsigned short dir, unsigned short opt)
{
	volatile unsigned long *CR;
	unsigned short tPIN = pin;
	unsigned short offset = 0x00;
	
	if (pin>7)
	{
		tPIN-=8;
		offset=0x01;
	}
	if (port==1)
	{
		RCC_APB2ENR |=4; /// Enabling Port A
		CR = (volatile unsigned long *) (&GPIO_A + offset); //Configuration Register 
	}
	else if (port==2)
	{
		RCC_APB2ENR |=8; /// Enabling Port B
		CR = (volatile unsigned long *) (&GPIO_B + offset);//Configuration Register 
	}
	else if (port==3)
	{
		RCC_APB2ENR |=0x10; /// Enabling Port C
		CR = (volatile unsigned long *) (&GPIO_C + offset);//Configuration Register 
	}
	*CR &= ~(0xF<<(tPIN)*4); ///Reset the target pin
	*CR |= ((dir<<(tPIN*4)) | (opt<<(tPIN*4+2)));/// setup the direction and the option of the PIN

}
int R_GP(unsigned short port, unsigned short pin)// state = IDR [0], this function returns 1 OR 0 
{
	volatile unsigned long * IDR;
	unsigned long offset = 0x02;
	int state;
	
	if (port == 1)
	{
		IDR = (volatile unsigned long *) (&GPIO_A + offset);
	}
	else if (port == 2)
	{
		IDR = (volatile unsigned long *) (&GPIO_B + offset);
	}
	else if (port == 3)
	{
		IDR = (volatile unsigned long *) (&GPIO_C + offset);
	}
	
	state = ((*IDR & (1<<pin))>>pin);
	return state;
}

void W_GP(unsigned short port, unsigned short pin, unsigned short state)/// this function returns to "state[15:0]"
{
	volatile unsigned long * ODR;
	unsigned long offset = 0x03;

	
	if (port == 1)
	{
		ODR = (volatile unsigned long *) (&GPIO_A + offset);
	}
	else if (port == 2)
	{
		ODR = (volatile unsigned long *) (&GPIO_B + offset);
	}
	else if (port == 3)
	{
		ODR = (volatile unsigned long *) (&GPIO_C + offset);
	}

	state ? (*ODR |= (state<<pin)) : (*ODR &= ~(1<<pin));
}

void toggle (unsigned short port, unsigned short pin)
{
	if(R_GP(port, pin))/// if R_GP function returns 1 (state=1)= IDR[0] in R_GP function  
	{
		W_GP(port, pin, 0);// Write 0 in ODR[pin]
	}
	else
	{
		W_GP(port, pin, 1);//Write 1 in ODR[pin]
	}
}

//void PINc(unsigned short pin, unsigned short STATUS){
//	STATUS ? (GPIOC->ODR |=(1<<pin)) : (GPIOC->ODR |= ~(1<<pin);
//}

void Digital_Input(unsigned short PORT, unsigned short PIN){
	init_GP(PORT, PIN, IN, I_PP);
}


void Digital_Output(unsigned short PORT, unsigned short PIN){
	init_GP(PORT, PIN, OUT50, O_GP_PP);
}
