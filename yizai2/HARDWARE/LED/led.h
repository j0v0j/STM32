#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
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
#define TIM_BREAK	   PGout(11)//����ɲ��

#define LEFT_MOVE    Turn1=RESET;Turn2=RESET;  //00->0
#define RIGHT_MOVE   Turn1=SET;Turn2=SET;		   //11->3
#define UP_MOVE      Turn1=RESET;Turn2=SET;		 //01->1
#define DOWN_MOVE  	 Turn1=SET;Turn2=RESET; 	 //10->2
#define LEFT         0
#define RIGHT        3
#define DOWN         2
#define UP           1
#define PLUS_DIR     dir_plus()
void LED_Init(void);//��ʼ��
u8 dir_plus(void);
		 				    
#endif
