#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "ltdc.h"
#include "lcd.h"
#include "sdram.h"
#include "pcf8574.h"
#include "mpu.h"
#include "timer.h"
#include "includes.h"
//#include "lwip_comm.h"
//#include "lwip/netif.h"
//#include "ethernetif.h"
//#include "lwipopts.h"
//#include "tcp_client_demo.h"
#include "mqttclient.h"

//#include "rs485.h"
//�����Լ��޸ĺ����ӵ�ͷ�ļ�

#include "usmart.h"
#include "fdcan.h"
#include "set_io.h"

/************************************************


************************************************/

extern struct PLUS__NUM PLUS_NUM;
extern TIM_HandleTypeDef TIM_Handle;      

#define KEY_TASK_PRIO 		8
#define KEY_STK_SIZE		128
OS_TCB KeyTaskTCB;
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
void key_task(void *pdata);


#define usmart__TASK_PRIO		9
#define usmart_STK_SIZE		128
OS_TCB usmart_TaskTCB;
CPU_STK	usmart_TASK_STK[usmart_STK_SIZE];
void usmart_thread(void *pdata);

//��LCD����ʾ��ַ��Ϣ����
//�������ȼ�
#define DISPLAY_TASK_PRIO	10
//�����ջ���?
#define DISPLAY_STK_SIZE	128
//������ƿ�?
OS_TCB DisplayTaskTCB;
//������?
CPU_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//������
void display_task(void *pdata);


#define START_TASK_PRIO		11

#define START_STK_SIZE		128

OS_TCB StartTaskTCB;

CPU_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *pdata);



#define PLUS_SHOW_PRIO	12

#define PLUS_SHOW_STK_SIZE	128

OS_TCB PLUS_SHOW_TaskTCB;

CPU_STK	PLUS_SHOW_TASK_STK[PLUS_SHOW_STK_SIZE];
void PLUS_show(void *pdata);



#define PLUS_SHOW_PRIO	12

#define PLUS_SHOW_STK_SIZE	128

OS_TCB PLUS_SHOW_TaskTCB;

CPU_STK	PLUS_SHOW_TASK_STK[PLUS_SHOW_STK_SIZE];
void PLUS_show(void *pdata);


#define plant_PRIO	13

#define plant_STK_SIZE	128

OS_TCB plant_TaskTCB;

CPU_STK	plant_TASK_STK[plant_STK_SIZE];
extern void Auto_runing(void *pdata);


void show_address(u8 mode)
{
//    u8 buf[30];
//    if(mode==2)
//    {
//        sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
//        LCD_ShowString(30,170,210,16,16,buf);
//        sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
//        LCD_ShowString(30,190,210,16,16,buf);
//        sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ����������?
//        LCD_ShowString(30,210,210,16,16,buf);
//        LCD_ShowString(30,230,210,16,16,(u8 *)"Port:8087!");
//    }
//    else
//    {
//        sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
//        LCD_ShowString(30,170,210,16,16,buf);
//        sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
//        LCD_ShowString(30,190,210,16,16,buf);
//        sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ����������?
//        LCD_ShowString(30,210,210,16,16,buf);
//        LCD_ShowString(30,230,210,16,16,(u8 *)"Port:8087!");
//    }
}

