#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ltdc.h"
#include "lcd.h"
#include "sdram.h"
#include "pcf8574.h"
#include "mpu.h"
#include "timer.h"
#include "includes.h"
#include "lwip_comm.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "lwipopts.h"
#include "tcp_client_demo.h"
#include "mqttclient.h"

//#include "rs485.h"
//以下自己修改后添加的头文件

#include "usmart.h"
#include "fdcan.h"


/************************************************
 ALIENTEK 北极星H750/F750开发板 网络实验12
 基于NETCONN API的TCP客户端实验(UCOSIII版本)

 UCOSIII中以下优先级用户程序不能使用，ALIENTEK
 将这些优先级分配给了UCOSIII的5个系统内部任务
 优先级0：中断服务服务管理任务 OS_IntQTask()
 优先级1：时钟节拍任务 OS_TickTask()
 优先级2：定时任务 OS_TmrTask()
 优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
 优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

extern struct PLUS__NUM PLUS_NUM;
extern TIM_HandleTypeDef TIM_Handle;      //定时器句柄
//KEY任务
#define KEY_TASK_PRIO 		8
//任务堆栈大小
#define KEY_STK_SIZE		128
//任务控制块
OS_TCB KeyTaskTCB;
//任务堆栈
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata);

//LED任务
//任务优先级
#define LED_TASK_PRIO		9
//任务堆栈大小
#define LED_STK_SIZE		128
//任务控制块
OS_TCB LedTaskTCB;
//任务堆栈
CPU_STK	LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);

//在LCD上显示地址信息任务
//任务优先级
#define DISPLAY_TASK_PRIO	10
//任务堆栈大小
#define DISPLAY_STK_SIZE	128
//任务控制块
OS_TCB DisplayTaskTCB;
//任务堆栈
CPU_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//任务函数
void display_task(void *pdata);

//START任务
//任务优先级
#define START_TASK_PRIO		11
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);


//MQTT任务
//任务优先级
#define MQTT_TASK_PRIO	12
//任务堆栈大小
#define MQTT_STK_SIZE	128
//任务控制块
OS_TCB MQTTTaskTCB;
//任务堆栈
CPU_STK	MQTT_TASK_STK[MQTT_STK_SIZE];
//任务函数
void mqtt_thread1(void *pdata);


//在LCD上显示地址信息
//mode:1 显示DHCP获取到的地址
//	  其他 显示静态地址
void show_address(u8 mode)
{
    u8 buf[30];
    if(mode==2)
    {
        sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
        LCD_ShowString(30,170,210,16,16,buf);
        sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
        LCD_ShowString(30,190,210,16,16,buf);
        sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
        LCD_ShowString(30,210,210,16,16,buf);
        LCD_ShowString(30,230,210,16,16,"Port:8087!");
    }
    else
    {
        sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
        LCD_ShowString(30,170,210,16,16,buf);
        sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
        LCD_ShowString(30,190,210,16,16,buf);
        sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
        LCD_ShowString(30,210,210,16,16,buf);
        LCD_ShowString(30,230,210,16,16,"Port:8087!");
    }
}

int main(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    Cache_Enable();                			//打开L1-Cache
    Write_Through();
    HAL_Init();				        		//初始化HAL库
    Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz
    delay_init(400);						//延时初始化
    uart_init(115200);						//串口初始化
    uart3_init(115200);						//串口初始化

    LED_Init();								//初始化LED
    KEY_Init();								//初始化按键
    SDRAM_Init();                   		//初始化SDRAM
    MPU_Memory_Protection();		//保护相关存储区域
    LCD_Init();								//初始化LCD
    usmart_init(200); 			//初始化USMART
    FDCAN1_Mode_Init(22,1,5,3,FDCAN_MODE_NORMAL);        //正常模式,波特率1Mbps
    POINT_COLOR=RED;
    LCD_ShowString(30,50,200,16,16,"POLARIS H750/F750");
    LCD_ShowString(30,70,200,16,16,"TCP Client UCOSIII Test");
    LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,"2019/5/13");
    TIM8_Init(1000);
    TIM2_Init();
    OSInit(&err); 					//UCOSIII初始化
    while(lwip_comm_init()) 		//lwip初始化
    {
        LCD_ShowString(30,110,200,20,16,"Lwip Init failed!"); 	//lwip初始化失败
        delay_ms(500);
        LCD_Fill(30,110,230,150,WHITE);
        delay_ms(500);
    }
    LCD_ShowString(30,110,200,20,16,"Lwip Init Success!"); 		//lwip初始化成功
    while(tcp_client_init()) 									//初始化tcpclient_demo(创建tcpclient_demo线程)
    {
        LCD_ShowString(30,150,200,20,16,"TCP Client failed!!"); //tcpclient创建失败
        delay_ms(500);
        LCD_Fill(30,150,230,170,WHITE);
        delay_ms(500);
    }
    LCD_ShowString(30,150,200,20,16,"TCP Client Success!"); 	//tcpclient创建成功

    OS_CRITICAL_ENTER();//进入临界区x
    //创建开始任务
    OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
                 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
    OS_CRITICAL_EXIT();	//退出临界区
    OSStart(&err); 		//开启UCOS
}

//start任务
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
    //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

#if LWIP_DHCP
    lwip_comm_dhcp_creat(); //创建DHCP任务
#endif
//	mqtt_thread_init();
    OS_CRITICAL_ENTER();	//进入临界区
    //创建LED任务
    OSTaskCreate((OS_TCB 	* )&LedTaskTCB,
                 (CPU_CHAR	* )"led0 task",
                 (OS_TASK_PTR )led_task,
                 (void		* )0,
                 (OS_PRIO	  )LED_TASK_PRIO,
                 (CPU_STK   * )&LED_TASK_STK[0],
                 (CPU_STK_SIZE)LED_STK_SIZE/10,
                 (CPU_STK_SIZE)LED_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
    //创建KEY任务
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
    //创建显示任务
    OSTaskCreate((OS_TCB 	* )&DisplayTaskTCB,
                 (CPU_CHAR	* )"display task",
                 (OS_TASK_PTR )display_task,
                 (void		* )0,
                 (OS_PRIO	  )DISPLAY_TASK_PRIO,
                 (CPU_STK   * )&DISPLAY_TASK_STK[0],
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE/10,
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

    //创建MQTT任务
    OSTaskCreate(  (OS_TCB 	  * )&MQTTTaskTCB,
                   (CPU_CHAR	* )"mqtt task",
                   (OS_TASK_PTR )mqtt_thread1,
                   (void		  * )0,
                   (OS_PRIO	    )MQTT_TASK_PRIO,
                   (CPU_STK   * )&MQTT_TASK_STK[0],
                   (CPU_STK_SIZE)MQTT_STK_SIZE/10,
                   (CPU_STK_SIZE)MQTT_STK_SIZE,
                   (OS_MSG_QTY  )0,
                   (OS_TICK	    )0,
                   (void   	  * )0,
                   (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                   (OS_ERR 	  * )&err);

    OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务
    OS_CRITICAL_EXIT();	//退出临界区
}

//显示地址等信息
void display_task(void *pdata)
{
    OS_ERR err;

    while(1)
    {
#if LWIP_DHCP									//当开启DHCP的时候
        if(lwipdev.dhcpstatus != 0) 			//开启DHCP
        {
            show_address(lwipdev.dhcpstatus );	//显示地址信息
            OS_TaskSuspend((OS_TCB*)&DisplayTaskTCB,&err);		//挂起自身任务
        }
#else
        show_address(0); 						//显示静态地址
        OS_TaskSuspend((OS_TCB*)&DisplayTaskTCB,&err);		//挂起自身任务
#endif //LWIP_DHCP
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
    }
}

//key任务
void key_task(void *pdata)
{
    u8 key;
    OS_ERR err;
		u8 buf[9]= {'w','c','h','l','o','v','e','\r','\n'};
    while(1)
    {
        key = KEY_Scan(0);
       switch(key)
			 {
				 case KEY1_PRES:
					  
            HAL_UART_Transmit(&UART3_Handler,buf,9,1000);//串口2发送数据			 
			 
			 }
        OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
    }
}

//led任务
void led_task(void *pdata)
{
    OS_ERR err;
    u8 i=0;
		CPU_SR_ALLOC();
    while(1)
    {
        if(i++==10)
        {
            LED0_Toggle;
            i=0;
        }
				OS_CRITICAL_ENTER();//进入临界区
        usmart_dev.scan();	//执行usmart扫描
        usmart_scan3();
        OS_CRITICAL_EXIT();	//退出临界区

        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
    }
}
void mqtt_thread1(void *pdata)
{
    OS_ERR err;
		u8 flag=1;
		char buf[20];
    LCD_ShowString(330,140,256,32,32,"PLUS_NUM.number=");
    while(1)
    {
        if(TIM2->CNT==0)  {
            if(flag) {
                LCD_ShowNum(612,140,PLUS_NUM.number,6,32);
								sprintf(buf,"{plus:\"%d\"}\r\n",PLUS_NUM.number);
								HAL_UART_Transmit(&UART3_Handler,(u8 *)buf,strlen(buf),1000);//串口2发送数据
                flag=0;							
            }
        }
        else  {
            LCD_ShowNum(612,140,TIM2->CNT,6,32);
            flag=1;
            sprintf(buf,"{plus:\"%d\"}\r\n",TIM2->CNT);
            HAL_UART_Transmit(&UART3_Handler,(u8 *)buf,strlen(buf),1000);//串口2发送数据
        }

        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms

    }
}



