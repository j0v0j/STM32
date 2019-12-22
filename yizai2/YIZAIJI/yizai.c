#include "rs485.h"
#include "yizai.h"
#include "Big_motor.h"
#include "can_motor.h"
#include "advance_time.h"
#include "usart.h"
#include "can.h"
#include "yizai.h"
#include "delay.h"
#include "usart3.h"
#include "key.h"
#include "string.h"
#include "led.h"
#include "ekey.h"
#include "remote.h"
#include "beep.h"   //蜂鸣器库
#include "byhandle.h"

extern u8 TIM2_IRQ;  //脉冲发送完成标志
extern long Real_Position_Value[4];  //大伺服完成标志
extern short Real_Velocity_Value[4];
extern u8 mx[3],my[3],dj;
extern u8 RS485_RX_CNT;
extern u8 RS485_flag;
u8 yizai_flag=1;
//任务句柄
TaskHandle_t YZTask_Handler;


/***************************************************		
	大伺服脉冲和距离换算   x=n*0.0226    
	小伺服qc和距离换算   y=n*0.00245    
	l=45mm，大伺服脉冲个数1991    小伺服qc=18367
	大伺服1mm对应44脉冲(估算)           小伺服1mm对应408qc(估算)
****************************************************/

//u16  LX=4400;  	//原始苗盘第一空到原点100mm
//u32  LY=40800;  //原始苗盘第一空到原点的长度100mm
//u32  D1=88000;   //一级苗盘第一空到原点的长度2000mm
//u32 D2=132000;   //二级苗盘第一空到原点的长度3000mm
//u16  ll=18360;  	 //横移苗穴间距45mm
//u16  l=1980;  	 //十字苗穴间距45mm
u16  LX=0;  	//原始苗盘第一空到原点100mm
u32  LY=0;  //原始苗盘第一空到原点的长度100mm
u32  D1=0;   //一级苗盘第一空到原点的长度2000mm
u32 D2=0;   //二级苗盘第一空到原点的长度3000mm
u16  ll=18360;  	 //横移苗穴间距45mm
u16  l=1980;  	 //十字苗穴间距45mm
u8 St_init=0;		//苗盘参数初始化标志位

//单位为脉冲个数
extern s32 X; 			  	 //此时的实际坐标脉冲个数
extern u32 Y;						 //此时的实际坐标脉冲个数
extern s32 Z;
u8 Reset_flag=0;
u8 i_1=0,j_1=0;			 //一级苗的行列
u8 i_2=0,j_2=0;			 //二级苗的行列
u32 x_0=0,y_0=0;			 //原始苗的行列
u32 x_1=0,y_1=0;			 //一级苗的坐标
u32 x_2=0,y_2=0;			 //二级苗的坐标
u32 Dx;							 //大伺服X方向移动距离
u32 Dy;							 //小伺服移动距离
u32 DZ=8800;	
u16 F_S=10000;
u8 hang=6,lie=12;
/*起点相对距离函数的初始化
void Init_number(void); 查看初始化的函数        
void Start_init(u32 lx,u32 ly,u32 d1,u32 d2,u32 dz);初始化函数
void Clear_st(void);重新初始化
void Set_F(u16 f);设置脉冲频率65535
u8 Limit_key(void);查看限位
u8 Reset_run(void);复位
void yizai_run(void);移栽自动
void P2P(u16 x1,u16 y1);从现在位置移动到目标位置
*/

//void Set_F(u16 f)
//{ //频率换算
//	if(f<=0 | f>=30000)//如果频率设置为0，或者大于30kHz那么就打开脉冲刹车
//	{
//		TIM_BREAK=1;
//		printf("break_plus");
//	}
//	else
//	{
//		F_S=f;
//		printf("plus=%d",F_S);
//	}
//	
//}

void Init_number(void)
{
	printf("\r\n苗盘LX=%dmm\r\n",LX/44);
	printf("LY =%dmm\r\n",LY/408);
	printf("D1 =%dmm\r\n",D1/44);
	printf("D2 =%dmm\r\n",D2/44);
	printf("DZ =%dmm\r\n",DZ/44);
	printf("F_S=%dmm\r\n",F_S);	
}

void Set_miaopan(u8 detal,u8 C, u8 R)
{
	l=detal*44;
	ll=detal*408;
	hang =C;
	lie  =R;
}

