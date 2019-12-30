#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 

//高级定时器宏定义
#define ADVANCE_TIM           		        TIM8
#define ADVANCE_TIM_CLK_ENABLE()       		__TIM8_CLK_ENABLE()
#define ADVANCE_TIM_CH                    TIM_CHANNEL_1
#define ADVANCE_TIM_CH2                   TIM_CHANNEL_2
#define ADVANCE_TIM_CH3                   TIM_CHANNEL_3
//TIM通道CH1的引脚宏定义
#define ADVANCE_TIM_CHx_CLK()             __GPIOC_CLK_ENABLE()
#define ADVANCE_TIM_CHx_PORT              GPIOC
#define ADVANCE_TIM_CHx_PIN               GPIO_PIN_6
#define ADVANCE_TIM_CHx_PIN2              GPIO_PIN_7
#define ADVANCE_TIM_CHx_PIN3              GPIO_PIN_8

//TIM通道CH1N的引脚宏定义
#define ADVANCE_TIM_CHxN_CLK()            __GPIOA_CLK_ENABLE()
#define ADVANCE_TIM_CHxN_PORT             GPIOA 
#define ADVANCE_TIM_CHxN_PIN              GPIO_PIN_5
//TIM引脚BKIN宏定义
#define ADVANCE_TIM_BKIN_CLK()            __GPIOA_CLK_ENABLE()
#define ADVANCE_TIM_BKIN_PORT             GPIOA 
#define ADVANCE_TIM_BKIN_PIN              GPIO_PIN_6


struct PLUS__NUM
{
	
	u32 number;
	u8  stop_flag;

};

void PWM_gpio(void);
void TIM8_Init(u16 HZ);

void TIM2_Init(void);

void Plus_out_stop(void);
void Plus_out(u16 HZ,u32 plus_n);

#endif

