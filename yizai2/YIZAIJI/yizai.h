#ifndef __YIZAI_H
#define __YIZAI_H
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

/*移栽任务相关的变量*/
//任务优先级
#define YZ_TASK_PRIO		7
//任务堆栈大小	
#define YZ_STK_SIZE 		128  



//任务函数
void yizai_task(void *pvParameters);

void yizai_run(void *pvParameters);
void yizai_stop(void *pvParameters);
void yizai_restart(void);

void Set_miaopan(u8 detal,u8 C, u8 R);
void Start_init(u32 lx,u32 ly,u32 d1,u32 z,u32 d2,u32 dz);
void Clear_st(void);
void Init_number(void);
//void Set_F(u16 f);
u8 Limit_key(void);
u8 Reset_run(void);
void Waiting_dj(void);
void P2P(u16 x1,u16 y1);

#endif

