#ifndef _TIMER__PLUS_H
#define _TIMER__PLUS_H
#include "sys.h"
#include "set_par.h"

extern struct dish__init 			*dish_init;
extern struct xyz__position 	*real_position;


void B_servo_init(void);
u8 move_x(u32 MX,u16 f);
u8 move_y(u32 MX,u16 f);
struct B_servo__  
{   
    u8 (*move_x)(u32 ,u16); //函数指针 
		u8 (*move_y)(u32 ,u16); //函数指针
		u8 (*stop  )(void);
}; 

struct B_servo__  *B_servo;


#endif

