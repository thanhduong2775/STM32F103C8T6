#include "stm32f10x.h"                  // Device header
#include "lcd_1602_drive.h"

/*
Pin setup
PA8 - RS
PA9 - RW
PA10- E
PA0 - D0
PA1 - D1
...
PA7 - D7
	
*/
void lcd_rs(unsigned char LOW_HIGH){
	W_GP(PA, 8, LOW_HIGH);
}
void lcd_rw(unsigned char LOW_HIGH){
	W_GP(PA, 9, LOW_HIGH);
}
void lcd_e(unsigned char LOW_HIGH){
	W_GP(PA, 10, LOW_HIGH);
}
void pin_output(unsigned char start, unsigned char end){
	char i;
	for (i=start;i<end;i++)
	Digital_Output(PA,i);
}
void lcd_data(unsigned char data){
	pin_output(0,11);
	lcd_rs(1);
	lcd_rw(0);
	DelayUs(10);
	lcd_e(1);
	DelayUs(5);
	GPIOA->ODR &=0xff00;
	GPIOA->ODR |=data;
	DelayUs(10);
	lcd_e(0);
	
}

void lcd_data_4(unsigned char data){
//	pin_output(4,11);
	lcd_rs(1);
	lcd_rw(0);
	DelayUs(10);
//////////////////////////////////////
	lcd_e(1);
	DelayUs(5);
	GPIOA->ODR &=0xff0f;
	GPIOA->ODR |=(data&0xf0);
	DelayUs(10);
	lcd_e(0);
////////////////////////////////////	
	DelayUs(20);
/////////////////////////////////	
	lcd_e(1);
	DelayUs(5);
	GPIOA->ODR &=0xff0f;
	GPIOA->ODR |=((data<<4)&0xf0);
	DelayUs(10);
	lcd_e(0);
	
}
void lcd_cmd(unsigned char data){
	pin_output(0,11);
	lcd_rs(0);
	lcd_rw(0);
	DelayUs(10);
	lcd_e(1);
	DelayUs(5);
	GPIOA->ODR &=0xff00;
	GPIOA->ODR |=data;
	DelayUs(10);
	lcd_e(0);
	
}

void lcd_cmd_4(unsigned char data){

	lcd_rs(0);
	lcd_rw(0);
	DelayUs(10);
	
	
//////////////////////////////////////
	lcd_e(1);
	DelayUs(5);
	GPIOA->ODR &=0xff0f;
	GPIOA->ODR |=(data&0xf0);
	DelayUs(10);
	lcd_e(0);
////////////////////////////////////	
	DelayUs(20);
/////////////////////////////////	
	lcd_e(1);
	DelayUs(5);
	GPIOA->ODR &=0xff0f;
	GPIOA->ODR |=((data<<4)&0xf0);
	DelayUs(10);
	lcd_e(0);
	
}

void lcd_init(void){
	pin_output(0,11);
	DelayMs(20);
	
	lcd_cmd(0x3C); // Function set 8bit, 2lines
	DelayMs(5);
	
	lcd_cmd(0x0C);//Display ON/OFF control
	DelayMs(5);
	
	lcd_cmd(0x01);// Clear Display
	DelayMs(5);
	
	lcd_cmd(0x02);// Get back to initial address
	DelayMs(5);
	
	
}
void lcd_init_4(void){
	pin_output(4,11);
	DelayMs(20);

	lcd_cmd_4(0x2C); // Function set 8bit, 2lines
	DelayMs(5);
	
	lcd_cmd_4(0x0C);//Display ON/OFF control
	DelayMs(5);
	
	lcd_cmd_4(0x01);// Clear Display
	DelayMs(5);
	
	lcd_cmd_4(0x02);// Get back to initial address
	DelayMs(5);
	
	
}


void lcd_send(char str[]){
	int i=0;
	while(str[i]!='\0'){
		lcd_data(str[i]);
		i++;
		DelayUs(100);
	}
}

void lcd_msg(unsigned char line_1_2, unsigned char pos_0_16, char msg[]){
	short pos=0;
	if(line_1_2==1)
		pos=0;
	else if(line_1_2==2)
		pos=0x40;
	lcd_cmd((0x80+pos)+pos_0_16);
	DelayUs(100);
	lcd_send(msg);
}

void lcd_msg_1(unsigned char line_1_2, unsigned char pos_0_16, const char *str){
	short pos=0;
		//int i=0;
	if(line_1_2==1)
		pos=0;
	else if(line_1_2==2)
		pos=0x40;
	lcd_cmd((0x80+pos)+pos_0_16);
	DelayUs(100);
	

	while(*str!='\0'){
		lcd_data(*str);
		str++;
		DelayUs(100);
	
	
}
}

void lcd_msg_4(unsigned char line_1_2, unsigned char pos_0_16, const char *str){
	short pos=0;
		//int i=0;
	if(line_1_2==1)
		pos=0;
	else if(line_1_2==2)
		pos=0x40;
	lcd_cmd_4((0x80+pos)+pos_0_16);
	DelayUs(100);
	

	while(*str!='\0'){
		lcd_data_4(*str);
		str++;
		DelayUs(100);
	
	
}
}



