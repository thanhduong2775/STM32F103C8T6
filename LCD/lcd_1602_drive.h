#include "gpio_drive.h"
#include "systick_time.h"
void pin_output(unsigned char start, unsigned char end);
void lcd_rs(unsigned char LOW_HIGH);
void lcd_rw(unsigned char LOW_HIGH);
void lcd_e(unsigned char LOW_HIGH);
void pin_output(unsigned char start, unsigned char end);
void lcd_data(unsigned char data);
void lcd_cmd(unsigned char data);
void lcd_init(void);
void lcd_send(char str[]);
void lcd_msg(unsigned char line_1_2, unsigned char pos_0_16, char msg[]);
void lcd_msg_1(unsigned char line_1_2, unsigned char pos_0_16, const char *str);


void lcd_init_4(void);
void lcd_data_4(unsigned char data);
void lcd_cmd_4(unsigned char data);
void lcd_msg_4(unsigned char line_1_2, unsigned char pos_0_16, const char *str);
