
#include "gpio_drive.h"
#include "systick_time.h"
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

int main(void){
	//char c[]="Toi da yeu em";
	systick_init();
	lcd_init_4();
	
	//lcd_msg(1,0,c);
	
lcd_msg_4(1,0,"dit me Q.pHUC");
	
	
}
