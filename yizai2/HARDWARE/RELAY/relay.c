#include "relay.h"
#include "sys.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
//继电器PC8-PC12   PG13-PG14
void 	Relay_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PF时钟
	
 GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8
																|GPIO_Pin_9
																|GPIO_Pin_10
																|GPIO_Pin_11
																|GPIO_Pin_12;//PF0-PF7端口配置
	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOF0-7
 GPIO_SetBits(GPIOC, GPIO_Pin_8
										|GPIO_Pin_9
										|GPIO_Pin_10
										|GPIO_Pin_11
										|GPIO_Pin_12);						 //GPIOC8-12 输出Low	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//使能PF时钟
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12
														 |GPIO_Pin_14
														 |GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_12
										|GPIO_Pin_14
										|GPIO_Pin_15); 
}

void Relay_test(u8 i,u8 sta)
{
	sta=!sta;
	switch(i)
	{
		case 123:
			RIN5=sta;
			RIN6=sta;
			RIN7=sta;
			break;
		case 1:
			RIN1=sta;
			break;
		case 2:
			RIN2=sta;
			break;
		case 3:
			RIN3=sta;
			break;
		case 4:
			RIN4=sta;
			break;
		case 5:
			RIN5=sta;
			break;
		case 6:
			RIN6=sta;
			break;
		case 7:
			RIN7=sta;
			break;
		case 8:
			RIN8=sta;
			break;
		case 10:
			RINall(0);
			break;
		case 11:
			RINall(1);
			break;
			

	}

}

//void Break_re(void)
//{
//	RIN1=!RIN1;
//	RIN2=!RIN2;

//}




