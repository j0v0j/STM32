#ifndef __CAN_MORTOR_H
#define __CAN_MORTOR_H	
#include "can.h"


/*С�ŷ������λ*/
void Can_rest(void);

/*С�ŷ�ģʽѡ���ٶ�λ��ģʽ*/
void Can_mode(void);

/*С�ŷ�0ģʽ�ٶȺ�λ��*/
void Can_v0_p0(u16 v,long p);

/*С�ŷ�0ģʽ�ٶȺ�λ��*/
void Can_v1_p1(u16 v,long p);

//С�ŷ������ٶ�λ�÷�������
void Now_open(void);

//С�ŷ������ٶ�λ�÷����ر�
void Now_close(void);

//��ʾС�ŷ�����ٶ�λ��

void move_y(u16 v,long MY);
#endif 