void Start_init(u32 lx,u32 ly,u32 z,u32 d1,u32 d2,u32 dz)//初始化苗盘LX,LY,d1,d2
{
	if(!St_init)
	{	//将实际的距离转化为脉冲或者qc
		LX=44*lx;
		LY=408*ly;
		D1=44*d1;
		D2=44*d2;
		DZ=44*dz;
		Z=z;
		can_set_relay_out(0,1,0);//气动手收起
		
		St_init=1;	
		
	}
	else
	{
		printf("未置位,Clear_st()\r\n");
	}
	
}


void Clear_st(void)//清除初始化参数
{
	St_init=0;
}


void CrossReady(void)//十字到位判断
{
	while(1)
	{
		
		if(TIM2_IRQ)
			{
				printf("大伺服到位！\r\n");
				return;
			}
	}
}

void HyReady(void)//小伺服到位判断
{	
	Now_open();
	vTaskDelay(10);
//	delay_xms(1000);
	while(1)
		{
//			u3_printf("{\"p1\":%ld}\r\n",Real_Position_Value[0]);
//			vTaskDelay(100);
//			u3_printf("{\"v1\":%ld}\r\n",Real_Velocity_Value[0]);	
			if(Dy>(Real_Position_Value[0]+500) || Dy<(Real_Position_Value[0]+500))//误差1mm
			{
				printf("小伺服到位！%ld\r\n",Real_Position_Value[0]);
//				u3_printf("{\"p1\":%ld}\r\n",Real_Position_Value[0]);
				vTaskDelay(100);
//				u3_printf("{\"v1\":%ld}\r\n",Real_Velocity_Value[0]);	
				Now_close();
				return;
			}
		}			
}
//一步一步的按键执行，当按下WKUP键执行下一动作
void key_enter(void)
{
//	u8 keys;
//	printf("\r\nWKUP_PRES continue\r\n");
	while(1)
		{
			delay_ms(1000);
			break;
//			keys=Remote_Scan();	
//			BEEP=1;
//			if(WKUP_PRES==KEY_Scan(0) || 162==Remote_Scan())
//			{
//				BEEP=0;
//				printf("WKUP_PRES press\r\n");
//				break;
//			}
//			vTaskDelay(100);
		}
}
//下面是检查限位开关是否正常
//ps:先这样写着，以后再想怎么写比较好，if  else  太多了。
u8 Limit_key(void)
{
	if(EKEY0)
	{
//		u3_printf("{\"break0\":1}\r\n");
		printf("\r\nlimit Right ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break0\":0}\r\n");
		printf("\r\nlimit Right err.\r\n");
		
	}
	if(EKEY1)
	{
//		u3_printf("{\"break1\":1}\r\n");
		printf("\r\nlimit Up ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break1\":0}\r\n");		
		printf("\r\nlimit Up err.\r\n");
	}
	if(EKEY2)
	{
//		u3_printf("{\"break2\":1}\r\n");
		printf("\r\nlimit Left ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break2\":0}\r\n");
		printf("\r\nlimit Left err.\r\n");
	}
	if(EKEY3)
	{
//		u3_printf("{\"break3\":1}\r\n");
		printf("\r\nlimit Down ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break3\":0}\r\n");
		printf("\r\nlimit Down err.\r\n");
	}
	if(EKEY4)
	{
//		u3_printf("{\"break4\":1}\r\n");
		printf("\r\nLittle serve ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break4\":0}\r\n");
		printf("\r\nLittle serve err.\r\n");
	}
	if(EKEY5)
	{
//		u3_printf("{\"break5\":1}\r\n");
		printf("\r\nLittle serve ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break5\":0}\r\n");
		printf("\r\nLittle serve err.\r\n");
	}
	if(EKEY5 && EKEY4 && EKEY3 && EKEY2 && EKEY1 && EKEY0)
		return 1;
	else
		return 0;
}
//限位开关在未触发时将外部中断IO的电平拉高,在触发限位后电平拉低（IO为内部下拉）
u8 Reset_run(void)
{	 
	Reset_flag=Limit_key();  //对限位开关状态进行检验	
	if(Reset_flag)//限位开关正常
	{//必须让十字先向上运行接触到限位
		Turn1=RESET;
		Turn2=RESET;
		Pulse_output(100,10000);
		//接着让十字先右走接触到限位
		while(1)//当十字运行到上限位后，先向下运行一段距离，，，再执行向右运行操作
		{
			if(!EKEY1)	//等待到达上限位
			{
				printf("\r\nlimit UP！\r\n");

				Turn1=SET;  //向下运行一段距离
				Turn2=SET;
				vTaskDelay(1000);
				Pulse_output(100,8800);//
				while(1)       //离开了限位位置
				{
					if(EKEY1)
					{
						TIM_dis();
						printf("reset up_down\r\n");
						break;
					}
						vTaskDelay(100);
					
				}
				
				//向右运行一段距离
				vTaskDelay(1000);				
				Turn1=RESET;
				Turn2=SET;
				Pulse_output(100,30000);	
				break;
			}
			vTaskDelay(100);
		}
		
		while(1)	//右走操作是否完成
		{
			if(!EKEY0) 	//等待到达右限位
			{ //向左运行一段距离
				printf("\r\nlimit RIGHT！\r\n");
				vTaskDelay(1000);				
				Turn1=SET;
				Turn2=RESET;
				Pulse_output(100,8800);
				while(1)       //离开了限位位置
				{ 
					if(EKEY0)
					{
						TIM_dis();
						printf("reset right_left\r\n");
						break;
					}
					vTaskDelay(100);				
				}
				
				break;
			}
			vTaskDelay(100);
		}
		Reset_flag=2;
		printf("\r\nReset OK\r\n");
		return 1;
	}
	else
	{
		printf("\r\nLimit key err.\r\n");
		return 0;
	}
}


