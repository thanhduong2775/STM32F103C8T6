#include "stm32f10x.h"                  // Device header
#include "lcd_1602_drive.h"
#include "PCF8574_drive.h"
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

//PCF8574 
/*
LCD Command Wire
P0 - RS
P1 - RW
P2 - E
P3 - Screen Light->> must be always 1
-------------------------------------
LCD Data Wire
P4 - D4 
P5 - D5
P6 - D6
P7 - D7 -> MSB
	
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
void lcd_i2c_data(char i2c, char data)
{
	PCF8574_tx_byte(i2c, 0x09);//E low: 1 E 0 RW: 1001
	
	DelayUs(10);
//////////////////////////////////////
	//lcd_e(1);
	PCF8574_tx_byte(i2c, 0x0D); //E high 1 E 0 RW
	DelayUs(5);
	//GPIOA->ODR &=0xff0f;
	//GPIOA->ODR |=(data&0xf0);
	PCF8574_tx_byte(i2c, ((data & 0xf0) | 0x0D));	// E high
	DelayUs(10);
	//lcd_e(0);
	PCF8574_tx_byte(i2c, ((data & 0xf0) | 0x09));	// E Low
////////////////////////////////////	
	DelayUs(20);
/////////////////////////////////	
//	lcd_e(1);
	PCF8574_tx_byte(i2c, 0x0D); //E high 1 E 0 RW
	DelayUs(5);
//	GPIOA->ODR &=0xff0f;
//	GPIOA->ODR |=((data<<4)&0xf0);
	PCF8574_tx_byte(i2c, (((data<<4) & 0xf0) | 0x0D));	// E high
	DelayUs(10);
	//lcd_e(0);
	PCF8574_tx_byte(i2c, (((data<<4) & 0xf0) | 0x09));	// E low
}
void lcd_i2c_cmd(char i2c, char data)
{
	lcd_rs(0);
	lcd_rw(0);
	PCF8574_tx_byte(i2c, 0x08);//E low: 1 E 0 RW: 1000
	DelayUs(10);
	
	
//////////////////////////////////////
	//lcd_e(1);
	PCF8574_tx_byte(i2c, 0x0C);//E high: 1 1 0 0
	
	DelayUs(5);
	//GPIOA->ODR &=0xff0f;
	//GPIOA->ODR |=(data&0xf0);
	PCF8574_tx_byte(i2c, ((data & 0xf0) | 0x0C)); // E ligh
	DelayUs(10);
	//lcd_e(0);
	PCF8574_tx_byte(i2c, ((data & 0xf0) | 0x08));// E low
////////////////////////////////////	
	DelayUs(20);
/////////////////////////////////	
	//lcd_e(1);
	PCF8574_tx_byte(i2c, 0x0C);//E high: 1 1 0 0

	DelayUs(5);
	//GPIOA->ODR &=0xff0f;
	//GPIOA->ODR |=((data<<4)&0xf0);
	PCF8574_tx_byte(i2c, (((data<<4) & 0xf0) | 0x0C)); // E ligh

	DelayUs(10);
	//lcd_e(0);
	PCF8574_tx_byte(i2c, (((data<<4) & 0xf0) | 0x08));// E low
	
}
void lcd_i2c_init(char i2c)
{
	i2c_init(i2c, i2c_FM);
	DelayMs(20);

	lcd_i2c_cmd(i2c, 0x2C); // Function set 4bit, 2lines
	DelayMs(5);
	
	lcd_i2c_cmd(i2c, 0x0C);//Display ON/OFF control
	DelayMs(5);
	
	lcd_i2c_cmd(i2c, 0x01);// Clear Display
	DelayMs(5);
	
	lcd_i2c_cmd(i2c, 0x02);// Get back to initial address
	DelayMs(5);
}

void lcd_i2c_send(char i2c, char str[]){
	int i=0;
	while(str[i]!='\0'){
		lcd_i2c_data(i2c,str[i]);
		i++;
		DelayUs(100);
	}
}

void lcd_i2c_msg(char i2c, unsigned char line, unsigned char col,  char msg[])
{
	unsigned char address;
	switch (line)
	{
		case 0: address = 0x80; break;
		case 1: address = 0xC0; break;
		case 2: address = 0x94; break;
		case 3: address = 0xD4; break;
	}
	
	
	lcd_i2c_cmd(i2c,address + col);
	DelayUs(100);
	lcd_i2c_send(i2c,msg);
	
	

}
