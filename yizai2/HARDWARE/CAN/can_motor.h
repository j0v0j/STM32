#ifndef __CAN_MORTOR_H
#define __CAN_MORTOR_H	
#include "can.h"


/*小伺服电机复位*/
void Can_rest(void);

/*小伺服模式选择速度位置模式*/
void Can_mode(void);

/*小伺服0模式速度和位置*/
void Can_v0_p0(u16 v,long p);

/*小伺服0模式速度和位置*/
void Can_v1_p1(u16 v,long p);

//小伺服电流速度位置反馈开启
void Now_open(void);

//小伺服电流速度位置反馈关闭
void Now_close(void);

//显示小伺服电机速度位置

void move_y(u16 v,long MY);
#endif 

