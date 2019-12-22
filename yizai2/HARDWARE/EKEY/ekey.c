#include "ekey.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "relay.h"
#include "sys.h"
#include "advance_time.h"
#include "usart3.h"

//////////////////////////////////////////////////////////////////////////////////	 
//�ⲿ�жϴ���������λ����							  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������




void EKEY_GPIO(void)
{ 
    //	�����˿ڳ�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTG,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;//PC0-4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC0,1,2,3,4,5,7

}


u8 EKEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(EKEY_STATE))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(EKEY0==1)return EKEY0_PRES;
		else if(EKEY1==1)return EKEY1_PRES;
		else if(EKEY2==1)return EKEY2_PRES;
		else if(EKEY3==1)return EKEY3_PRES;
		else if(EKEY4==1)return EKEY4_PRES;
		else if(EKEY5==1)return EKEY5_PRES;
		else if(EKEY6==1)return EKEY6_PRES;
	}	else if(EKEY_STATE)key_up=1; 	    
 	return 0;// �ް�������
}


















