#ifndef __YIZAI2_H
#define __YIZAI2_H
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"


#define DJ1miao  1
#define DJ2miao  2
/*移栽任务相关的变量*/
//任务优先级
#define YZ2_TASK_PRIO		7
//任务堆栈大小	
#define YZ2_STK_SIZE 		128
//脉冲发送完成标志,即大伺服运行到位了



/*行参数大伺服电机控制，列参数小伺服控制*/
//任务函数
void chushi_weizhi(void);//回到初始的位置
void yizai2(void *pvParameters);//移栽自动程序
void yizai2_restart(void);//重启自动程序
void yizai2_stop(u8 yn);
void piaoman_canshu(u8 hang,u8 lie,u8 L,u16 d1x,u16 d2x);//设置苗盘参数
void set_yizai2_F(u16 i);//设置大伺服的运行速度即频率f
void xiaosifu_v(u16 XV);//设置小伺服的速度
void dasifu_daowei(void);//判断大伺服是否到位，如果不到位不等进行其他的操作
void xiaosifu_daowei(long Y);//判断小伺服是否到位
void yudingwei(u16 h);//对准一个苗盘后的操作，记录一些参数
void START_DSF(u8 K_G);
void yizai2_shoudong(void);

#endif

