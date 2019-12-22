#ifndef __EKEY_H
#define __EKEY_H	
#include "sys.h"




#define EKEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)//读取PC0  用于大伺服右复位
#define EKEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//读取PC1  用于大伺服上复位
#define EKEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)//读取PC2	 用于大伺服左复位
#define EKEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)//读取PC3	 用于大伺服下复位
#define EKEY4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//读取PC4	 用于小伺服限位
#define EKEY5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//读取PC4	 用于小伺服限位
#define EKEY6  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//读取PC4	 用于小伺服限位

#define EKEY0_PRES 1
#define EKEY1_PRES 2
#define EKEY2_PRES 3
#define EKEY3_PRES 4
#define EKEY4_PRES 5
#define EKEY5_PRES 6
#define EKEY6_PRES 7

#define EKEY_STATE   EKEY0==0||EKEY1==0||EKEY2==0||EKEY3==0||EKEY4==0||EKEY5==0||EKEY6==0
#define sEKEY_STATE  EKEY0==1||EKEY1==1||EKEY2==1||EKEY3==1||EKEY4==1||EKEY5==1||EKEY6==1


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 
u8 EKEY_Scan(u8 mode);//外部中断初始化		 
void EKEY_GPIO(void);

#endif

