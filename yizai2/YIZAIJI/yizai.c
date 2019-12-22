#include "rs485.h"
#include "yizai.h"
#include "Big_motor.h"
#include "can_motor.h"
#include "advance_time.h"
#include "usart.h"
#include "can.h"
#include "yizai.h"
#include "delay.h"
#include "usart3.h"
#include "key.h"
#include "string.h"
#include "led.h"
#include "ekey.h"
#include "remote.h"
#include "beep.h"   //��������
#include "byhandle.h"

extern u8 TIM2_IRQ;  //���巢����ɱ�־
extern long Real_Position_Value[4];  //���ŷ���ɱ�־
extern short Real_Velocity_Value[4];
extern u8 mx[3],my[3],dj;
extern u8 RS485_RX_CNT;
extern u8 RS485_flag;
u8 yizai_flag=1;
//������
TaskHandle_t YZTask_Handler;


/***************************************************		
	���ŷ�����;��뻻��   x=n*0.0226    
	С�ŷ�qc�;��뻻��   y=n*0.00245    
	l=45mm�����ŷ��������1991    С�ŷ�qc=18367
	���ŷ�1mm��Ӧ44����(����)           С�ŷ�1mm��Ӧ408qc(����)
****************************************************/

//u16  LX=4400;  	//ԭʼ���̵�һ�յ�ԭ��100mm
//u32  LY=40800;  //ԭʼ���̵�һ�յ�ԭ��ĳ���100mm
//u32  D1=88000;   //һ�����̵�һ�յ�ԭ��ĳ���2000mm
//u32 D2=132000;   //�������̵�һ�յ�ԭ��ĳ���3000mm
//u16  ll=18360;  	 //������Ѩ���45mm
//u16  l=1980;  	 //ʮ����Ѩ���45mm
u16  LX=0;  	//ԭʼ���̵�һ�յ�ԭ��100mm
u32  LY=0;  //ԭʼ���̵�һ�յ�ԭ��ĳ���100mm
u32  D1=0;   //һ�����̵�һ�յ�ԭ��ĳ���2000mm
u32 D2=0;   //�������̵�һ�յ�ԭ��ĳ���3000mm
u16  ll=18360;  	 //������Ѩ���45mm
u16  l=1980;  	 //ʮ����Ѩ���45mm
u8 St_init=0;		//���̲�����ʼ����־λ

//��λΪ�������
extern s32 X; 			  	 //��ʱ��ʵ�������������
extern u32 Y;						 //��ʱ��ʵ�������������
extern s32 Z;
u8 Reset_flag=0;
u8 i_1=0,j_1=0;			 //һ���������
u8 i_2=0,j_2=0;			 //�����������
u32 x_0=0,y_0=0;			 //ԭʼ�������
u32 x_1=0,y_1=0;			 //һ���������
u32 x_2=0,y_2=0;			 //�����������
u32 Dx;							 //���ŷ�X�����ƶ�����
u32 Dy;							 //С�ŷ��ƶ�����
u32 DZ=8800;	
u16 F_S=10000;
u8 hang=6,lie=12;
/*�����Ծ��뺯���ĳ�ʼ��
void Init_number(void); �鿴��ʼ���ĺ���        
void Start_init(u32 lx,u32 ly,u32 d1,u32 d2,u32 dz);��ʼ������
void Clear_st(void);���³�ʼ��
void Set_F(u16 f);��������Ƶ��65535
u8 Limit_key(void);�鿴��λ
u8 Reset_run(void);��λ
void yizai_run(void);�����Զ�
void P2P(u16 x1,u16 y1);������λ���ƶ���Ŀ��λ��
*/

//void Set_F(u16 f)
//{ //Ƶ�ʻ���
//	if(f<=0 | f>=30000)//���Ƶ������Ϊ0�����ߴ���30kHz��ô�ʹ�����ɲ��
//	{
//		TIM_BREAK=1;
//		printf("break_plus");
//	}
//	else
//	{
//		F_S=f;
//		printf("plus=%d",F_S);
//	}
//	
//}

