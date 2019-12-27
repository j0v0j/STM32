#include "timer.h"
#include "led.h" 
#include "timer_plus.h"  
//////////////////////////////////////////////////////////////////////////////////	 
void B_servo_init(void)   //初始化函数指针
{
    B_servo->move_x=&move_x;
		B_servo->move_y=&move_y;
}
u8 move_x(u32 MX,u16 f)
{

}
u8 move_y(u32 MX,u16 f)
{

}

