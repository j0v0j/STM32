#include "espmqtt.h"
#include "usart3.h"
#include "usart.h"
//#include "lcd.h"

//���ڽ��ջ����� 	
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
extern vu16 USART3_RX_STA;   	
void mqtt_esp()
{
	
	if(USART3_RX_STA&0x8000)
		{		
				u16 len=USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\nMQTT��ϢΪ:\r\n\r\n");
//			for(u8 t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART3_RX_BUF[t]);//�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
			if(USART3_RX_BUF[0]=='R'&&USART3_RX_BUF[1]=='e'&&USART3_RX_BUF[2]==':')
			{
			
			}
			else //(USART3_RX_BUF[0]=='F'&&USART3_RX_BUF[1]=='d'&&USART3_RX_BUF[2]==':')
			
			{
				printf("\r\n\r\n");//���뻻��
			}
			USART3_RX_STA=0;
		}
}




