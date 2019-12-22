#include "espmqtt.h"
#include "usart3.h"
#include "usart.h"
//#include "lcd.h"

//串口接收缓存区 	
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
extern vu16 USART3_RX_STA;   	
void mqtt_esp()
{
	
	if(USART3_RX_STA&0x8000)
		{		
				u16 len=USART3_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\nMQTT消息为:\r\n\r\n");
//			for(u8 t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART3_RX_BUF[t]);//向串口1发送数据
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//			}
			if(USART3_RX_BUF[0]=='R'&&USART3_RX_BUF[1]=='e'&&USART3_RX_BUF[2]==':')
			{
			
			}
			else //(USART3_RX_BUF[0]=='F'&&USART3_RX_BUF[1]=='d'&&USART3_RX_BUF[2]==':')
			
			{
				printf("\r\n\r\n");//插入换行
			}
			USART3_RX_STA=0;
		}
}