void Init_number(void)
{
	printf("\r\n����LX=%dmm\r\n",LX/44);
	printf("LY =%dmm\r\n",LY/408);
	printf("D1 =%dmm\r\n",D1/44);
	printf("D2 =%dmm\r\n",D2/44);
	printf("DZ =%dmm\r\n",DZ/44);
	printf("F_S=%dmm\r\n",F_S);	
}

void Set_miaopan(u8 detal,u8 C, u8 R)
{
	l=detal*44;
	ll=detal*408;
	hang =C;
	lie  =R;
}

void Start_init(u32 lx,u32 ly,u32 z,u32 d1,u32 d2,u32 dz)//��ʼ������LX,LY,d1,d2
{
	if(!St_init)
	{	//��ʵ�ʵľ���ת��Ϊ�������qc
		LX=44*lx;
		LY=408*ly;
		D1=44*d1;
		D2=44*d2;
		DZ=44*dz;
		Z=z;
		can_set_relay_out(0,1,0);//����������
		
		St_init=1;	
		
	}
	else
	{
		printf("δ��λ,Clear_st()\r\n");
	}
	
}


void Clear_st(void)//�����ʼ������
{
	St_init=0;
}


void CrossReady(void)//ʮ�ֵ�λ�ж�
{
	while(1)
	{
		
		if(TIM2_IRQ)
			{
				printf("���ŷ���λ��\r\n");
				return;
			}
	}
}