int main(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    Cache_Enable();                			//��L1-Cache
    Write_Through();
    HAL_Init();				        		//��ʼ��HAL��
    Stm32_Clock_Init(160,5,2,4);  		    //����ʱ��,400Mhz
    delay_init(400);						//��ʱ��ʼ��
    uart_init(115200);						//���ڳ�ʼ��
    uart3_init(115200);						//���ڳ�ʼ��
    PCF8574_Init();                         //��ʼ��PCF8574
    LED_Init();								//��ʼ��LED
//    KEY_Init();								//��ʼ������
    REY_io_init();//��ʼ���̵���IO��
    EXTI_Init();					//��ʼ���ⲿ�ж�����
    SDRAM_Init();                   		//��ʼ��SDRAM
    MPU_Memory_Protection();		//������ش�?����
    LCD_Init();								//��ʼ��LCD
    usmart_init(200); 			//��ʼ��USMART
    PWM_gpio();
    FDCAN1_Mode_Init(22,1,5,3,FDCAN_MODE_NORMAL);        //����ģʽ,������1Mbps
    POINT_COLOR=RED;
    LCD_ShowString(30,50,200,16,16,(u8 *)"POLARIS H750/F750");
    LCD_ShowString(30,70,200,16,16,(u8 *)"TCP Client UCOSIII Test");
    LCD_ShowString(30,90,200,16,16,(u8 *)"ATOM@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,(u8 *)"2019/5/13");
    TIM8_Init(1000);
    TIM2_Init();
    OSInit(&err); 					//UCOSIII��ʼ��
		
//    while(lwip_comm_init()) 		//lwip��ʼ��
//    {    
//				printf("lwip_comm_init()=%d\r\n",lwip_comm_init()) ;
//        LCD_ShowString(30,110,200,20,16,(u8 *)"Lwip Init failed!"); 	//lwip��ʼ��ʧ��
//        delay_ms(500);
//        LCD_Fill(30,110,230,150,WHITE);
//        delay_ms(500);
//    }
//    LCD_ShowString(30,110,200,20,16,(u8 *)"Lwip Init Success!"); 		//lwip��ʼ���ɹ�
//    while(tcp_client_init()) 									//��ʼ��tcpclient_demo(����tcpclient_demo�߳�)
//    {
//        LCD_ShowString(30,150,200,20,16,(u8 *)"TCP Client failed!!"); //tcpclient����ʧ��
//        delay_ms(500);
//        LCD_Fill(30,150,230,170,WHITE);
//        delay_ms(500);
//    }
//    LCD_ShowString(30,150,200,20,16,(u8 *)"TCP Client Success!"); 	//tcpclient�����ɹ�

    OS_CRITICAL_ENTER();//�����ٽ���x
    //������ʼ����
    OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�?
                 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ�����?
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ���?
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ�������?��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ�?��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ����?
    OS_CRITICAL_EXIT();	//�˳��ٽ���
    OSStart(&err); 		//����UCOS
}

//start����
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  	//ͳ������
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��?
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
    //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

#if LWIP_DHCP
//    lwip_comm_dhcp_creat(); //����DHCP����
#endif
//	mqtt_thread_init();
    OS_CRITICAL_ENTER();	//�����ٽ���
    //����LED����
    OSTaskCreate((OS_TCB 	* )&usmart_TaskTCB,
                 (CPU_CHAR	* )"led0 task",
                 (OS_TASK_PTR )usmart_thread,
                 (void		* )0,
                 (OS_PRIO	  )usmart__TASK_PRIO,
                 (CPU_STK   * )&usmart_TASK_STK[0],
                 (CPU_STK_SIZE)usmart_STK_SIZE/10,
                 (CPU_STK_SIZE)usmart_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
    //����KEY����
    OSTaskCreate((OS_TCB 	* )&KeyTaskTCB,
                 (CPU_CHAR	* )"key task",
                 (OS_TASK_PTR )key_task,
                 (void		* )0,
                 (OS_PRIO	  )KEY_TASK_PRIO,
                 (CPU_STK   * )&KEY_TASK_STK[0],
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,
                 (CPU_STK_SIZE)KEY_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
    //������ʾ����
//    OSTaskCreate((OS_TCB 	* )&DisplayTaskTCB,
//                 (CPU_CHAR	* )"display task",
//                 (OS_TASK_PTR )display_task,
//                 (void		* )0,
//                 (OS_PRIO	  )DISPLAY_TASK_PRIO,
//                 (CPU_STK   * )&DISPLAY_TASK_STK[0],
//                 (CPU_STK_SIZE)DISPLAY_STK_SIZE/10,
//                 (CPU_STK_SIZE)DISPLAY_STK_SIZE,
//                 (OS_MSG_QTY  )0,
//                 (OS_TICK	  )0,
//                 (void   	* )0,
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//                 (OS_ERR 	* )&err);

    //����MQTT����
    OSTaskCreate(  (OS_TCB 	  * )&PLUS_SHOW_TaskTCB,
                   (CPU_CHAR	* )"PLUS_SHOW",
                   (OS_TASK_PTR )PLUS_show,
                   (void		  * )0,
                   (OS_PRIO	    )PLUS_SHOW_PRIO,
                   (CPU_STK   * )&PLUS_SHOW_TASK_STK[0],
                   (CPU_STK_SIZE)PLUS_SHOW_STK_SIZE/10,
                   (CPU_STK_SIZE)PLUS_SHOW_STK_SIZE,
                   (OS_MSG_QTY  )0,
                   (OS_TICK	    )0,
                   (void   	  * )0,
                   (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                   (OS_ERR 	  * )&err);
									 
									 
									 
									 
   OSTaskCreate(  (OS_TCB 	  * )&plant_TaskTCB,
                   (CPU_CHAR	* )"Auto_runing",
                   (OS_TASK_PTR )Auto_runing,
                   (void		  * )0,
                   (OS_PRIO	    )plant_PRIO,
                   (CPU_STK   * )&plant_TASK_STK[0],
                   (CPU_STK_SIZE)plant_STK_SIZE/10,
                   (CPU_STK_SIZE)plant_STK_SIZE,
                   (OS_MSG_QTY  )0,
                   (OS_TICK	    )0,
                   (void   	  * )0,
                   (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                   (OS_ERR 	  * )&err);
									 								 
									 
    OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����
    OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//��ʾ��ַ����Ϣ
void display_task(void *pdata)
{
    OS_ERR err;

    while(1)
    {
//#if LWIP_DHCP									//������DHCP��ʱ��
//        if(lwipdev.dhcpstatus != 0) 			//����DHCP
//        {
//            show_address(lwipdev.dhcpstatus );	//��ʾ��ַ��Ϣ
//            OS_TaskSuspend((OS_TCB*)&DisplayTaskTCB,&err);		//������������
//        }
//#else
//        show_address(0); 						//��ʾ��̬��ַ
//        OS_TaskSuspend((OS_TCB*)&DisplayTaskTCB,&err);		//������������
//#endif //LWIP_DHCP
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
    }
}

//key����
void key_task(void *pdata)   //����ɨ����λ�����Ƿ񴥷���ʵ��ִ�����ж���
{
    u8 key;
    OS_ERR err;
    u8 buf[9]= {'k','e','y','l','o','v','e','\r','\n'};
    while(1)
    {
        key = KEY_Scan(1);
        if(key==KEY0_PRES)
        {
            sprintf((char *)buf,"{\"k\":%d}\r\n",key);
            HAL_UART_Transmit(&UART3_Handler,buf,sizeof(buf),1000);//����2��������
            printf("%s",buf);
        }
        if(key==KEY1_PRES)
        {
            sprintf((char *)buf,"{\"k\":%d}\r\n",key);
            HAL_UART_Transmit(&UART3_Handler,buf,sizeof(buf),1000);//����2��������
            printf("%s",buf);
        }
        if(key==KEY2_PRES)
        {
            sprintf((char *)buf,"{\"k\":%d}\r\n",key);
            HAL_UART_Transmit(&UART3_Handler,buf,sizeof(buf),1000);//����2��������

            printf("%s",buf);
        }


        OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ10ms
    }
}

//led����
void usmart_thread(void *pdata)
{
    OS_ERR err;
    u8 i=0;
//		CPU_SR_ALLOC();
    while(1)
    {
        if(i++==20)
        {
            LED0_Toggle;
            i=0;
        }
        usmart_dev.scan();	//ִ��usmartɨ��
        usmart_scan3();
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
    }
}
void PLUS_show(void *pdata)
{
    OS_ERR err;
    u8 flag=1;
    char buf[20];
		u8 i=0;
    LCD_ShowString(330,140,256,32,32,(u8*)"PLUS_NUM.number=");
    while(1)
    {
        if(TIM2->CNT==0)  {
            if(flag) {
                LED1_Toggle;
                LCD_ShowNum(612,140,PLUS_NUM.number,6,32);
                sprintf(buf,"{plus:\"%d\"}\r\n",PLUS_NUM.number);
                HAL_UART_Transmit(&UART3_Handler,(u8 *)buf,strlen(buf),1000);//����2��������
                flag=0;
								i=0;
            }
        }
        else  {
            LED1_Toggle;
            LCD_ShowNum(612,140,TIM2->CNT,6,32);
            flag=1;
//						i+=100;
//						TIM2->PSC= 199+i;
            sprintf(buf,"{plus:\"%d\"}\r\n",TIM2->CNT);
            HAL_UART_Transmit(&UART3_Handler,(u8 *)buf,strlen(buf),1000);//����2��������
        }

        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms

    }
}



