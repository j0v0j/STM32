#ifndef  _TRANSPLANT_H
#define  _TRANSPLANT_H
#include "sys.h"

#include "trans_byhand.h"

#include "set_par.h"
#include "set_f.h"

#define   Z_down        0
#define   Z_up					plant_init->hz
/*-----------定义9种苗情况----------------
0:无苗或未抓取		1:一级苗		2:二级苗
抓取方式：	两手间隔5穴依次抓取
机械手苗等级
-------------------------------------------
|左手\右手 |	   	0	   |    1	    |	   2     |
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

//对两只手抓苗取苗函数宏定义
#define 	Left_fetch()    		leftcan_fetch()				//左手抓苗
#define 	Right_fetch()	   		rightcan_fetch()			//右手抓苗
#define 	Left_release()    	leftcan_release()			//左手放苗
#define 	Right_release()    	rightcan_release()		//右手放苗



extern struct set__f 			*set_f;
extern struct plant__init 		*plant_init;
extern struct xyz__position 	*real_position;
extern struct dish__init 		*dish_init;
extern struct plant__rank 	*plant_rank_one;
extern struct plant__rank 	*plant_rank_two;
//移栽机运行状态运行或者暂停
#define Plant_stop   	0
#define Plant_run    	1
struct plant__status
{
    u8 stop_or_run;		//初始时的HZ
} plant__status= {Plant_stop};
struct plant__status *plant_status;

struct dish_RC//定义穴盘苗正在移栽的行和列，为全局变量
{
    s8 row;
    s8 col;
};


struct positionA//定义A点的行列信息，为全局变量
{
    u16 _x;
    s16 _y1;
    s16 _y2;
    u16 _z;
};


//struct positionB//定义B点的行列信息，为全局变量
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionC//定义C点的行列信息，为全局变量
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;

//};

//struct positionD//定义D点的行列信息，为全局变量
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionE//定义E点的行列信息，为全局变量
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionF//定义F点的行列信息，为全局变量
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};

//struct positionG//定义G点的行列信息，为全局变量
//{
//    u16 _x;
//    u16 _y1;
//    u16 _y2;
//    u16 _z;
//};


//苗等级接收
void plant_rank(u8 dj);

u8 wait_rank(void);

//运行移栽任务函数
void planting(void);
void planting_stop(void);
void planting_start(void);
void planting_restart(void);
void planting_ERR(void);


#endif