void HyReady(void)//С�ŷ���λ�ж�
{	
	Now_open();
	vTaskDelay(10);
//	delay_xms(1000);
	while(1)
		{
//			u3_printf("{\"p1\":%ld}\r\n",Real_Position_Value[0]);
//			vTaskDelay(100);
//			u3_printf("{\"v1\":%ld}\r\n",Real_Velocity_Value[0]);	
			if(Dy>(Real_Position_Value[0]+500) || Dy<(Real_Position_Value[0]+500))//���1mm
			{
				printf("С�ŷ���λ��%ld\r\n",Real_Position_Value[0]);
//				u3_printf("{\"p1\":%ld}\r\n",Real_Position_Value[0]);
				vTaskDelay(100);
//				u3_printf("{\"v1\":%ld}\r\n",Real_Velocity_Value[0]);	
				Now_close();
				return;
			}
		}			
}
//һ��һ���İ���ִ�У�������WKUP��ִ����һ����
void key_enter(void)
{
//	u8 keys;
//	printf("\r\nWKUP_PRES continue\r\n");
	while(1)
		{
			delay_ms(1000);
			break;
//			keys=Remote_Scan();	
//			BEEP=1;
//			if(WKUP_PRES==KEY_Scan(0) || 162==Remote_Scan())
//			{
//				BEEP=0;
//				printf("WKUP_PRES press\r\n");
//				break;
//			}
//			vTaskDelay(100);
		}
}
//�����Ǽ����λ�����Ƿ�����
//ps:������д�ţ��Ժ�������ôд�ȽϺã�if  else  ̫���ˡ�
u8 Limit_key(void)
{
	if(EKEY0)
	{
//		u3_printf("{\"break0\":1}\r\n");
		printf("\r\nlimit Right ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break0\":0}\r\n");
		printf("\r\nlimit Right err.\r\n");
		
	}
	if(EKEY1)
	{
//		u3_printf("{\"break1\":1}\r\n");
		printf("\r\nlimit Up ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break1\":0}\r\n");		
		printf("\r\nlimit Up err.\r\n");
	}
	if(EKEY2)
	{
//		u3_printf("{\"break2\":1}\r\n");
		printf("\r\nlimit Left ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break2\":0}\r\n");
		printf("\r\nlimit Left err.\r\n");
	}
	if(EKEY3)
	{
//		u3_printf("{\"break3\":1}\r\n");
		printf("\r\nlimit Down ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break3\":0}\r\n");
		printf("\r\nlimit Down err.\r\n");
	}
	if(EKEY4)
	{
//		u3_printf("{\"break4\":1}\r\n");
		printf("\r\nLittle serve ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break4\":0}\r\n");
		printf("\r\nLittle serve err.\r\n");
	}
	if(EKEY5)
	{
//		u3_printf("{\"break5\":1}\r\n");
		printf("\r\nLittle serve ok.\r\n");
	}
	else
	{
//		u3_printf("{\"break5\":0}\r\n");
		printf("\r\nLittle serve err.\r\n");
	}
	if(EKEY5 && EKEY4 && EKEY3 && EKEY2 && EKEY1 && EKEY0)
		return 1;
	else
		return 0;
}
//��λ������δ����ʱ���ⲿ�ж�IO�ĵ�ƽ����,�ڴ�����λ���ƽ���ͣ�IOΪ�ڲ�������
u8 Reset_run(void)
{	 
	Reset_flag=Limit_key();  //����λ����״̬���м���	
	if(Reset_flag)//��λ��������
	{//������ʮ�����������нӴ�����λ
		Turn1=RESET;
		Turn2=RESET;
		Pulse_output(100,10000);
		//������ʮ�������߽Ӵ�����λ
		while(1)//��ʮ�����е�����λ������������һ�ξ��룬������ִ���������в���
		{
			if(!EKEY1)	//�ȴ���������λ
			{
				printf("\r\nlimit UP��\r\n");

				Turn1=SET;  //��������һ�ξ���
				Turn2=SET;
				vTaskDelay(1000);
				Pulse_output(100,8800);//
				while(1)       //�뿪����λλ��
				{
					if(EKEY1)
					{
						TIM_dis();
						printf("reset up_down\r\n");
						break;
					}
						vTaskDelay(100);
					
				}
				
				//��������һ�ξ���
				vTaskDelay(1000);				
				Turn1=RESET;
				Turn2=SET;
				Pulse_output(100,30000);	
				break;
			}
			vTaskDelay(100);
		}
		
		while(1)	//���߲����Ƿ����
		{
			if(!EKEY0) 	//�ȴ���������λ
			{ //��������һ�ξ���
				printf("\r\nlimit RIGHT��\r\n");
				vTaskDelay(1000);				
				Turn1=SET;
				Turn2=RESET;
				Pulse_output(100,8800);
				while(1)       //�뿪����λλ��
				{ 
					if(EKEY0)
					{
						TIM_dis();
						printf("reset right_left\r\n");
						break;
					}
					vTaskDelay(100);				
				}
				
				break;
			}
			vTaskDelay(100);
		}
		Reset_flag=2;
		printf("\r\nReset OK\r\n");
		return 1;
	}
	else
	{
		printf("\r\nLimit key err.\r\n");
		return 0;
	}
}


u8 ZB[]={0};//RS485�ķ�������buffer
//��ʶ��ȴ�����
void Waiting_dj()
{
	u32 t=0;
	u8 keys=0;
	//�±ߵ����while�ǵȴ����ڷ���������ȼ�
	//�õ���ȼ�,��ĵȼ��洢�ڱ�����dj ����,
	while(1)
	{//����һ�εȴ�ʶ������ȼ��ĳ���ÿ��һ��ʱ��ִ��һ�εڶ���if���
		t++;
		keys=0;
		vTaskDelay(100);
		keys=Remote_Scan();
		switch(keys)
		{
						case 104: //ң��1��
							dj=1;
							RS485_flag=0;
							return;
						case 152: //ң��2��
							dj=2;
							RS485_flag=0;
							return;
		}
	  if(RS485_flag==0)
		{
						RS485_flag=1;
						printf("��ȼ��������\r\n");
//						u3_printf("{\"p3\":\"��ȼ��������\"}\r\n");
						return;
		}
		if (t==20)
		{    
			t=0;
			sprintf((char*)ZB,"DJ\r\n");
			RS485_Send_Data(ZB,strlen((char*)ZB));
			printf("�ȴ�ʶ����ȼ�\r\n");				
			}
		}
}



