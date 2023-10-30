#include "systick_time.h"
#include "lcd_1602_drive.h"

int main(void)
{
//char c[]="CRISTIANO";
systick_init();	
lcd_i2c_init(2);
//lcd_i2c_msg(2,1,1,  c);
	lcd_i2c_msg(2,0,1, "SIUUUUUUUUU");

	while(1)
	{

			lcd_i2c_msg(2,1,1, "FU     ");
			lcd_i2c_msg(2,2,1, "FO       ");
			lcd_i2c_msg(2,3,1, "  FA");
	}
	



}

