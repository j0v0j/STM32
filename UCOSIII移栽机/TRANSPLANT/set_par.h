#ifndef  	_SET_PAR_H
#define  	_SET_PAR_H
#include  "sys.h"






//规定苗盘参数，数大的是行数，数小的是列数
struct dish__init
{
    u8   row;				//苗盘行数
    u8   col;				//苗盘列数
    u8   dta;				//苗盘间距
    u16  dx1;				//初始位置距离一级苗盘距离
    u16  dx2;				//初始位置距离二级苗盘距离
    u32  num;       //移栽的苗盘数
    u16 dis_length; //苗盘长度参数
    u16 dis_width;  //苗盘宽度参数
};




struct xyz__position//记录x、y、z三个方向的运行位置
{
    u32 pos_x;
    u32 pos_y1;
    u32 pos_y2;
    u32 pos_z;



};


//位置参数
struct plant__init
{
    u8   	hz;				//初始时的HZ
    u8  	lx;				//初始时的LX
    u8  	lbc;		 	//取苗时向前移动的距离
    u16  	camera_x;	//相机识别位置X
    u16  	camera_y1;//相机识别位置Y1
    u16  	camera_y2;//相机识别位置Y2
    u8    rank;     //存储苗的等级信息
    s16  	Y1_Ncrash_dis;   	//左手不碰撞位置
    s16  	Y2_Ncrash_dis;		//右手不碰撞位置
    s16   target_position;
};


//1级苗盘、2级苗盘穴盘的参数
struct plant__rank
{
    u8 	hang;
    u8 	lie;
    u16	_X;      //行
    u16 _Y;
    u16 _Z;
};


//苗盘参数初始化
void all_par_init(void);

//苗盘参数设置
void set_dish_par(u8 r,u8 c,u8 dt,u16 dx1,u16 dx2);

#endif



