#include "yizai2.h"
#include "Big_motor.h"
#include "can_motor.h"
#include "advance_time.h"
#include "usart.h"
#include "can.h"
#include "key.h"
#include "string.h"
#include "led.h"
#include "ekey.h"
#include "beep.h"   //��������
#include "malloc.h"
#include "relay.h"
//����������
TaskHandle_t YZTask2_Handler;
/*******ȫ�ֲ�������*******/
extern long Real_Position_Value[4];  //���ŷ���ɱ�־
extern u8 TIM2_IRQ;
//��λΪ�������
extern s32 X; 			  	 //��ʱ��ʵ�������������
extern u32 Y;						 //��ʱ��ʵ�������������
extern s32 Z;


u16 xiao_v=1500;

u8  yizai2_hang=8,yizai2_lie=12;//���Ե�����������
u8  yudingwei_flag;//����ִ��Ԥ��λ��������������Զ�����
u8  L=30;//����Ѩ֮��ļ��
u16 HZ=0;//ÿ����Ҫ�ڴ�ֱ�������еľ��룬Ҳ�����Ի�������̵���������߶�
u16 F_Z_f=1000;//���ŷ�������Ƶ��
u8 yizai2_runing=1;//�����Զ������־λ
u8 dj_x=0,dj_y;  //�ȼ�ʶ���X��Yλ��
u8 yizai2_flag=0;	 //���񴴽���־λ
u16 D1X=500,D2X=500;//D0X��ʶ���Xλ�ã�D0Y��ʶ���Yλ��,D1Xһ�����̾���ԭ���λ�ã�D2X�������̾���ԭ���λ��
#define  D0X (yizai2_hang*L+50)
#define  D0Y ((yizai2_lie-1)*L/2)
#define  M_DJ  miao_dj()
/*
1.���Ȼص���ʼ�õ�λ��
2.�����������
3.һ�����Խ����󣬻ص���ʼλ��׼��������һ����
4.dasifu_daowei(void)������ȷ�����ŷ����������ʱ���ᱻ������������
1000000/F_Z_f�ǻ�����Ƶ��ֵ��move_x((yizai2_hang+1)*L,1000000/F_Z_f);����������ʱ�ı���һ�ε�����Ƶ�ʡ�
*/
void chushi_weizhi()//�ص���ʼ��λ��
{
//	move_z(HZ,1000000/F_Z_f);
//	dasifu_daowei();
//	move_x((yizai2_hang+1)*L*44,1000000/F_Z_f);
//	move_y(xiao_v,0);
//	dasifu_daowei();//X��Y��������
}


void LowerToCap(u8 *str,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		if((96<str[i])&&(str[i]<123))	//Сд��ĸ
		str[i]=str[i]-32;				//ת��Ϊ��д
	}
}
//�ȴ�ʶ����ȼ�
u8 miao_dj()
{
	u16 len;
	u8 *CommandStr;
	while(1)
	{
		vTaskDelay(500);
		if(USART_RX_STA&0x8000)
		{				
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				CommandStr=mymalloc(SRAMIN,len+1);				//�����ڴ�
				sprintf((char*)CommandStr,"%s",USART_RX_BUF);
				CommandStr[len]='\0';							//�����ַ�����β����
				LowerToCap(CommandStr,len);				//���ַ���ת��Ϊ��д
				USART_RX_STA=0;
				if(strcmp((char*)CommandStr,"DJ1")==0)
				{		
						memset(USART_RX_BUF,0,USART_REC_LEN);			//���ڽ��ջ���������
						myfree(SRAMIN,CommandStr);						//�ͷ��ڴ�
						return DJ1miao;
				}
				else if(strcmp((char*)CommandStr,"DJ2")==0) 
				{
						memset(USART_RX_BUF,0,USART_REC_LEN);			//���ڽ��ջ���������
						myfree(SRAMIN,CommandStr);						//�ͷ��ڴ�
						return DJ2miao;
				}
		}
		else
		{
				printf("δ���յ��ȼ�����\r\n");
		}		
	}
}

