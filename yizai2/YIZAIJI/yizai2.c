#include "yizai2.h"
#include "Big_motor.h"
#include "can_motor.h"
#include "advance_time.h"
#include "usart.h"
#include "can.h"
#include "key.h"
#include "string.h"
#include "led.h"
#include "ekey.h"
#include "beep.h"   //蜂鸣器库
#include "malloc.h"
#include "relay.h"
//移栽任务句柄
TaskHandle_t YZTask2_Handler;
/*******全局参数定义*******/
extern long Real_Position_Value[4];  //大伺服完成标志
extern u8 TIM2_IRQ;
//单位为脉冲个数
extern s32 X; 			  	 //此时的实际坐标脉冲个数
extern u32 Y;						 //此时的实际坐标脉冲个数
extern s32 Z;


u16 xiao_v=1500;

u8  yizai2_hang=8,yizai2_lie=12;//移栽的行数和列数
u8  yudingwei_flag;//必须执行预定位程序否则不能运行自动程序
u8  L=30;//苗盘穴之间的间距
u16 HZ=0;//每次需要在垂直方向运行的距离，也是移栽机相对苗盘的最大提升高度
u16 F_Z_f=1000;//大伺服的运行频率
u8 yizai2_runing=1;//移栽自动程序标志位
u8 dj_x=0,dj_y;  //等级识别的X、Y位置
u8 yizai2_flag=0;	 //任务创建标志位
u16 D1X=500,D2X=500;//D0X苗识别的X位置，D0Y苗识别的Y位置,D1X一级苗盘距离原点的位置，D2X二级苗盘距离原点的位置
#define  D0X (yizai2_hang*L+50)
#define  D0Y ((yizai2_lie-1)*L/2)
#define  M_DJ  miao_dj()
/*
1.首先回到初始好的位置
2.运行移苗程序
3.一盘移栽结束后，回到初始位置准备移栽下一盘苗
4.dasifu_daowei(void)函数是确保大伺服电机在运行时不会被其他的任务打断
1000000/F_Z_f是换算后的频率值，move_x((yizai2_hang+1)*L,1000000/F_Z_f);这样可以随时改变下一次的运行频率。
*/
void chushi_weizhi()//回到初始的位置
{
//	move_z(HZ,1000000/F_Z_f);
//	dasifu_daowei();
//	move_x((yizai2_hang+1)*L*44,1000000/F_Z_f);
//	move_y(xiao_v,0);
//	dasifu_daowei();//X和Y可以联动
}


void LowerToCap(u8 *str,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		if((96<str[i])&&(str[i]<123))	//小写字母
		str[i]=str[i]-32;				//转换为大写
	}
}
//等待识别苗等级
u8 miao_dj()
{
	u16 len;
	u8 *CommandStr;
	while(1)
	{
		vTaskDelay(500);
		if(USART_RX_STA&0x8000)
		{				
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				CommandStr=mymalloc(SRAMIN,len+1);				//申请内存
				sprintf((char*)CommandStr,"%s",USART_RX_BUF);
				CommandStr[len]='\0';							//加上字符串结尾符号
				LowerToCap(CommandStr,len);				//将字符串转换为大写
				USART_RX_STA=0;
				if(strcmp((char*)CommandStr,"DJ1")==0)
				{		
						memset(USART_RX_BUF,0,USART_REC_LEN);			//串口接收缓冲区清零
						myfree(SRAMIN,CommandStr);						//释放内存
						return DJ1miao;
				}
				else if(strcmp((char*)CommandStr,"DJ2")==0) 
				{
						memset(USART_RX_BUF,0,USART_REC_LEN);			//串口接收缓冲区清零
						myfree(SRAMIN,CommandStr);						//释放内存
						return DJ2miao;
				}
		}
		else
		{
				printf("未接收到等级数据\r\n");
		}		
	}
}

