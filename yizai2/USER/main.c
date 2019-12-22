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

extern s32 X; 				  //��ʱ��ʵ������
extern s32 Z;          //����ʮ�������ʱ��Z��ֵΪ0

extern long Real_Position_Value[4];
extern u8 yizai_flag;
extern u8 TIM2_IRQ;
extern TaskHandle_t YZTask2_Handler;
/************************************************
 ALIENTEK ��ӢSTM32F103������ FreeRTOSʵ��6-3
 FreeRTOS�������ͻָ�ʵ��-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		100  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//USMART�������ȼ�
#define USMART_TASK_PRIO		2
//�����ջ��С	
#define USMART_STK_SIZE 		100 
//������
TaskHandle_t UsmartTask_Handler;
//������
void usmart_task(void *pvParameters);

//��λ����ɨ���������ȼ�
#define EKEY_TASK_PRIO		3
//��λ����ɨ�������ջ��С	
#define EKEY_STK_SIZE 		100  
//��λ����ɨ��������
TaskHandle_t EKeyTask_Handler;
//��λ����ɨ��������
void ekey_task(void *pvParameters);

////�������ȼ�
//#define TASK2_TASK_PRIO		5
////�����ջ��С	
//#define TASK2_STK_SIZE 		100  
////������
//TaskHandle_t Task2Task_Handler;
////������
//void task2_task(void *pvParameters);



//��������������ȼ�
#define PLUS_TASK_PRIO		6
//������������ջ��С	
#define PLUS_STK_SIZE 		100  
//�������������
TaskHandle_t PLUS_Task_Handler;
//�������������
void plus_task(void *pvParameters);
extern u8 yizai2_runing;//�����Զ������־λ

//��ֵ�ź������
SemaphoreHandle_t BinarySemaphore;	//��ֵ�ź������

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	uart_init(115200);					//��ʼ������
	LED_Init();		  					//��ʼ��LED
	KEY_Init();							//��ʼ������
	EKEY_GPIO();
	CAN1_Configuration(); 
	usmart_dev.init(SystemCoreClock/1000000);	//��ʼ��USMART	
	RS485_Init(115200);	//��ʼ��RS485
	TIM5_Int_Init(4999,7199);
	//������ʼ����
  xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
  vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
		taskENTER_CRITICAL();           //�����ٽ���

		//����USMART����
		xTaskCreate((TaskFunction_t )usmart_task,             
                (const char*    )"usmart_task",           
                (uint16_t       )USMART_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USMART_TASK_PRIO,        
                (TaskHandle_t*  )&UsmartTask_Handler);   
//    //����TASK2����
//    xTaskCreate((TaskFunction_t )task2_task,     
//                (const char*    )"task2_task",   
//                (uint16_t       )TASK2_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )TASK2_TASK_PRIO,
//                (TaskHandle_t*  )&Task2Task_Handler);
    //����ekey�����ⲿ��λ����
    xTaskCreate((TaskFunction_t )ekey_task,     
                (const char*    )"ekey_task",   
                (uint16_t       )EKEY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )EKEY_TASK_PRIO,
                (TaskHandle_t*  )&EKeyTask_Handler);								

	//�����������													
	xTaskCreate((TaskFunction_t )plus_task,     
              (const char*    )"plus_task",   
              (uint16_t       )PLUS_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )PLUS_TASK_PRIO,
              (TaskHandle_t*  )&PLUS_Task_Handler);	
	//��������													
	xTaskCreate((TaskFunction_t )yizai2,     
              (const char*    )"yz2_task",   
              (uint16_t       )YZ2_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )YZ2_TASK_PRIO,
              (TaskHandle_t*  )&YZTask2_Handler);						
	vTaskSuspend(YZTask2_Handler);
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//key������
void usmart_task(void *pvParameters)
{
	
	while(1){	
		usmart_dev.scan();	//ִ��usmartɨ��		
		vTaskDelay(100);			//��ʱ10ms 
	}
}

//ekey����������ɨ���Ƿ�����λ���ش����Լ����������������Ҫ���Ի������ƶ�ֹͣ�Ĵ���
void ekey_task(void *pvParameters)
{
	u8 statflag=0,keys;	
	Relay_Init();
	while(1)
	{
		keys=EKEY_Scan(1);//ɨ���ĸ���λ���ش���
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
		vTaskDelay(10);			//��ʱ10ms 
	}	
}


//task2������
//void task2_task(void *pvParameters)
//{
//	u8 task2_num=0;
//	while(1)
//	{
//		task2_num++;	//����2ִ�д�����1 ע��task1_num2�ӵ�255��ʱ������㣡��
////    LED1=!LED1;
//    vTaskDelay(100);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
//	}
//}
//�������������
void plus_task(void *pvParameters)
{
	u32 pnm;
	long long_pos=0,long_pos1=0;	
	while(1)
	{
//		if(!TIM2_IRQ &&pnm!=TIM_GetCounter(TIM2))
//		{
//				pnm=TIM_GetCounter(TIM2);//�������������	
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


