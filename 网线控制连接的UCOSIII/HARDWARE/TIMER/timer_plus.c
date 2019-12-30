#include "timer.h"
#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
//TIM_HandleTypeDef TIM2_Handler;      //��ʱ����� 
TIM_HandleTypeDef TIM7_Handler;      //��ʱ����� 
TIM_HandleTypeDef TIM_Handle;      //��ʱ����� 
TIM_OC_InitTypeDef sConfig;
TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;


__IO uint16_t ChannelPulse = 4999;

struct PLUS__NUM PLUS_NUM;

//ͨ�ö�ʱ��3�жϳ�ʼ��,��ʱ��3��APB1�ϣ�APB1�Ķ�ʱ��ʱ��Ϊ200MHz
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
//TIM3_Init(5000-1,20000-1);		//10Khz�ļ���Ƶ�ʣ�����5K��Ϊ500ms
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
//		TIM3->SMCR|=0x07;//0111
		
		HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE    
}

//��ʱ��2��ģʽ�����մ����źţ�
void TIM2_Init()
{  
	
		
		TIM_SlaveConfigTypeDef sSlaveConfig;
//		TIM_ClockConfigTypeDef sClockSourceConfig;
		/* ������ʱ������ʱ��ʹ�� */
		__HAL_RCC_TIM2_CLK_ENABLE();	
		/* �Ӷ�ʱ�������������� */
		TIM2_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��2
    TIM2_Handler.Init.Prescaler=0;                     //��Ƶ
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM2_Handler.Init.Period=0xffff;                        //�Զ�װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM2_Handler);

		/* ��ʱ��ʱ��Դѡ�� */
//		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//		HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig);
//		
		/* ��ģʽ���ⲿ����ģʽ1(�ڲ���ʱ������)*/
		sSlaveConfig.SlaveMode=TIM_SLAVEMODE_GATED;									//����Ϊ�Ŵ���
		sSlaveConfig.InputTrigger=TIM_TS_ITR1;                      //����(TIM8:ITGO)->(TIM2:ITR1)Ϊ����Դ
		sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;  //���ô���ģʽΪ������
		sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;  //������Ԥ��Ƶ
		sSlaveConfig.TriggerFilter = 0x0;                           //�������˲�
		HAL_TIM_SlaveConfigSynchronization(&TIM2_Handler, &sSlaveConfig);
		
		

//		HAL_TIM_Base_Stop_IT(&TIM2_Handler); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE    
		
	
		

}

//��ʼ��PWM��gpio
void PWM_gpio()
{
		GPIO_InitTypeDef GPIO_InitStruct;
    
    ADVANCE_TIM_CHx_CLK();
    ADVANCE_TIM_CHxN_CLK();
    ADVANCE_TIM_BKIN_CLK();
  
    GPIO_InitStruct.Pin = ADVANCE_TIM_CHx_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH ;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(ADVANCE_TIM_CHx_PORT, &GPIO_InitStruct);
  
    GPIO_InitStruct.Pin = ADVANCE_TIM_CHxN_PIN;
    HAL_GPIO_Init(ADVANCE_TIM_CHxN_PORT, &GPIO_InitStruct);
  
    GPIO_InitStruct.Pin = ADVANCE_TIM_BKIN_PIN;
    HAL_GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStruct);   
}

//�߼���ʱ��8��ģʽ�����������źţ�PWM���PC6����N��PA5ɲ��PA6
/*
		���Ƶ�����1MHz,��С20Hz
*/
void TIM8_Init(u16 HZ)	
{  
		TIM_ClockConfigTypeDef sClockSourceConfig = {0};		
		TIM_MasterConfigTypeDef sMasterConfig = {0};
	
		PWM_gpio();
    //ʹ��TIMx��ʱ��
    ADVANCE_TIM_CLK_ENABLE(); 
   			
    //TIMx������Ĵ�������ַ
    TIM_Handle.Instance = ADVANCE_TIM;
    //����ģʽΪ���ϼ���������
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    /* �ۼ� TIM_Period�������һ�����»����ж�*/		
    //����ʱ����0������9999����Ϊ10000�Σ�Ϊһ����ʱ����
    TIM_Handle.Init.Period = 1000000/HZ - 1;
    //ʱ�Ӳ���Ƶ����һ��TIM_CLKʱ�Ӽ�1��
    TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    //��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
    //				PCLK1 = HCLK / 4 
    //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
    // �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=1000000Hz
    TIM_Handle.Init.Prescaler = 199;   
    // ��ʼ����ʱ��TIM
    HAL_TIM_Base_Init(&TIM_Handle);
		
		/* ��ʱ��ʱ��Դѡ�� */
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_Handle, &sClockSourceConfig);
  
	
		//����ʱ����������
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;       //��������ģʽ
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;              //�����¼���ѡΪ��������		
		HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle, &sMasterConfig);
		
    /* ����TIMΪ�������ģʽ */
    //���������PWM1ģʽ
    //����ֵCNTС�ڱȽ�ֵCCR��sConfig.Pulse����ͨ��xΪ��Ч��ƽ
    //����ֵCNT���ڱȽ�ֵCCR��sConfig.Pulse����ͨ��xΪ��Ч��ƽ
    sConfig.OCMode = TIM_OCMODE_PWM1;
    //CHx����Ч��ƽΪ�ߵ�ƽ
    sConfig.OCPolarity = TIM_OCNPOLARITY_LOW;
    //CHx�ڿ���״̬��Ϊ�͵�ƽ
    sConfig.OCIdleState = TIM_OCNIDLESTATE_SET;
    //CHxN�ڿ���״̬��Ϊ�ߵ�ƽ(������CHx�෴)
    sConfig.OCNIdleState = TIM_OCIDLESTATE_RESET;
    //CHxN����Ч��ƽΪ�ߵ�ƽ
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    //�Ƚ���CCR��ֵ
    sConfig.Pulse = (1000000/HZ - 1)/2;
    //��ʼ������Ƚ�ͨ��
    HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &sConfig, ADVANCE_TIM_CH);
 
    //ʹ���Զ��������
//    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
    //��·���뼫�ԣ��͵�ƽ��Ч
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;
    //ʹ�ܶ�·����
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
    //����ʱ�䣺11*Fdts
    sBreakDeadTimeConfig.DeadTime = 11;
    //�ԼĴ����ṩд����������Խ�ߣ��ɲ����ļĴ���λԽ��
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_1;
    //����ģʽ�µĶ�·״̬������ͨ�����
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
    //����ģʽ�µĶ�·״̬������ͨ�����
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
    //����TIMΪ��������·����
    HAL_TIMEx_ConfigBreakDeadTime(&TIM_Handle, &sBreakDeadTimeConfig);
    //�������CHxʹ��
    HAL_TIM_PWM_Start(&TIM_Handle, ADVANCE_TIM_CH);
    //�������CHxNʹ��
//    HAL_TIMEx_PWMN_Start(&TIM_Handle, ADVANCE_TIM_CH);
//		TIM8->SMCR=0X07;
		HAL_TIM_Base_Start(&TIM_Handle);
		__HAL_RCC_TIM8_CLK_DISABLE();
}


void TIM7_Init(u16 arr,u16 psc)
{  
    TIM7_Handler.Instance=TIM7;                          //ͨ�ö�ʱ��7
    TIM7_Handler.Init.Prescaler=199;                     //��Ƶ
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM7_Handler.Init.Period=(1000000/arr) - 1;                        //�Զ�װ��ֵ
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM7_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //ʹ�ܶ�ʱ��7�Ͷ�ʱ��7�����жϣ�TIM_IT_UPDATE    
}


void Timer3_disable(void)					   //TIM3ʧ��
{
	__HAL_RCC_TIM3_CLK_DISABLE();
	
	HAL_NVIC_DisableIRQ(TIM3_IRQn);
}
void Timer3_enable(void)					   //TIM3ʧ��
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void Timer7_disable(void)					   //TIM3ʧ��
{
	__HAL_RCC_TIM7_CLK_DISABLE();	
	HAL_NVIC_DisableIRQ(TIM7_IRQn);
}

void Timer7_enable(void)					   //TIM3ʧ��
{
	__HAL_RCC_TIM7_CLK_ENABLE();	
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//��ʱ��7�жϷ�����
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handler);
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    
			__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
			HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
			HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	 
		
			__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��
			HAL_NVIC_SetPriority(TIM7_IRQn,1,0);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
			HAL_NVIC_EnableIRQ(TIM7_IRQn);          //����ITM7�ж�  	
	
		
			HAL_NVIC_SetPriority(TIM2_IRQn,1,2);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
			HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж�
}



//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//		static u16 p_num=0;
    if(htim==(&TIM3_Handler))
    {
//			printf("TIME3\r\n");
			
    }
		if(htim==(&TIM7_Handler))
    { 						
			PLUS_NUM.number =TIM2->CNT;  
			LED1_Toggle;
    }
		if(htim==(&TIM2_Handler))
    { 
//			printf("PLUS_NUM=%d\r\n",PLUS_NUM.number);
			//****************************************************//       
        if(__HAL_TIM_GET_FLAG(&TIM2_Handler, TIM_FLAG_CC1) != RESET)           //�ж��Ƿ񴥷��ж�
        {         
					__HAL_TIM_CLEAR_FLAG(&TIM2_Handler, TIM_FLAG_CC1);                    //����жϱ�־λ
              
          HAL_TIM_PWM_Stop_IT(&TIM_Handle, TIM_CHANNEL_1);                    //�ر�����ʱ����PWM���
             
          HAL_TIM_Base_Stop_IT(&TIM2_Handler);                                  //�رմӶ�ʱ���ļ���
					__HAL_RCC_TIM8_CLK_DISABLE();
					Timer7_disable();
//						printf("TIM2 interrupt\r\n");
				//****************************************************//       
				}
		}
}
void Set_plus_f(u32 HZ)
{
	
	
	TIM8_Init(HZ);
	TIM7_Init(HZ,2);
//	__HAL_RCC_TIM8_CLK_DISABLE();
//	TIM8->ARR =1000000/HZ - 1;
//	TIM8->CCR1=(1000000/HZ - 1)/2;
//	__HAL_RCC_TIM8_CLK_ENABLE();
}
void Turn_on(u8 i)
{
	switch(i)
	{
		case 0:
			__HAL_RCC_TIM8_CLK_DISABLE();
		return;
		case 1:
			__HAL_RCC_TIM8_CLK_ENABLE();
		return;
		default:
			return;
	}

}
void Plus_out(u32 plus_n)
{
		__HAL_RCC_TIM8_CLK_ENABLE();
		Timer7_enable();
		__HAL_TIM_SET_AUTORELOAD(&TIM2_Handler,plus_n);    //ARRװ��Ҫ�����PWM������ 
		HAL_TIM_Base_Start_IT(&TIM2_Handler);                   //�Ӷ�ʱ����������
		HAL_TIM_PWM_Start_IT(&TIM_Handle, TIM_CHANNEL_1);        //����ʱ��PWM�������
		__HAL_RCC_TIM7_CLK_ENABLE();
}