void yizai_task(void *pvParameters)
{
//	u8 t_num=0;
	while(1)
	{
	if(St_init)
	{
		move_z(DZ,1000000/F_S);			//ʮ�ִӸߴ���ʼִ�г�������150mm
		for(u8 j=0;j<hang;j++)		//��ѭ��
		{
			x_0=LX+j*l;//��������
			for(u8 i=0;i<lie;i++)//��ѭ��
			{		
				sprintf((char*)ZB,"{\"X\":%d,\"Y\":%d}\r\n",j,i);
				RS485_Send_Data(ZB,strlen((char*)ZB));
				//����ԭʼ���̵��������				
				y_0=LY+i*ll;//��������
				//�ƶ������λ��
				Dx=x_0;//������ŷ������Ҫ�ƶ��ľ���
				move_x(Dx,1000000/F_S);//������������
				CrossReady();//���ŷ��Ƿ�λ
				Dy=y_0;//����С�ŷ������Ҫ�ƶ��ľ���	
				move_y(Dy);							
				HyReady();		//С�ŷ��Ƿ�λ	
				printf("\r\nץ��ǰλ��X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
				key_enter();
				move_z(DZ,1000000/F_S);//��ʮ�������˶�׼������
				CrossReady();//���ŷ��Ƿ�λ
				printf("\r\nץ��ʱλ��X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
				//****************************************************************
				//****************************************************************
				key_enter();
				can_set_relay_out(0,1,1);//�����֣���ȡ��
				printf("������צץ��\r\n");	
				key_enter();
				move_z(0,1000000/F_S);//��ʮ�������˶�
				CrossReady();//���ŷ��Ƿ�λ
				printf("\r\nץ���λ��X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);	
				Waiting_dj();
				//RS485���ж��н�������
				//Waiting_dj()��whileѭ���˳�˵����ȼ�ʶ���Ѿ����
				key_enter();
//				u3_printf("{\"m1\":%d}\r\n",dj);
				switch(dj) 
				{
							
					case 1://һ��������
						//����һ�����̵����Ե�
							printf("һ����\r\n");
							x_1=D1+j_1*l;					//����һ�����̵�����
							y_1=LY+i_1*ll;					
						//�ƶ���Ŀ��λ����Ҫ�ľ���
						Dx=x_1;
						Dy=y_1;
						move_x(Dx,1000000/F_S);
						CrossReady(); 		//���ŷ��Ƿ�λ
						move_y(Dy);
						HyReady();				//С�ŷ��Ƿ�λ					
						//׼������
						move_z(DZ,1000000/F_S);				//��ʮ�������˶�׼������
						CrossReady(); 		//���ŷ������Ƿ�λ
						printf("\r\n����ʱλ��X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
						key_enter();
						can_set_relay_out(0,1,0);//�����֣�����
//						u3_printf("{\"m1y\":%d,\"m1x\":%d}\r\n",i_1,j_1);
						printf("\r\n�����ַ���\r\n");
						key_enter();									
						move_z(0,1000000/F_S);			//��ʮ�������˶�׼��
						CrossReady(); 		//���ŷ������Ƿ�λ
							i_1++;          	//Ϊ������һ��׼��
						//һ�����̰�˳��ڷ�
						if(i_1%lie==0)   	  	//���ڴ�0��ʼ�����������һ��ѭ����12˵����������
						{
							j_1++;			  	//������һ��
							if(j_1==hang)	  	//һ��������
							{
								u3_printf("{\"m1y\":188,\"m1x\":188}\r\n");
								j_1=0;					
							}
							i_1=0;
						}						
						
						printf("\r\n�����λ��X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
						key_enter();
						break;
					case 2://����������
						//����һ�����̵����Ե�
							printf("\r\n������");
							x_2=D2+j_2*l;
							y_2=LY+i_2*ll;

						Dx=x_2;
						Dy=y_2;
						move_x(Dx,1000000/F_S);
						CrossReady(); //���ŷ��Ƿ�λ
						move_y(Dy);
						HyReady();		//С�ŷ��Ƿ�λ									
						//׼������
						move_z(DZ,1000000/F_S);//��ʮ�������˶�׼������
						CrossReady(); //���ŷ������Ƿ�λ
						printf("\r\n����ʱλ��X=%d,Y=%d,Z=%d",X/44,Y/408,Z/44);
						key_enter();
						can_set_relay_out(0,1,0);//�����֣�����
						u3_printf("{\"m2y\":%d,\"m2x\":%d}\r\n",i_2,j_2);
						key_enter();
						move_z(0,1000000/F_S);//��ʮ�������˶�׼��
						CrossReady(); //���ŷ������Ƿ�λ
						i_2++;
						if(i_2%lie==0)
						{
							j_2++;
							if(j_2==hang)//����������
							{
								u3_printf("{\"m2y\":188,\"m2x\":188}\r\n");
								j_2=0;					
							}
							i_2=0;
						}
						printf("\r\n�����λ��X=%d,Y=%d,Z=%d\r\n",X/44,Y/408,Z/44);
						key_enter();
						break;
				}
			}
			//ԭʼ��һ�����������	
		}
		//ԭʼ��һ�����������
//		u3_printf("{\"my\":188,\"mx\":188}\r\n");
	}
	else
	{
//		t_num++;
//		printf("t_num=%d\r\n",t_num);
		if(!St_init)
			printf("\r\n���Ƚ������̳�ʼ��");	
		if(Reset_flag!=2)
			printf(",��λʧ��");
		vTaskDelay(500);	
	}
	vTaskDelay(1000);			//��ʱ10ms 	

}
	}

//�Զ�����ָ�
void yizai_run(void *pvParameters)//���Ի��Զ�����
{
	vTaskResume(YZTask_Handler);	//�ָ�����1
	yizai_flag=0;
	printf("\r\n�ָ����Ե�����!");
}
//�Զ�������ͣ
void yizai_stop(void *pvParameters)//���Ի��Զ�����
{
	vTaskSuspend(YZTask_Handler);//��������
	yizai_flag=1;
	printf("\r\n�������Ե�����!");
}
//�Զ�������������,ɾ������->�½�����
void yizai_restart(void)
{
	vTaskDelete(YZTask_Handler); //ɾ����ʼ����
//	vTaskDelay(1000);
	taskENTER_CRITICAL();//�����ٽ��������б���
		//����yizai����
  xTaskCreate((TaskFunction_t )yizai_task,     
              (const char*    )"yz_task",   
              (uint16_t       )YZ_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )YZ_TASK_PRIO,
              (TaskHandle_t*  )&YZTask_Handler); 
	vTaskSuspend(YZTask_Handler);//�����Զ���������
	taskEXIT_CRITICAL();            //�˳��ٽ���						
}

//��һ������λ���˶���Ŀ��ģ�x1,y1��
void P2P(u16 x1,u16 y1)
{
	Dx=408*y1;
	Dy=44*x1;
	move_z(0,1000000/F_S);//�����ʮ���������˶�׼��	
	CrossReady(); //���ŷ��Ƿ�λ
	move_x(Dx,1000000/F_S);
	vTaskDelay(500);
	move_y(Dy);
	CrossReady(); //���ŷ��Ƿ�λ
	HyReady();		//С�ŷ��Ƿ�λ
	move_z(DZ,1000000/F_S);//��ʮ�������˶�׼������
	CrossReady(); //���ŷ��Ƿ�λ
}




