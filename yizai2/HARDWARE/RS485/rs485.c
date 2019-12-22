#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//RS485���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


#ifdef EN_USART2_RX   	//���ʹ���˽���


//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   		  
u8 RE_Head_flag=0;
u8 RE_Tail_flag=0;
u8 DH1_flag,DH2_flag;
u8 RS485_flag=1;
//�ֱ��¼����","���±�
u8 DH1_dex,DH2_dex;
u8 mx[3],my[3],dj;
u8 err[]="err";
#define   Head  '['
#define   Tail  ']'

void USART2_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	 
	 			 
		res =USART_ReceiveData(USART2); 	//��ȡ���յ�������
		if(RS485_RX_CNT<64)
		{
			switch(res)
			{
				case Head:
					RE_Head_flag=1;
					RE_Tail_flag=0;
					break;
				case Tail:
					RE_Tail_flag=1;
					RE_Head_flag=0;
					break;
			}
			if(RE_Head_flag)
			{
				RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
				if(res==',')
				{
					if(!DH1_flag)
					{
						DH1_dex=RS485_RX_CNT;
						DH1_flag=1;
					}
					else if(!DH2_flag)
					{
						DH2_dex=RS485_RX_CNT;
						DH2_flag=1;
					}					
				}
				RS485_RX_CNT++;						//�������ݸ���Ӧ��Ϊ  12,56,0
			}
			if(RE_Tail_flag)
			{		
				RS485_RX_BUF[RS485_RX_CNT++]=res;		//��¼���յ���ֵ	
				RS485_RX_BUF[RS485_RX_CNT]='\0';	
//				printf("%s\r\n",RS485_RX_BUF);

				if(DH1_dex==2)          //mxΪһλ��
				{	
					mx[0]=RS485_RX_BUF[1];
					mx[2]=mx[0]-48;       //�õ�mx����������
//					printf("mx=%d\r\n",mx[2]);
				}
				else if(DH1_dex==3) 
				{
					mx[0]=RS485_RX_BUF[1];
					mx[1]=RS485_RX_BUF[2];
					mx[2]=(mx[0]-48)*10+(mx[1]-48);
//					printf("mx=%d\r\n",mx[2]);
				}
				else
				{
				//���ݴ���
					RS485_Send_Data(err,4);
//					printf("mx err\r\n");
				}
				if((DH2_dex-DH1_dex)==2)
				{
					my[0]=RS485_RX_BUF[DH2_dex-1];
					my[2]=my[0]-48;
					dj=RS485_RX_BUF[DH2_dex+1];
//					printf("my=%d\r\n",my[2]);
				}
				else if((DH2_dex-DH1_dex)==3)
				{
					my[0]=RS485_RX_BUF[DH2_dex-2];
					my[1]=RS485_RX_BUF[DH2_dex-1];
					my[2]=(my[0]-48)*10+(my[1]-48);
					dj=RS485_RX_BUF[DH2_dex+1]-48;
//					printf("my=%d\r\n",my[2]);
				}
				else
				{
				//���ݴ���
//					printf("my err\r\n");
					RS485_Send_Data(err,4);
				}
				
//				printf("dj=%d",dj);
				RS485_Send_Data(RS485_RX_BUF,RS485_RX_CNT);
				RS485_flag=0;
//				printf("RS485_flag=%d\r\n",RS485_flag);
				RE_Tail_flag=0;
				RE_Head_flag=0;
				DH1_flag=0;
				DH2_flag=0;
			}
		} 
	}  											 
} 
#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);//ʹ��GPIOA,Dʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PD7�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
 
	
 #ifdef EN_USART2_RX		  	//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); ; //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
   
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

 #endif

  RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
 
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//����Ϊ����ģʽ
  for(t=0;t<len;t++)		//ѭ����������
	{		
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	 
		USART_SendData(USART2,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
u8 RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	vTaskDelay(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
		return 0;
	}
	return 1;
}





















