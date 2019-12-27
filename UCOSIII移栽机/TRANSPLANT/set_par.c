#include "set_par.h"


struct dish__init 		*dish_init;
struct xyz__position 	*real_position;
struct plant__init 		*plant_init;
struct plant__rank_one 	*plant_rank_one;
struct plant__rank_two 	*plant_rank_two;


void all_par_init()
{
    //苗盘参数初始化
    dish_init->row	=	12;
    dish_init->col	=	8;
    dish_init->dta	=	33;
    dish_init->dx1	=	416;
    dish_init->dx2	=	976;

    //记录实际运行的位置参数信息
    real_position->pos_x  = 0;
    real_position->pos_y1 = 0;
    real_position->pos_y2 = 0;
    real_position->pos_z  = 0;

    //移栽参数初始化
    plant_init->hz = 140;
    plant_init->lx = 33;
}
//设置苗盘行、列数、穴孔间距、初始位置距离一级苗盘和二级苗盘的距离、已经移栽的穴盘数量

void set_dish_par(u8 r,u8 c,u8 dt,u16 dx1,u16 dx2)
{
    dish_init->row=r;
    dish_init->col=c;
    dish_init->dta=dt;
    dish_init->dx1=dx1;
    dish_init->dx2=dx2;
    dish_init->num=0;
    dish_init->dis_length=r*(dt-1);
    dish_init->dis_width=c*(dt-1);
}