void yizai2(void *pvParameters)//移栽自动程序
{
	chushi_weizhi();	
	u32 D1x=0,D1y=0,D1hang=0,D1lie=0;
	u32 D2x=0,D2y=0,D2hang=0,D2lie=0;
	u16 Dx,Dy;
	for(u8 hang=yizai2_hang;hang>=0;hang--)
	{
		for(u16 lie=0;lie<=yizai2_lie;lie++)
		{
			Dx=hang*L*44;             //计算x坐标
			Dy=lie*L*404;             //计算y坐标
			move_x(Dx,1000000/F_Z_f);     //移动到绝对位置Dx处
			move_y(xiao_v,Dy);                //移动DAO绝对位置Dy处
			dasifu_daowei();           //等待大伺服到位Freertos所有的任务调度也发生在这个函数内部
			move_z(0,1000000/F_Z_f);      //向下运动HZ的距离
			dasifu_daowei();								//等待大伺服到位		
			move_x((hang-1)*L*44,1000000/F_Z_f);  //向前移动一个穴的距离,
			dasifu_daowei();               //大伺服到位
			can_set_relay_out(0,1,1);  //一号气动手，取苗
			printf("抓苗\r\n");
			vTaskDelay(300);
			move_z(HZ,1000000/F_Z_f);     //气动手上移
			dasifu_daowei();           //等待大伺服到位Freertos所有的任务调度也发生在这个函数内部
			move_x(D0X*44,1000000/F_Z_f); //运行到识别位置的X位置;
			printf("识别区\r\n");
			move_y(xiao_v,D0Y*404);           //运行到识别位置的Y位置
			dasifu_daowei();  
			switch(M_DJ)          //等待识别苗等级
			{
				case DJ1miao://一级苗
					D1x=D1hang*L+D1X;      //计算一级苗盘的X位置
					D1y=D1lie*L;					 //计算一级苗盘的Y位置
//					printf("第一个苗盘D1X=%d\r\n",D1x);
					move_x(D1x*44,1000000/F_Z_f);
					move_y(xiao_v,D1y*404);
					dasifu_daowei();
					move_z(0,1000000/F_Z_f);
					dasifu_daowei();
					can_set_relay_out(0,1,0);  //一号气动手，放苗
					printf("放苗\r\n");
					vTaskDelay(300);
					move_z(HZ,1000000/F_Z_f);//气动手上升HZ
					dasifu_daowei();
					D1lie++;
					if(D1lie==yizai2_lie)   //一级苗盘的列已经装满
					{
						D1lie=0;
						D1hang++;
						if(D1hang==yizai2_hang)//一级苗盘已经装满
						{
							D1hang=0;
							/*
								一级苗盘的输送带运动，将下一个空苗盘运送到指定位置
							*/
						}
					}
					break;
				case DJ2miao://二级苗
					D2x=D2hang*L+D2X;      //计算一级苗盘的X位置
					D2y=D2lie*L;					 //计算一级苗盘的Y位置
					move_x(D2x*44,1000000/F_Z_f);
					move_y(xiao_v,D2y*404);
					dasifu_daowei();
					move_z(0,1000000/F_Z_f);
					dasifu_daowei();
					can_set_relay_out(0,1,0);  //一号气动手，放苗
					vTaskDelay(500);
					move_z(HZ,1000000/F_Z_f);
					dasifu_daowei();
					D2lie++;
					if(D2lie==yizai2_lie)   //二级苗盘的列已经装满
					{
						D2lie=0;
						D2hang++;
						if(D2hang==yizai2_hang)//二级苗盘已经装满
						{
							D2hang=0;
							/*
								二级苗盘的输送带运动，将下一个空苗盘运送到指定位置
							*/
						}
					}
					break;
			}
		}
	}
	//要移栽的苗盘已经完成
}
void yizai2_shoudong(void)
{
	vTaskDelete(YZTask2_Handler); //删除开始任务
}




