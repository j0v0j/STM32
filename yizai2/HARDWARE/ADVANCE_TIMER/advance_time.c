/***
��������ɲ�����Ƶ�����ʹ��TIME1
��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
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
/*----------------------��ʱ������ָ��Ƶ�ʺ�����ĸ���----------------------
�߼���ʱ��1��������ģʽ�£��Ի�����ʽ���������źţ���������·�����ź���ȫһ�¡�
void TIM8_config(u32 Cycle)�����ı���Cycle����ָ�������Ƶ��
void TIM2_config(u32 PulseNum)�����ı���PulseNum����ָ������ĸ���
*/

u8 TIM2_IRQ=1;
/***��ʱ��1��ģʽ***/
void TIM8_config(u32 Cycle)
{
/*--------------------GPIO�ṹ���ʼ��-------------------------*/
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
		
		// ����Ƚ�ͨ�� GPIO ��ʼ�������Ųο�advance_time.h�µĺ궨�壬Ĭ��PA7����PC6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                   //TIM8_CH4 PA7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		// ����Ƚ�ͨ������ͨ�� GPIO ��ʼ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                   //TIM8_CH4 PC6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		// ����Ƚ�ͨ��ɲ��ͨ�� GPIO ��ʼ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		// ɲ������Ĭ��������͵�ƽ
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);			
		
/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE); 	
    TIM_TimeBaseStructure.TIM_Period = Cycle-1;                                                   
    TIM_TimeBaseStructure.TIM_Prescaler =71;                    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ                                                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָTDTS= Tck_tim            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //�ظ�������һ��Ҫ=0������
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);                                       
		
/*--------------------����ȽϽṹ���ʼ��-------------------*/		
		TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          	//ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // �������ʹ��
    TIM_OCInitStructure.TIM_Pulse = Cycle/2-1;                    //���ô�װ�벶��Ĵ���������ֵ                                   
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //������� 
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;		// �������ͨ����ƽ��������	
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  // ���ͨ�����е�ƽ��������
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	// �������ͨ�����е�ƽ��������
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);		

/*-------------------ɲ���������ṹ���ʼ��-------------------*/
		// �й�ɲ���������ṹ��ĳ�Ա����ɲο�BDTR�Ĵ���������
		TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
		TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
		TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
		TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
		// ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������
		// �������õ�����ʱ��Ϊ152ns
		TIM_BDTRInitStructure.TIM_DeadTime = 11;
		TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
		// ��BKIN���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��
		TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
		TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
		TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

    TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);

    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);                              
    TIM_ARRPreloadConfig(TIM8, ENABLE);                                                          
}
/***��ʱ��2��ģʽ***/
void TIM2_config(u32 PulseNum)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = PulseNum-1;   
    TIM_TimeBaseStructure.TIM_Prescaler =0;    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  

    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR1);
  
    TIM2->SMCR|=0x07;                                  //���ô�ģʽ�Ĵ��� 
      
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
/*-------------------�����ж�����-------------------*/
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
		TIM_CtrlPWMOutputs(TIM8, ENABLE);   //�߼���ʱ��һ��Ҫ���ϣ������ʹ��
	}
	else 
	{
		printf("f=%d,num=%d ���巢����,��ֹ���...\r\n",Cycle,PulseNum);
	}
}

void TIM_dis(void)
{	
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // ����жϱ�־λ 
    TIM_CtrlPWMOutputs(TIM8, DISABLE);  //�����ʹ��
    TIM_Cmd(TIM8, DISABLE); // �رն�ʱ�� 
    TIM_Cmd(TIM2, DISABLE); // �رն�ʱ�� 
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



