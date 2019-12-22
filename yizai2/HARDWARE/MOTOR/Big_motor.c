#include "Big_motor.h"
#include "advance_time.h"
#include "led.h"
#include "yizai.h"
#include "usart.h"
#include "Big_motor.h"

s32 X=0; 				  //此时的实际坐标
u32 Y=0;					//此时的实际坐标
s32 Z=0;          //定义十字在最顶端时，Z的值为0
void move_x(u32 MX,u16 f)//MX为绝对位置的脉冲个数
{	
	if(X>=0)
	{
		printf("\r\nMX=%d,X=%d\r\n",MX,X);
		if(MX>X)//目标位置大于现在位置，向左运动
		{	
			//需要移动的距离
			//改变方向
			LEFT_MOVE
			Pulse_output(f,MX-X);
			X=MX;
			
		}
		else if(MX<X)//目标位置小于现在位置，向右运动
		{	
			//需要移动的距离
			//改变方向
			RIGHT_MOVE
			Pulse_output(f,X-MX);
			X=MX;
		}
		else//目标位置和现在位置相同
		{
			printf("\r\nsame position！\r\n");
		}
	
	}
	else
	{
		printf("\r\nmove_x函数未执行！\r\n");
	
	}
}
void move_z(s32 MZ,u16 f)
{
	if(Z>=0)
	{
		printf("\r\nZ=%d\r\n",MZ);
		if(MZ<Z)//十字下落
		{	
			//需要移动的距离	
			DOWN_MOVE
			Pulse_output(f,Z-MZ);
			Z=MZ;
		}
		else if(MZ>Z)//十字升起
		{		
			UP_MOVE
			Pulse_output(f,MZ-Z);
			Z=MZ;
			
		}
		else//目标位置和现在位置相同
		{
			printf("\r\nsame position！\r\n");
		}
		
	}
	else
	{
		printf("\r\nmove_z函数未执行！\r\n");
	}
}
//u32 get_x(void)
//{
//	printf("\r\nX=%d\r\n",X);
//	return X;
//}
//u32 get_z(void)
//{
//	printf("\r\nZ=%d\r\n",Z);
//	return Z;
//}
//void set_Z0(void)
//{
//	Z=0;
//}
//void set_X0(void)
//{
//	X=0;
//}
//void set_Y0(void)
//{
//	Y=0;
//}



