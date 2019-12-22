#ifndef __EKEY_H
#define __EKEY_H	
#include "sys.h"




#define EKEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)//��ȡPC0  ���ڴ��ŷ��Ҹ�λ
#define EKEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//��ȡPC1  ���ڴ��ŷ��ϸ�λ
#define EKEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)//��ȡPC2	 ���ڴ��ŷ���λ
#define EKEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)//��ȡPC3	 ���ڴ��ŷ��¸�λ
#define EKEY4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//��ȡPC4	 ����С�ŷ���λ
#define EKEY5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//��ȡPC4	 ����С�ŷ���λ
#define EKEY6  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//��ȡPC4	 ����С�ŷ���λ

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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 
u8 EKEY_Scan(u8 mode);//�ⲿ�жϳ�ʼ��		 
void EKEY_GPIO(void);

#endif

