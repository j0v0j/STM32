#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//DHT11��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//IO��������
#define DHT11_IO_IN()  {GPIOG->MODER&=~(3<<(10*2));GPIOG->MODER|=0<<(10*2);}	//PB12����ģʽ
#define DHT11_IO_OUT() {GPIOG->MODER&=~(3<<(10*2));GPIOG->MODER|=1<<(10*2);} 	//PB12���ģʽ
 
////IO��������											   
#define	DHT11_DQ_OUT(n) (n?HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_RESET)) //���ݶ˿�	PB12
#define	DHT11_DQ_IN     HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10) //���ݶ˿�	PB12
   	
u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11  
#endif
