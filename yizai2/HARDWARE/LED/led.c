#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
//2019-08-18���鿴FSMC��IO�󣬷����ⲿSRAMʹ�ò�����ΪIO�ڣ����������IO�ĵ�PB6 PB7������������ԭΪIIC����
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��



void LED_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				
	 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
	
	
	
	
		//���ŷ��������
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	    		 //Turn1-->PB.6 Turn2-->PB.7�˿�����, �������
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 
	 GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 						       
		
	 
	 //����ɲ��
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13;	    		 //PLUS-->PG.13   tim_break-->PG.11 �˿�����, �������
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


