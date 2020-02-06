#include "set_io.h"

 /**************
 使用到的GPIO
 
	PC6
	PC7
	PC8
	PC9
	PC10
	PC11
	PC12
	PD2
	PD3
	PD7
	PD13

 
 **************/
void REY_io_init()
{
  GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();					//开启GPIOB时钟
//		__HAL_RCC_GPIOB_CLK_ENABLE();					//开启GPIOB时钟
//	  __HAL_RCC_GPIOA_CLK_ENABLE();					//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_13;			//PC9-PC12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;         			//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     		//初始化GPIOB.0和GPIOB.1
	
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_13,GPIO_PIN_RESET);	//PB0置0
		DIR_io_init();
}
void REY_n(u8 i,u8 n)
{  
	switch(i)
	{
		case 1:
	   REY_1(n);
			break;
		case 2:
	   REY_2(n);
			break;
		case 3:
	   REY_3(n);
			break;
		case 4:
	   REY_4(n);
			break;
		case 5:
	   REY_5(n);
			break;
		case 6:
	   REY_6(n);
			break;
		case 7:
	   REY_7(n);
			break;
		case 8:
	   REY_8(n);
			break;
		case 9:
			DIR_L(n);
			break;
		case 10:
			DIR_R(n);
			break;
	}
	printf("\r\n(%d,%d)\r\n",i,n);
   
}




void DIR_io_init()
{
  GPIO_InitTypeDef GPIO_Initure;
//    __HAL_RCC_GPIOD_CLK_ENABLE();					//开启GPIOB时钟
//		__HAL_RCC_GPIOB_CLK_ENABLE();					//开启GPIOB时钟
	  __HAL_RCC_GPIOA_CLK_ENABLE();					//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_8;			//PC9-PC12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;         			//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);     		//初始化GPIOB.0和GPIOB.1
	
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5|GPIO_PIN_8,GPIO_PIN_RESET);	//PB0置0
			
}


