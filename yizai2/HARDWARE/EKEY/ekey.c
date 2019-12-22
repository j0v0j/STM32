#include "ekey.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "relay.h"
#include "sys.h"
#include "advance_time.h"
#include "usart3.h"

//////////////////////////////////////////////////////////////////////////////////	 
//外部中断触发用于限位开关							  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序




void EKEY_GPIO(void)
{ 
    //	按键端口初始化
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTG,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;//PC0-4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成下拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC0,1,2,3,4,5,7

}


u8 EKEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(EKEY_STATE))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(EKEY0==1)return EKEY0_PRES;
		else if(EKEY1==1)return EKEY1_PRES;
		else if(EKEY2==1)return EKEY2_PRES;
		else if(EKEY3==1)return EKEY3_PRES;
		else if(EKEY4==1)return EKEY4_PRES;
		else if(EKEY5==1)return EKEY5_PRES;
		else if(EKEY6==1)return EKEY6_PRES;
	}	else if(EKEY_STATE)key_up=1; 	    
 	return 0;// 无按键按下
}


