void yizai2(void *pvParameters)//�����Զ�����
{
	chushi_weizhi();	
	u32 D1x=0,D1y=0,D1hang=0,D1lie=0;
	u32 D2x=0,D2y=0,D2hang=0,D2lie=0;
	u16 Dx,Dy;
	for(u8 hang=yizai2_hang;hang>=0;hang--)
	{
		for(u16 lie=0;lie<=yizai2_lie;lie++)
		{
			Dx=hang*L*44;             //����x����
			Dy=lie*L*404;             //����y����
			move_x(Dx,1000000/F_Z_f);     //�ƶ�������λ��Dx��
			move_y(xiao_v,Dy);                //�ƶ�DAO����λ��Dy��
			dasifu_daowei();           //�ȴ����ŷ���λFreertos���е��������Ҳ��������������ڲ�
			move_z(0,1000000/F_Z_f);      //�����˶�HZ�ľ���
			dasifu_daowei();								//�ȴ����ŷ���λ		
			move_x((hang-1)*L*44,1000000/F_Z_f);  //��ǰ�ƶ�һ��Ѩ�ľ���,
			dasifu_daowei();               //���ŷ���λ
			can_set_relay_out(0,1,1);  //һ�������֣�ȡ��
			printf("ץ��\r\n");
			vTaskDelay(300);
			move_z(HZ,1000000/F_Z_f);     //����������
			dasifu_daowei();           //�ȴ����ŷ���λFreertos���е��������Ҳ��������������ڲ�
			move_x(D0X*44,1000000/F_Z_f); //���е�ʶ��λ�õ�Xλ��;
			printf("ʶ����\r\n");
			move_y(xiao_v,D0Y*404);           //���е�ʶ��λ�õ�Yλ��
			dasifu_daowei();  
			switch(M_DJ)          //�ȴ�ʶ����ȼ�
			{
				case DJ1miao://һ����
					D1x=D1hang*L+D1X;      //����һ�����̵�Xλ��
					D1y=D1lie*L;					 //����һ�����̵�Yλ��
//					printf("��һ������D1X=%d\r\n",D1x);
					move_x(D1x*44,1000000/F_Z_f);
					move_y(xiao_v,D1y*404);
					dasifu_daowei();
					move_z(0,1000000/F_Z_f);
					dasifu_daowei();
					can_set_relay_out(0,1,0);  //һ�������֣�����
					printf("����\r\n");
					vTaskDelay(300);
					move_z(HZ,1000000/F_Z_f);//����������HZ
					dasifu_daowei();
					D1lie++;
					if(D1lie==yizai2_lie)   //һ�����̵����Ѿ�װ��
					{
						D1lie=0;
						D1hang++;
						if(D1hang==yizai2_hang)//һ�������Ѿ�װ��
						{
							D1hang=0;
							/*
								һ�����̵����ʹ��˶�������һ�����������͵�ָ��λ��
							*/
						}
					}
					break;
				case DJ2miao://������
					D2x=D2hang*L+D2X;      //����һ�����̵�Xλ��
					D2y=D2lie*L;					 //����һ�����̵�Yλ��
					move_x(D2x*44,1000000/F_Z_f);
					move_y(xiao_v,D2y*404);
					dasifu_daowei();
					move_z(0,1000000/F_Z_f);
					dasifu_daowei();
					can_set_relay_out(0,1,0);  //һ�������֣�����
					vTaskDelay(500);
					move_z(HZ,1000000/F_Z_f);
					dasifu_daowei();
					D2lie++;
					if(D2lie==yizai2_lie)   //�������̵����Ѿ�װ��
					{
						D2lie=0;
						D2hang++;
						if(D2hang==yizai2_hang)//���������Ѿ�װ��
						{
							D2hang=0;
							/*
								�������̵����ʹ��˶�������һ�����������͵�ָ��λ��
							*/
						}
					}
					break;
			}
		}
	}
	//Ҫ���Ե������Ѿ����
}
void yizai2_shoudong(void)
{
	vTaskDelete(YZTask2_Handler); //ɾ����ʼ����
}




