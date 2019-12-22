#include "led.h"

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
//2019-08-18，查看FSMC的IO后，发现外部SRAM使用不能作为IO口，将方向控制IO改到PB6 PB7，这两个引脚原为IIC引脚
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化



void LED_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,PE端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				
	 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
	
	
	
	
		//大伺服电机方向
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	    		 //Turn1-->PB.6 Turn2-->PB.7端口配置, 推挽输出
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 
	 GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 						       
		
	 
	 //脉冲刹车
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13;	    		 //PLUS-->PG.13   tim_break-->PG.11 端口配置, 推挽输出
	 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				
	 GPIO_SetBits(GPIOG,GPIO_Pin_11|GPIO_Pin_13); 			         	
}

/*
#define LEFT_MOVE    Turn1=RESET;Turn2=RESET;
#define RIGHT_MOVE   Turn1=SET;Turn2=SET;
#define UP_MOVE      Turn1=RESET;Turn2=SET;
#define DOWN_MOVE  	 Turn1=SET;Turn2=RESET;
*/
u8 dir_plus(void)
{
	u8 T1=Turn1,T2=Turn2; 
	switch((T1<<1)|T2)
		{
			case LEFT:
				return LEFT;
			case RIGHT:
				return RIGHT;
			case UP:
				return UP;
			case DOWN:
				return DOWN;	
		}
	return 255;
}


