#ifndef __YIZAI2_H
#define __YIZAI2_H
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"


#define DJ1miao  1
#define DJ2miao  2
/*����������صı���*/
//�������ȼ�
#define YZ2_TASK_PRIO		7
//�����ջ��С	
#define YZ2_STK_SIZE 		128
//���巢����ɱ�־,�����ŷ����е�λ��



/*�в������ŷ�������ƣ��в���С�ŷ�����*/
//������
void chushi_weizhi(void);//�ص���ʼ��λ��
void yizai2(void *pvParameters);//�����Զ�����
void yizai2_restart(void);//�����Զ�����
void yizai2_stop(u8 yn);
void piaoman_canshu(u8 hang,u8 lie,u8 L,u16 d1x,u16 d2x);//�������̲���
void set_yizai2_F(u16 i);//���ô��ŷ��������ٶȼ�Ƶ��f
void xiaosifu_v(u16 XV);//����С�ŷ����ٶ�
void dasifu_daowei(void);//�жϴ��ŷ��Ƿ�λ���������λ���Ƚ��������Ĳ���
void xiaosifu_daowei(long Y);//�ж�С�ŷ��Ƿ�λ
void yudingwei(u16 h);//��׼һ�����̺�Ĳ�������¼һЩ����
void START_DSF(u8 K_G);
void yizai2_shoudong(void);

#endif

