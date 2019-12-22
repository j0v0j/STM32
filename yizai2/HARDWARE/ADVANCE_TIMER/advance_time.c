/***
带死区和刹车控制的脉冲使用TIME1
当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
****************************************************************************************
     |    TIM1     |    TIM8  |       TIM2 |   TIM5   |    TIM3     |   TIM4   |
****************************************************************************************
CH1  | PA8/PE9     |  PC6     |  PA0/PA15  |   PA0    | PA6/PC6/PB4 | PB6/PD12 |
****************************************************************************************
CH1N | PB13/PA7/PE8|  PA7     |            |          |             |          |
****************************************************************************************
CH2  | PA9PE11     |  PC7     |  PA1/PB3   |   PA1    | PA7/PC7/PB5 | PB7/PD13 |
****************************************************************************************
CH2N |PB14/PB0/PE10|  PB0     |            |          |             |          |
****************************************************************************************
CH3  | PA10/PE13   |  PC8     |  PA2/PB10  |   PA2    |   PB0/PC8   | PB8/PD14 |
****************************************************************************************
CH3N |PB15/PB1/PE12|  PB1     |            |          |             |          |
****************************************************************************************
CH4  | PA11/PE14   |  PC9     |  PA3       |   PA3    |  PB1/PC9    | PB9/PD15 |
****************************************************************************************
ETR  | PA12/PE7    |  PA0     |            |          |  PD2        |  PE0     |
****************************************************************************************
BKIN |PB12/PA6/PE15|  PA6     |            |          |             |          |
****************************************************************************************


****/


#include "advance_time.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "FreeRTOS.h"
#include "task.h"
/*----------------------定时器发出指定频率和脉冲的个数----------------------
高级定时器1工作在主模式下，以互补形式发出脉冲信号，程序中两路脉冲信号完全一致。
void TIM8_config(u32 Cycle)函数的变量Cycle用于指定脉冲的频率
void TIM2_config(u32 PulseNum)函数的变量PulseNum用于指定脉冲的个数
*/

u8 TIM2_IRQ=1;
/***定时器1主模式***/
void TIM8_config(u32 Cycle)
{
/*--------------------GPIO结构体初始化-------------------------*/
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
		
		// 输出比较通道 GPIO 初始化，引脚参考advance_time.h下的宏定义，默认PA7互补PC6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                   //TIM8_CH4 PA7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		// 输出比较通道互补通道 GPIO 初始化
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                   //TIM8_CH4 PC6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		// 输出比较通道刹车通道 GPIO 初始化
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		// 刹车引脚默认先输出低电平
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);			
		
/*--------------------时基结构体初始化-------------------------*/	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE); 	
    TIM_TimeBaseStructure.TIM_Period = Cycle-1;                                                   
    TIM_TimeBaseStructure.TIM_Prescaler =71;                    //设置用来作为TIMx时钟频率除数的预分频值                                                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割：TDTS= Tck_tim            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //重复计数，一定要=0！！！
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);                                       
		
/*--------------------输出比较结构体初始化-------------------*/		
		TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          	//选择定时器模式：TIM脉冲宽度调制模式1       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // 互补输出使能
    TIM_OCInitStructure.TIM_Pulse = Cycle/2-1;                    //设置待装入捕获寄存器的脉冲值                                   
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //输出极性 
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;		// 互补输出通道电平极性配置	
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  // 输出通道空闲电平极性配置
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	// 互补输出通道空闲电平极性配置
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);		

/*-------------------刹车和死区结构体初始化-------------------*/
		// 有关刹车和死区结构体的成员具体可参考BDTR寄存器的描述
		TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
		TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
		TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
		TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
		// 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0]的描述
		// 这里配置的死区时间为152ns
		TIM_BDTRInitStructure.TIM_DeadTime = 11;
		TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
		// 当BKIN引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
		TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
		TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
		TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

    TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);

    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);                              
    TIM_ARRPreloadConfig(TIM8, ENABLE);                                                          
}
/***定时器2从模式***/
void TIM2_config(u32 PulseNum)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
/*--------------------时基结构体初始化-------------------------*/	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = PulseNum-1;   
    TIM_TimeBaseStructure.TIM_Prescaler =0;    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  

    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR1);
  
    TIM2->SMCR|=0x07;                                  //设置从模式寄存器 
      
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
/*-------------------配置中断向量-------------------*/
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;        
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}
void Pulse_output(u32 Cycle,u32 PulseNum)
{
//	printf("F=%d,nu=%d\r\n",Cycle,PulseNum);
	if(TIM2_IRQ)
	{
		PLUS=RESET;
		TIM2_IRQ=0;
    TIM2_config(PulseNum);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		TIM8_config(Cycle);
		TIM_Cmd(TIM8, ENABLE);
    TIM_Cmd(TIM2, ENABLE);   
		TIM_CtrlPWMOutputs(TIM8, ENABLE);   //高级定时器一定要加上，主输出使能
	}
	else 
	{
		printf("f=%d,num=%d 脉冲发送中,禁止打断...\r\n",Cycle,PulseNum);
	}
}

void TIM_dis(void)
{	
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位 
    TIM_CtrlPWMOutputs(TIM8, DISABLE);  //主输出使能
    TIM_Cmd(TIM8, DISABLE); // 关闭定时器 
    TIM_Cmd(TIM2, DISABLE); // 关闭定时器 
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
    TIM2_IRQ=1;
		PLUS=SET;
}




void TIM2_IRQHandler(void) 
{ 
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)     // TIM_IT_CC1
    { 
        TIM_dis();
    } 
} 