void yizai2_restart(void)//�����Զ�����
{
	vTaskDelete(YZTask2_Handler); //ɾ����ʼ����
	taskENTER_CRITICAL();//�����ٽ��������б���
	//����yizai����
  xTaskCreate((TaskFunction_t )yizai2,     
              (const char*    )"yz2_task",   
              (uint16_t       )YZ2_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )YZ2_TASK_PRIO,
              (TaskHandle_t*  )&YZTask2_Handler); 
	vTaskSuspend(YZTask2_Handler);//�����Զ���������
	taskEXIT_CRITICAL();          //�˳��ٽ���	
}
//������ͣ��������������
void yizai2_stop(u8 yn)
{
	if(yn)//�ָ�����
	{		
		vTaskResume(YZTask2_Handler);	//�ָ�����1
		yizai2_runing=0;
		return;
	}
	else 
	{
		vTaskSuspend(YZTask2_Handler);//��������
		yizai2_runing=1;
		return;
	}		
}
//�������̲�����ʱ���Զ�����������
void piaoman_canshu(u8 hang,u8 lie,u8 l,u16 d1x,u16 d2x)//�������̲���
{
	if(yizai2_runing)
	{
		yizai2_hang=hang;
		yizai2_lie =lie;
		L=l;
		D1X=d1x;
		D2X=d2x;
		printf("���̲�����%d��,%d��,%dѨ���,D1X=%d,D2X=%d\r\n",yizai2_hang,yizai2_lie,L,D1X,D2X);	
	}
	else
	{
		printf("�Զ���������ִ��\r\n");
	}
}

void set_yizai2_F(u16 i)//���ô��ŷ��������ٶȼ�Ƶ��f
{
	F_Z_f=i;
}

void xiaosifu_v(u16 XV)
{
	xiao_v=XV;
}//�жϴ��ŷ��Ƿ���������
void dasifu_daowei(void)//�жϴ��ŷ��Ƿ�λ���������λ���ܽ��������Ĳ���
{
	vTaskDelay(500);
	while(1)
	{
		vTaskDelay(500);
		if(TIM2_IRQ) 
		{
			printf("(%d,%d,%d)\r\n",X/44,Y/404,Z/44);
			return;
		}		
	}
}

//�ж�С�ŷ��Ƿ񵽴�ָ��λ��
void xiaosifu_daowei(long Ys)
{
	Now_open();
	while(1)
		{
			vTaskDelay(10);
			if(Ys>(Real_Position_Value[0]+500) || Ys<(Real_Position_Value[0]+500))//���1mm
			{
				Now_close();
				return;
			}
		}			
}
//���λ�����У�������HZ���ף�����������L����
void yudingwei(u16 h)//��׼һ�����̺�Ĳ�������¼��Ҫ�ڴ�ֱ���������еľ���
{//Ĭ��HZ=0,���С��50mm,��ǿ������Ϊ50mm
	if(h>=50)//���õ���С��������50mm
	{
		HZ=h*44;
	}
	else
	{
		HZ=50*44;
	}		
	Can_rest();Z=HZ; //��ʱ��ʵ�������������
	{UP_MOVE Pulse_output(1000,HZ);}//����h����
	dasifu_daowei(); //���ŷ��Ƿ�λ
	printf("����%d\r\n",h);
	X=yizai2_hang*L*44;
	{LEFT_MOVE Pulse_output(1000,L*44);}//����һ������Ѩ�ľ���	
	dasifu_daowei(); //���ŷ��Ƿ�λ
	printf("����%d\r\n",L);
	
	
}

void START_DSF(u8 K_G)
{
	switch(K_G)
	{
		case 1:
			Relay_test(1,1);//�ȴ򿪴��ŷ��ĵ�Դ
			vTaskDelay(2000);//�ȴ�һ��ʱ�䡣���ŷ���������ȫͨ��
			Relay_test(2,1);//���ŷ�û�б�բ
			Relay_test(3,1);
			return;
		case 0:
			Relay_test(2,0);//���ŷ���բ
			Relay_test(3,0);
			vTaskDelay(2000);//�ȴ�һ��ʱ�䡣���ŷ���������ȫͨ��
			Relay_test(1,0);//�̿����ŷ��ĵ�Դ
			return;
	}	
}
