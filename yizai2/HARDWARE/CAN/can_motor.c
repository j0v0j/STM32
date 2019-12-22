#include "can_motor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "can.h"

#include "usart.h"
#include "Big_motor.h"

#define  Motor0   1
#define  Motor1   2


#define  Motor_rest   Can_rest()
#define  Motor_mode   Can_mode()


u32 L_m0=0;        //L_m0记录0号小伺服电机的位置，
u32 L_m1=0;				 //L_m1记录1号小伺服电机的位置，


extern short Real_Velocity_Value[4];
extern long Real_Position_Value[4];
extern char Real_Online[4];
extern char Real_Ctl1_Value[4];
extern char Real_Ctl2_Value[4];
extern u32 Y;
/*小的伺服复位*/
void Can_rest()
{
	CAN_DRV_Reset(0,Motor0);
	CAN_DRV_Reset(0,Motor1);
	vTaskDelay(200);
	Can_mode();
}
/*小伺服模式选择速度位置模式*/
void Can_mode()
{
	
	CAN_DRV_Mode_Choice(0,Motor0,Velocity_Position_Mode);
	CAN_DRV_Mode_Choice(0,Motor1,Velocity_Position_Mode);
//	printf("v&p mode\r\n");
}

/*
pwm 的取值范围为： 0 ~ +5000。
velocity 的取值范围为： 0 ~ +32767。（16 位有符号整型数的正数范围。 单位 RPM）
position 的取值范围为： -2147483648 ~ +2147483647。（32 位有符号整型数的范围， 单位 qc）
*/
/*小伺服0模式速度和位置*/
void Can_v0_p0(u16 v,long p)
{
	CAN_DRV_Velocity_Position_Mode(0,Motor0,5000,v,p);
//	CAN_DRV_Current_Velocity_Position_Mode(0,Motor0,2500,v,p);
	L_m0=p;
}
/*小伺服0模式速度和位置*/
void Can_v1_p1(u16 v,long p)
{	
	CAN_DRV_Velocity_Position_Mode(0,Motor1,5000,v,p);
	L_m1=p;
}
//小伺服位置
void Now_open()
{
		CAN_DRV_Config(0,1,1,0);
		CAN_DRV_Config(0,2,1,0);
}	
void Now_close()
{
	CAN_DRV_Config(0,1,0,0);
	CAN_DRV_Config(0,2,0,0);
}	

u16 m1_velocity1=0,m2_velocity2=0;
u32	m1_position1=0,m2_position2=0;
u16 m1_velocity,m2_velocity;
u32	m1_position,m2_position;
	



void move_y(u16 v,long MY)
{	
	Can_v0_p0(v,MY);
	printf("\r\nY=%ld\r\n",MY/404);
}




