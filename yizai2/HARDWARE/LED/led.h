#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//enum DIRCTION{
//	LEFT,
//	RIGHT,
//	DOWN,
//	UP
//} Plus_Dir;
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

#define Turn1        PBout(6)// PB6
#define Turn2        PBout(7)// PB7	
#define PLUS	       PGout(13)
#define TIM_BREAK	   PGout(11)//脉冲刹车

#define LEFT_MOVE    Turn1=RESET;Turn2=RESET;  //00->0
#define RIGHT_MOVE   Turn1=SET;Turn2=SET;		   //11->3
#define UP_MOVE      Turn1=RESET;Turn2=SET;		 //01->1
#define DOWN_MOVE  	 Turn1=SET;Turn2=RESET; 	 //10->2
#define LEFT         0
#define RIGHT        3
#define DOWN         2
#define UP           1
#define PLUS_DIR     dir_plus()
void LED_Init(void);//初始化
u8 dir_plus(void);
		 				    
#endif