void yizai2_restart(void)//重启自动程序
{
	vTaskDelete(YZTask2_Handler); //删除开始任务
	taskENTER_CRITICAL();//进入临界区，运行保护
	//创建yizai任务
  xTaskCreate((TaskFunction_t )yizai2,     
              (const char*    )"yz2_task",   
              (uint16_t       )YZ2_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )YZ2_TASK_PRIO,
              (TaskHandle_t*  )&YZTask2_Handler); 
	vTaskSuspend(YZTask2_Handler);//挂起自动移栽任务
	taskEXIT_CRITICAL();          //退出临界区	
}
//移栽暂停和重新启动程序
void yizai2_stop(u8 yn)
{
	if(yn)//恢复程序
	{		
		vTaskResume(YZTask2_Handler);	//恢复任务1
		yizai2_runing=0;
		return;
	}
	else 
	{
		vTaskSuspend(YZTask2_Handler);//挂起任务
		yizai2_runing=1;
		return;
	}		
}
//设置苗盘参数的时候，自动程序不能运行
void piaoman_canshu(u8 hang,u8 lie,u8 l,u16 d1x,u16 d2x)//设置苗盘参数
{
	if(yizai2_runing)
	{
		yizai2_hang=hang;
		yizai2_lie =lie;
		L=l;
		D1X=d1x;
		D2X=d2x;
		printf("苗盘参数：%d行,%d列,%d穴间距,D1X=%d,D2X=%d\r\n",yizai2_hang,yizai2_lie,L,D1X,D2X);	
	}
	else
	{
		printf("自动程序正在执行\r\n");
	}
}

void set_yizai2_F(u16 i)//设置大伺服的运行速度即频率f
{
	F_Z_f=i;
}

void xiaosifu_v(u16 XV)
{
	xiao_v=XV;
}//判断大伺服是否发送完脉冲
void dasifu_daowei(void)//判断大伺服是否到位，如果不到位不能进行其他的操作
{
	vTaskDelay(500);
	while(1)
	{
		vTaskDelay(500);
		if(TIM2_IRQ) 
		{
			printf("(%d,%d,%d)\r\n",X/44,Y/404,Z/44);
			return;
		}		
	}
}

//判断小伺服是否到达指定位置
void xiaosifu_daowei(long Ys)
{
	Now_open();
	while(1)
		{
			vTaskDelay(10);
			if(Ys>(Real_Position_Value[0]+500) || Ys<(Real_Position_Value[0]+500))//误差1mm
			{
				Now_close();
				return;
			}
		}			
}
//相对位置运行，先上升HZ毫米，再向左运行L毫米
void yudingwei(u16 h)//对准一个苗盘后的操作，记录需要在垂直方向上运行的距离
{//默认HZ=0,如果小于50mm,则强行设置为50mm
	if(h>=50)//设置的最小上升距离50mm
	{
		HZ=h*44;
	}
	else
	{
		HZ=50*44;
	}		
	Can_rest();Z=HZ; //此时的实际坐标脉冲个数
	{UP_MOVE Pulse_output(1000,HZ);}//上移h毫米
	dasifu_daowei(); //大伺服是否到位
	printf("上移%d\r\n",h);
	X=yizai2_hang*L*44;
	{LEFT_MOVE Pulse_output(1000,L*44);}//左移一个苗盘穴的距离	
	dasifu_daowei(); //大伺服是否到位
	printf("左移%d\r\n",L);
	
	
}

void START_DSF(u8 K_G)
{
	switch(K_G)
	{
		case 1:
			Relay_test(1,1);//先打开大伺服的电源
			vTaskDelay(2000);//等待一段时间。让伺服驱动器完全通电
			Relay_test(2,1);//大伺服没有抱闸
			Relay_test(3,1);
			return;
		case 0:
			Relay_test(2,0);//大伺服抱闸
			Relay_test(3,0);
			vTaskDelay(2000);//等待一段时间。让伺服驱动器完全通电
			Relay_test(1,0);//短开大伺服的电源
			return;
	}	
}
