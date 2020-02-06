#include "timer.h"
#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
TIM_HandleTypeDef TIM2_Handler;      //从定时器句柄 
TIM_HandleTypeDef TIM_Handle;      	 //定时器8句柄 
TIM_OC_InitTypeDef sConfig;
TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;


__IO uint16_t ChannelPulse = 4999;

struct PLUS__NUM PLUS_NUM;

//通用定时器3中断初始化,定时器3在APB1上，APB1的定时器时钟为200MHz
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
//TIM3_Init(5000-1,20000-1);		//10Khz的计数频率，计数5K次为500ms


//定时器2从模式（接收触发信号）
void TIM2_Init()
{  		
		TIM_SlaveConfigTypeDef sSlaveConfig;
//		TIM_ClockConfigTypeDef sClockSourceConfig;
		/* 基本定时器外设时钟使能 */
		__HAL_RCC_TIM2_CLK_ENABLE();	
		/* 从定时器基本功能配置 */
		TIM2_Handler.Instance=TIM2;                          //通用定时器2
    TIM2_Handler.Init.Prescaler=0;                     //分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=0xffff;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);

		/* 定时器时钟源选择 */
		//		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		//		HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig);		
		/* 从模式：外部触发模式1(内部定时器触发)*/
		sSlaveConfig.SlaveMode=TIM_SLAVEMODE_GATED;									//设置为门触发
		sSlaveConfig.InputTrigger=TIM_TS_ITR1;                      //设置(TIM8:ITGO)->(TIM2:ITR1)为输入源
		sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;  //设置触发模式为上升沿
		sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;  //设置无预分频
		sSlaveConfig.TriggerFilter = 0x0;                           //设置无滤波
		HAL_TIM_SlaveConfigSynchronization(&TIM2_Handler, &sSlaveConfig);	

}

//初始化PWM的gpio
void PWM_gpio()
{
		GPIO_InitTypeDef GPIO_InitStruct;
    
    ADVANCE_TIM_CHx_CLK();
//    ADVANCE_TIM_CHxN_CLK();
    ADVANCE_TIM_BKIN_CLK();
  
    GPIO_InitStruct.Pin =  ADVANCE_TIM_CHx_PIN2| ADVANCE_TIM_CHx_PIN3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH ;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(ADVANCE_TIM_CHx_PORT, &GPIO_InitStruct);
		
//    GPIO_InitStruct.Pin = ADVANCE_TIM_CHxN_PIN;
//    HAL_GPIO_Init(ADVANCE_TIM_CHxN_PORT, &GPIO_InitStruct);
  
    GPIO_InitStruct.Pin = ADVANCE_TIM_BKIN_PIN;
    HAL_GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStruct);   
}

//高级定时器8主模式（发出触发信号）PWM输出PC6、（N）PA5刹车PA6
/*
		输出频率最大1MHz,最小20Hz
*/
static void set_TIM8_HZ(u16 HZ)
{
		PLUS_NUM.HZ= 1000000/HZ - 1;
		//TIMx的外设寄存器基地址
    TIM_Handle.Instance = ADVANCE_TIM;
    //计数模式为向上计数，递增
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    /* 累计 TIM_Period个后产生一个更新或者中断*/		
    //当定时器从0计数到9999，即为10000次，为一个定时周期
    TIM_Handle.Init.Period = PLUS_NUM.HZ;
    //时钟不分频，即一个TIM_CLK时钟计1次
    TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    //定时器时钟源TIMxCLK = 2 * PCLK1  
    //				PCLK1 = HCLK / 4 
    //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
    // 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=1000000Hz
    TIM_Handle.Init.Prescaler = 199;   
    // 初始化定时器TIM
    HAL_TIM_Base_Init(&TIM_Handle);

}



void TIM8_Init(u16 HZ)	
{  
		TIM_ClockConfigTypeDef sClockSourceConfig = {0};		
		TIM_MasterConfigTypeDef sMasterConfig = {0};		
    //使能TIMx的时钟
    ADVANCE_TIM_CLK_ENABLE();    			
    set_TIM8_HZ(HZ);		
		/* 定时器时钟源选择 */
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_Handle, &sClockSourceConfig);
  
	
		//主定时器基本配置
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;       //开启主从模式
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;              //更新事件被选为触发输入		
		HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle, &sMasterConfig);
		
    /* 配置TIM为互补输出模式 */
    //互补输出，PWM1模式
    //计数值CNT小于比较值CCR（sConfig.Pulse），通道x为有效电平
    //计数值CNT大于比较值CCR（sConfig.Pulse），通道x为无效电平
    sConfig.OCMode = TIM_OCMODE_PWM1;
    //CHx的有效电平为高电平
    sConfig.OCPolarity = TIM_OCNPOLARITY_LOW;
    //CHx在空闲状态下为低电平
    sConfig.OCIdleState = TIM_OCNIDLESTATE_RESET;
    //CHxN在空闲状态下为高电平(必须与CHx相反)
