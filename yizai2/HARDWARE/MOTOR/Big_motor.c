#include "Big_motor.h"
#include "advance_time.h"
#include "led.h"
#include "yizai.h"
#include "usart.h"
#include "Big_motor.h"

s32 X=0; 				  //��ʱ��ʵ������
u32 Y=0;					//��ʱ��ʵ������
s32 Z=0;          //����ʮ�������ʱ��Z��ֵΪ0
void move_x(u32 MX,u16 f)//MXΪ����λ�õ��������
{	
	if(X>=0)
	{
		printf("\r\nMX=%d,X=%d\r\n",MX,X);
		if(MX>X)//Ŀ��λ�ô�������λ�ã������˶�
		{	
			//��Ҫ�ƶ��ľ���
			//�ı䷽��
			LEFT_MOVE
			Pulse_output(f,MX-X);
			X=MX;
			
		}
		else if(MX<X)//Ŀ��λ��С������λ�ã������˶�
		{	
			//��Ҫ�ƶ��ľ���
			//�ı䷽��
			RIGHT_MOVE
			Pulse_output(f,X-MX);
			X=MX;
		}
		else//Ŀ��λ�ú�����λ����ͬ
		{
			printf("\r\nsame position��\r\n");
		}
	
	}
	else
	{
		printf("\r\nmove_x����δִ�У�\r\n");
	
	}
}
void move_z(s32 MZ,u16 f)
{
	if(Z>=0)
	{
		printf("\r\nZ=%d\r\n",MZ);
		if(MZ<Z)//ʮ������
		{	
			//��Ҫ�ƶ��ľ���	
			DOWN_MOVE
			Pulse_output(f,Z-MZ);
			Z=MZ;
		}
		else if(MZ>Z)//ʮ������
		{		
			UP_MOVE
			Pulse_output(f,MZ-Z);
			Z=MZ;
			
		}
		else//Ŀ��λ�ú�����λ����ͬ
		{
			printf("\r\nsame position��\r\n");
		}
		
	}
	else
	{
		printf("\r\nmove_z����δִ�У�\r\n");
	}
}
//u32 get_x(void)
//{
//	printf("\r\nX=%d\r\n",X);
//	return X;
//}
//u32 get_z(void)
//{
//	printf("\r\nZ=%d\r\n",Z);
//	return Z;
//}
//void set_Z0(void)
//{
//	Z=0;
//}
//void set_X0(void)
//{
//	X=0;
//}
//void set_Y0(void)
//{
//	Y=0;
//}



