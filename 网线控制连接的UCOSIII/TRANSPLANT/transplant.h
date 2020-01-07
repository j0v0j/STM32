#ifndef  _TRANSPLANT_H
#define  _TRANSPLANT_H
#include "sys.h"

#include "trans_byhand.h"

#include "set_par.h"
#include "set_f.h"

#define   Z_down        0
#define   Z_up					plant_init->hz
/*-----------����9�������----------------
0:�����δץȡ		1:һ����		2:������
ץȡ��ʽ��	���ּ��5Ѩ����ץȡ
��е����ȼ�
-------------------------------------------
|����\���� |	   	0	   |    1	    |	   2     |
-------------------------------------------
|	 	 0	  |  A=(0,0) |	B=(0,1)	|  C=(0,2) |
-------------------------------------------
|	 	 1	  |  D=(1,0) |	E=(1,1)	|  F=(1,2) |
-------------------------------------------
|	 	 2	  |  G=(2,0) |	H=(2,1)	|  I=(2,2) |
-------------------------------------------
*/
#define 	A_case  			0
#define 	B_case  			1
#define 	C_case  			2
#define 	D_case  			3
#define 	E_case  			4
#define 	F_case  			5
#define 	G_case  			6
#define 	H_case  			7
#define 	I_case  			8
#define 	M_rank				wait_rank()
#define   clear_rank()	plant_init->rank=9

//����ֻ��ץ��ȡ�纯���궨��
#define 	Left_fetch()    		leftcan_fetch()				//����ץ��
#define 	Right_fetch()	   		rightcan_fetch()			//����ץ��
#define 	Left_release()    	leftcan_release()			//���ַ���
#define 	Right_release()    	rightcan_release()		//���ַ���



extern struct set__f 			*set_f;
extern struct plant__init 		*plant_init;
extern struct xyz__position 	*real_position;
extern struct dish__init 		*dish_init;
extern struct plant__rank 	*plant_rank_one;
extern struct plant__rank 	*plant_rank_two;
//���Ի�����״̬���л�����ͣ
#define Plant_stop   	0
#define Plant_run    	1
struct plant__status
{
    u8 stop_or_run;		//��ʼʱ��HZ
} plant__status= {Plant_stop};
struct plant__status *plant_status;

struct dish_RC//����Ѩ�����������Ե��к��У�Ϊȫ�ֱ���
{
    s8 row;
    s8 col;
};


struct positionA//����A���������Ϣ��Ϊȫ�ֱ���
{
    u16 _x;
    s16 _y1;
    s16 _y2;
    u16 _z;
};


//struct positionB//����B���������Ϣ��Ϊȫ�ֱ���
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionC//����C���������Ϣ��Ϊȫ�ֱ���
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;

//};

//struct positionD//����D���������Ϣ��Ϊȫ�ֱ���
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionE//����E���������Ϣ��Ϊȫ�ֱ���
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionF//����F���������Ϣ��Ϊȫ�ֱ���
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionG//����G���������Ϣ��Ϊȫ�ֱ���
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};


//��ȼ�����
void plant_rank(u8 dj);

u8 wait_rank(void);

//��������������
void planting(void);
void planting_stop(void);
void planting_start(void);
void planting_restart(void);
void planting_ERR(void);


#endif