//    sConfig.OCNIdleState = TIM_OCIDLESTATE_RESET;
    //CHxN的有效电平为高电平
//    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    //比较器CCR的值
    sConfig.Pulse = (1000000/HZ - 1)/2;
    //初始化输出比较通道
//    HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &sConfig, ADVANCE_TIM_CH);
		HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &sConfig, ADVANCE_TIM_CH2);
		HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &sConfig, ADVANCE_TIM_CH3);
    //使能自动输出功能
//    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
    //断路输入极性：低电平有效
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;
    //使能断路功能
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
    //死区时间：11*Fdts
    sBreakDeadTimeConfig.DeadTime = 11;
    //对寄存器提供写保护，级别越高，可操作的寄存器位越少
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_1;
    //空闲模式下的断路状态，允许通道输出
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
    //运行模式下的断路状态，允许通道输出
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
    //配置TIM为死区，断路功能
    HAL_TIMEx_ConfigBreakDeadTime(&TIM_Handle, &sBreakDeadTimeConfig);
    //主动输出CHx使能
//    HAL_TIM_PWM_Start(&TIM_Handle, ADVANCE_TIM_CH);
		HAL_TIM_PWM_Start(&TIM_Handle, ADVANCE_TIM_CH2);
		HAL_TIM_PWM_Start(&TIM_Handle, ADVANCE_TIM_CH3);
    //主动输出CHxN使能
//    HAL_TIMEx_PWMN_Start(&TIM_Handle, ADVANCE_TIM_CH);
		HAL_TIM_Base_Start(&TIM_Handle);
		__HAL_RCC_TIM8_CLK_DISABLE();
}






//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{	
			HAL_NVIC_SetPriority(TIM2_IRQn,1,2);    //设置中断优先级，抢占优先级1，子优先级3
			HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断
}



//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

		if(htim==(&TIM2_Handler))
    { 
			//****************************************************//       
        if(__HAL_TIM_GET_FLAG(&TIM2_Handler, TIM_FLAG_CC1) != RESET)           //判断是否触发中断
        {         
					__HAL_TIM_CLEAR_FLAG(&TIM2_Handler, TIM_FLAG_CC1);                    //清除中断标志位
              
          HAL_TIM_PWM_Stop_IT(&TIM_Handle, TIM_CHANNEL_1);                    //关闭主定时器的PWM输出
             
          HAL_TIM_Base_Stop_IT(&TIM2_Handler);                                  //关闭从定时器的计数
					__HAL_RCC_TIM8_CLK_DISABLE();
				//****************************************************//       
				}
		}
}


void Plus_out_stop()
{
	static u8 stop_flag=0;
	if(TIM2->CNT!=0)
	{
		if(stop_flag)
		{
			stop_flag = 0;
			PLUS_NUM.stop_flag=0;
			__HAL_RCC_TIM8_CLK_ENABLE();
		}
		else
		{
			stop_flag = 1;
			PLUS_NUM.stop_flag=1;
			__HAL_RCC_TIM8_CLK_DISABLE();
		}
	}
	else
		printf("\r\nplus not start!\r\n");
}


//输出特定频率和数量的脉冲,当脉冲数量不为0时说明正在发送脉冲，因此跳过
void Plus_out(u16 HZ,u32 plus_n)
{	
	PLUS_NUM.number=plus_n;
	if(TIM2->CNT==0)
	{		
		__HAL_RCC_TIM8_CLK_DISABLE();
		TIM8_Init(HZ);
		__HAL_TIM_SET_AUTORELOAD(&TIM2_Handler,plus_n);    //ARR装载要输出的PWM脉冲数 
		HAL_TIM_Base_Start_IT(&TIM2_Handler);                   //从定时器计数开启
		HAL_TIM_PWM_Start_IT(&TIM_Handle, TIM_CHANNEL_1);        //主定时器PWM脉冲输出
		__HAL_RCC_TIM8_CLK_ENABLE();
	}
	else
	{
		printf("\r\nPlus_out...\r\n");		
	}
}











