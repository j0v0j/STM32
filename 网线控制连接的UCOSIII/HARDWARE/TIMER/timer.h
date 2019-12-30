#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern TIM_HandleTypeDef TIM2_Handler;      //��ʱ����� 

//�߼���ʱ���궨��
#define ADVANCE_TIM           		        TIM8
#define ADVANCE_TIM_CLK_ENABLE()       		__TIM8_CLK_ENABLE()
#define ADVANCE_TIM_CH                    TIM_CHANNEL_1
#define ADVANCE_TIM_CH2                   TIM_CHANNEL_2
#define ADVANCE_TIM_CH3                   TIM_CHANNEL_3
//TIMͨ��CH1�����ź궨��
#define ADVANCE_TIM_CHx_CLK()             __GPIOC_CLK_ENABLE()
#define ADVANCE_TIM_CHx_PORT              GPIOC
#define ADVANCE_TIM_CHx_PIN               GPIO_PIN_6
#define ADVANCE_TIM_CHx_PIN2              GPIO_PIN_7
#define ADVANCE_TIM_CHx_PIN3              GPIO_PIN_8

//TIMͨ��CH1N�����ź궨��
#define ADVANCE_TIM_CHxN_CLK()            __GPIOA_CLK_ENABLE()
#define ADVANCE_TIM_CHxN_PORT             GPIOA 
#define ADVANCE_TIM_CHxN_PIN              GPIO_PIN_5
//TIM����BKIN�궨��
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

