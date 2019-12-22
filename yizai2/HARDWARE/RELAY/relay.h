#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//GPIO位带操作							  
////////////////////////////////////////////////////////////////////////////////// 
#define RIN1 PCout(8)         //大伺服电源 
#define RIN2 PCout(9)					//大伺服抱闸
#define RIN3 PCout(10)				
#define RIN4 PCout(11)				//小伺服电源
#define RIN5 PCout(12)				//1号输送电机
#define RIN8 PGout(12)				//2号输送电机 
#define RIN7 PGout(14)				//3号输送电机 
#define RIN6 PGout(15)					//0 
#define RINall(n) RIN1=n;RIN2=n;RIN3=n;RIN4=n;RIN5=n;RIN6=n;RIN7=n;RIN8=n;


void 	Relay_Init(void);//初始化
void Relay_test(u8 i,u8 sta);

#endif


