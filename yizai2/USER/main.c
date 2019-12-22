#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "key.h"
#include "exti.h"
#include "FreeRTOS.h"
#include "task.h"
#include "relay.h"
#include "ekey.h"
#include "usmart.h"
#include "usart3.h"
#include "yizai2.h"
#include "rs485.h"
#include "semphr.h"
#include "Big_motor.h"
#include "can.h"

extern s32 X; 				  //此时的实际坐标
extern s32 Z;          //定义十字在最顶端时，Z的值为0

extern long Real_Position_Value[4];
extern u8 yizai_flag;
extern u8 TIM2_IRQ;
extern TaskHandle_t YZTask2_Handler;
/************************************************
 ALIENTEK 精英STM32F103开发板 FreeRTOS实验6-3
 FreeRTOS任务挂起和恢复实验-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		100  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//USMART任务优先级
#define USMART_TASK_PRIO		2
//任务堆栈大小	
#define USMART_STK_SIZE 		100 
//任务句柄
TaskHandle_t UsmartTask_Handler;
//任务函数
void usmart_task(void *pvParameters);

//限位开关扫描任务优先级
#define EKEY_TASK_PRIO		3
//限位开关扫描任务堆栈大小	
#define EKEY_STK_SIZE 		100  
//限位开关扫描任务句柄
TaskHandle_t EKeyTask_Handler;
//限位开关扫描任务函数
void ekey_task(void *pvParameters);

////任务优先级
//#define TASK2_TASK_PRIO		5
////任务堆栈大小	
//#define TASK2_STK_SIZE 		100  
////任务句柄
//TaskHandle_t Task2Task_Handler;
////任务函数
//void task2_task(void *pvParameters);



//脉冲个数任务优先级
#define PLUS_TASK_PRIO		6
//脉冲个数任务堆栈大小	
#define PLUS_STK_SIZE 		100  
//脉冲个数任务句柄
TaskHandle_t PLUS_Task_Handler;
//脉冲个数任务函数
void plus_task(void *pvParameters);
extern u8 yizai2_runing;//移栽自动程序标志位

//二值信号量句柄
SemaphoreHandle_t BinarySemaphore;	//二值信号量句柄

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	 
	uart_init(115200);					//初始化串口
	LED_Init();		  					//初始化LED
	KEY_Init();							//初始化按键
	EKEY_GPIO();
	CAN1_Configuration(); 
	usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART	
	RS485_Init(115200);	//初始化RS485
	TIM5_Int_Init(4999,7199);
	//创建开始任务
  xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
  vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
		taskENTER_CRITICAL();           //进入临界区

		//创建USMART任务
		xTaskCreate((TaskFunction_t )usmart_task,             
                (const char*    )"usmart_task",           
                (uint16_t       )USMART_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USMART_TASK_PRIO,        
                (TaskHandle_t*  )&UsmartTask_Handler);   
//    //创建TASK2任务
//    xTaskCreate((TaskFunction_t )task2_task,     
//                (const char*    )"task2_task",   
//                (uint16_t       )TASK2_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )TASK2_TASK_PRIO,
//                (TaskHandle_t*  )&Task2Task_Handler);
    //创建ekey任务，外部限位触发
    xTaskCreate((TaskFunction_t )ekey_task,     
                (const char*    )"ekey_task",   
                (uint16_t       )EKEY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )EKEY_TASK_PRIO,
                (TaskHandle_t*  )&EKeyTask_Handler);								

	//脉冲个数任务													
	xTaskCreate((TaskFunction_t )plus_task,     
              (const char*    )"plus_task",   
              (uint16_t       )PLUS_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )PLUS_TASK_PRIO,
              (TaskHandle_t*  )&PLUS_Task_Handler);	
	//移栽任务													
	xTaskCreate((TaskFunction_t )yizai2,     
              (const char*    )"yz2_task",   
              (uint16_t       )YZ2_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )YZ2_TASK_PRIO,
              (TaskHandle_t*  )&YZTask2_Handler);						
	vTaskSuspend(YZTask2_Handler);
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//key任务函数
void usmart_task(void *pvParameters)
{
	
	while(1){	
		usmart_dev.scan();	//执行usmart扫描		
		vTaskDelay(100);			//延时10ms 
	}
}

//ekey任务函数不断扫描是否有限位开关触发以及其他导致情况，需要移栽机紧急制动停止的处理
void ekey_task(void *pvParameters)
{
	u8 statflag=0,keys;	
	Relay_Init();
	while(1)
	{
		keys=EKEY_Scan(1);//扫描哪个限位开关触发
		switch(keys)
		{
			case 0:
				if(statflag==0)
				{
					statflag=1;
					printf("{\"kp\":\"9\"}\r\n");
				}
				break;
			case EKEY0_PRES :
				statflag=0;
				printf("{\"kp\":\"0\"}\r\n");
				break;
			case EKEY1_PRES :
				statflag=0;
				printf("{\"kp\":\"1\"}\r\n");
				break;
			case EKEY2_PRES :
				statflag=0;
				printf("{\"kp\":\"2\"}\r\n");
				break;
			case EKEY3_PRES :
				statflag=0;
				printf("{\"kp\":\"3\"}\r\n");
				break;
			case EKEY4_PRES :
				statflag=0;
				printf("{\"kp\":\"4\"}\r\n");
				break;
			case EKEY5_PRES :
				statflag=0;
				printf("{\"kp\":\"5\"}\r\n");
				break;	
		}
		vTaskDelay(10);			//延时10ms 
	}	
}


//task2任务函数
//void task2_task(void *pvParameters)
//{
//	u8 task2_num=0;
//	while(1)
//	{
//		task2_num++;	//任务2执行次数加1 注意task1_num2加到255的时候会清零！！
////    LED1=!LED1;
//    vTaskDelay(100);                           //延时1s，也就是1000个时钟节拍	
//	}
//}
//脉冲个数任务函数
void plus_task(void *pvParameters)
{
	u32 pnm;
	long long_pos=0,long_pos1=0;	
	while(1)
	{
//		if(!TIM2_IRQ &&pnm!=TIM_GetCounter(TIM2))
//		{
//				pnm=TIM_GetCounter(TIM2);//发出的脉冲个数	
//				printf("{\"plusabs\":\"%d\"}\r\n",pnm); 
//				vTaskDelay(15);
//				switch(PLUS_DIR)
//				{
//					case LEFT:
//						printf("{\"plusX\":\"%d\"}\r\n",X-pnm);
//						break;
//					case RIGHT:
//						printf("{\"plusX\":\"%d\"}\r\n",X+pnm); 
//						break;
//					case UP:
//						printf("{\"plusZ\":\"%d\"}\r\n",Z-pnm);
//						break;
//					case DOWN:
//						printf("{\"plusZ\":\"%d\"}\r\n",Z+pnm); 
//						break;
//					default:
//						break;
//				}
//				
//		}					
//		if(long_pos!=Real_Position_Value[0])
//		{
//				long_pos=	Real_Position_Value[0];
//				printf("{\"lpos1\":\"%ld\"}\r\n",long_pos);			
//		}
//		else if(long_pos1!=Real_Position_Value[1])
//		{
//				long_pos1=	Real_Position_Value[1];
//				printf("{\"lpos2\":\"%ld\"}\r\n",long_pos1);			
//		}
		vTaskDelay(10);		
	}
}


