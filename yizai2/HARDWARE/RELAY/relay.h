#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//GPIOλ������							  
////////////////////////////////////////////////////////////////////////////////// 
#define RIN1 PCout(8)         //���ŷ���Դ 
#define RIN2 PCout(9)					//���ŷ���բ
#define RIN3 PCout(10)				
#define RIN4 PCout(11)				//С�ŷ���Դ
#define RIN5 PCout(12)				//1�����͵��
#define RIN8 PGout(12)				//2�����͵�� 
#define RIN7 PGout(14)				//3�����͵�� 
#define RIN6 PGout(15)					//0 
#define RINall(n) RIN1=n;RIN2=n;RIN3=n;RIN4=n;RIN5=n;RIN6=n;RIN7=n;RIN8=n;


void 	Relay_Init(void);//��ʼ��
void Relay_test(u8 i,u8 sta);

#endif