u8 ZB[]={0};//RS485的发送数据buffer
//苗识别等待程序
void Waiting_dj()
{
	u32 t=0;
	u8 keys=0;
	//下边的这个while是等待串口发送来的苗等级
	//得到苗等级,苗的等级存储在变量“dj ”里,
	while(1)
	{//这是一段等待识别完苗等级的程序，每隔一段时间执行一次第二个if语句
		t++;
		keys=0;
		vTaskDelay(100);
		keys=Remote_Scan();
		switch(keys)
		{
						case 104: //遥控1键
							dj=1;
							RS485_flag=0;
							return;
						case 152: //遥控2键
							dj=2;
							RS485_flag=0;
							return;
		}
	  if(RS485_flag==0)
		{
						RS485_flag=1;
						printf("苗等级接收完成\r\n");
//						u3_printf("{\"p3\":\"苗等级接收完成\"}\r\n");
						return;
		}
		if (t==20)
		{    
			t=0;
			sprintf((char*)ZB,"DJ\r\n");
			RS485_Send_Data(ZB,strlen((char*)ZB));
			printf("等待识别苗等级\r\n");				
			}
		}
}



void yizai_task(void *pvParameters)
{
//	u8 t_num=0;
	while(1)
	{
	if(St_init)
	{
		move_z(DZ,1000000/F_S);			//十字从高处开始执行程序运行150mm
		for(u8 j=0;j<hang;j++)		//行循环
		{
			x_0=LX+j*l;//脉冲运算
			for(u8 i=0;i<lie;i++)//列循环
			{		
				sprintf((char*)ZB,"{\"X\":%d,\"Y\":%d}\r\n",j,i);
				RS485_Send_Data(ZB,strlen((char*)ZB));
				//计算原始苗盘的起点坐标				
				y_0=LY+i*ll;//脉冲运算
				//移动到起点位置
				Dx=x_0;//计算大伺服电机需要移动的距离
				move_x(Dx,1000000/F_S);//换算成脉冲个数
				CrossReady();//大伺服是否到位
				Dy=y_0;//计算小伺服电机需要移动的距离	
				move_y(Dy);							
				HyReady();		//小伺服是否到位	
				printf("\r\n抓苗前位置X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
				key_enter();
				move_z(DZ,1000000/F_S);//大十字向下运动准备夹苗
				CrossReady();//大伺服是否到位
				printf("\r\n抓苗时位置X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
				//****************************************************************
				//****************************************************************
				key_enter();
				can_set_relay_out(0,1,1);//气动手，夹取苗
				printf("气动手爪抓苗\r\n");	
				key_enter();
				move_z(0,1000000/F_S);//大十字向上运动
				CrossReady();//大伺服是否到位
				printf("\r\n抓苗后位置X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);	
				Waiting_dj();
				//RS485在中断中接收数据
				//Waiting_dj()的while循环退出说明苗等级识别已经完成
				key_enter();
//				u3_printf("{\"m1\":%d}\r\n",dj);
				switch(dj) 
				{
							
					case 1://一级苗的情况
						//计算一级苗盘的移栽点
							printf("一级苗\r\n");
							x_1=D1+j_1*l;					//计算一级苗盘的坐标
							y_1=LY+i_1*ll;					
						//移动到目标位置需要的距离
						Dx=x_1;
						Dy=y_1;
						move_x(Dx,1000000/F_S);
						CrossReady(); 		//大伺服是否到位
						move_y(Dy);
						HyReady();				//小伺服是否到位					
						//准备放苗
						move_z(DZ,1000000/F_S);				//大十字向下运动准备放苗
						CrossReady(); 		//大伺服放苗是否到位
						printf("\r\n放苗时位置X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
						key_enter();
						can_set_relay_out(0,1,0);//气动手，放苗
//						u3_printf("{\"m1y\":%d,\"m1x\":%d}\r\n",i_1,j_1);
						printf("\r\n气动手放苗\r\n");
						key_enter();									
						move_z(0,1000000/F_S);			//大十字向上运动准备
						CrossReady(); 		//大伺服向上是否到位
							i_1++;          	//为进入下一行准备
						//一级苗盘按顺序摆放
						if(i_1%lie==0)   	  	//由于从0开始计数，如果下一次循环是12说明此列已满
						{
							j_1++;			  	//进入下一列
							if(j_1==hang)	  	//一级苗盘满
							{
								u3_printf("{\"m1y\":188,\"m1x\":188}\r\n");
								j_1=0;					
							}
							i_1=0;
						}						
						
						printf("\r\n放苗后位置X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
						key_enter();
						break;
					case 2://二级苗的情况
						//计算一级苗盘的移栽点
							printf("\r\n二级苗");
							x_2=D2+j_2*l;
							y_2=LY+i_2*ll;

						Dx=x_2;
						Dy=y_2;
						move_x(Dx,1000000/F_S);
						CrossReady(); //大伺服是否到位
						move_y(Dy);
						HyReady();		//小伺服是否到位									
						//准备放苗
						move_z(DZ,1000000/F_S);//大十字向下运动准备放苗
						CrossReady(); //大伺服放苗是否到位
						printf("\r\n放苗时位置X=%d,Y=%d,Z=%d",X/44,Y/408,Z/44);
						key_enter();
						can_set_relay_out(0,1,0);//气动手，放苗
						u3_printf("{\"m2y\":%d,\"m2x\":%d}\r\n",i_2,j_2);
						key_enter();
						move_z(0,1000000/F_S);//大十字向上运动准备
						CrossReady(); //大伺服向上是否到位
						i_2++;
						if(i_2%lie==0)
						{
							j_2++;
							if(j_2==hang)//二级苗盘满
							{
								u3_printf("{\"m2y\":188,\"m2x\":188}\r\n");
								j_2=0;					
							}
							i_2=0;
						}
						printf("\r\n放苗后位置X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
						key_enter();
						break;
				}
			}
			//原始的一组苗移栽完成	
		}
		//原始的一盘苗移栽完成
//		u3_printf("{\"my\":188,\"mx\":188}\r\n");
	}
	else
	{
//		t_num++;
//		printf("t_num=%d\r\n",t_num);
		if(!St_init)
			printf("\r\n请先进行苗盘初始化");	
		if(Reset_flag!=2)
			printf(",复位失败");
		vTaskDelay(500);	
	}
	vTaskDelay(1000);			//延时10ms 	

}
	}

//自动程序恢复
void yizai_run(void *pvParameters)//移栽机自动程序
{
	vTaskResume(YZTask_Handler);	//恢复任务1
	yizai_flag=0;
	printf("\r\n恢复移栽的运行!");
}
//自动程序暂停
void yizai_stop(void *pvParameters)//移栽机自动程序
{
	vTaskSuspend(YZTask_Handler);//挂起任务
	yizai_flag=1;
	printf("\r\n挂起移栽的运行!");
}
//自动程序重新启动,删除任务->新建任务
void yizai_restart(void)
{
	vTaskDelete(YZTask_Handler); //删除开始任务
//	vTaskDelay(1000);
	taskENTER_CRITICAL();//进入临界区，运行保护
		//创建yizai任务
  xTaskCreate((TaskFunction_t )yizai_task,     
              (const char*    )"yz_task",   
              (uint16_t       )YZ_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )YZ_TASK_PRIO,
              (TaskHandle_t*  )&YZTask_Handler); 
	vTaskSuspend(YZTask_Handler);//挂起自动移栽任务
	taskEXIT_CRITICAL();            //退出临界区						
}

//从一个现在位置运动到目标的（x1,y1）
void P2P(u16 x1,u16 y1)
{
	Dx=408*y1;
	Dy=44*x1;
	move_z(0,1000000/F_S);//如果大十字先向上运动准备	
	CrossReady(); //大伺服是否到位
	move_x(Dx,1000000/F_S);
	vTaskDelay(500);
	move_y(Dy);
	CrossReady(); //大伺服是否到位
	HyReady();		//小伺服是否到位
	move_z(DZ,1000000/F_S);//大十字向下运动准备放苗
	CrossReady(); //大伺服是否到位
}




