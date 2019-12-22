#include "byhandle.h"
#include "advance_time.h"  //����
#include "led.h"
#include "usart.h"
#include "yizai2.h"
extern u8 yizai2_runing;
extern u16 F_Z_f;//���ŷ�������Ƶ��

void Set_F(u16 f)
{
	F_Z_f=f;
}

void Handle_right_start(u16 Fh_f)
{	
	if(yizai2_runing)
	{
		RIGHT_MOVE Pulse_output(1000000/Fh_f,65535);
	}
	else
	{
		printf("set fall\r\n");
	}
}

void Handle_left_start(u16 Fh_f)
{
	
	if(yizai2_runing)
	{
		LEFT_MOVE Pulse_output(1000000/Fh_f,65535);
	}
	else
	{
		printf("set fall\r\n");
	}
	
}

void Handle_down_start(u16 Fh_f)
{
	
	if(yizai2_runing)
	{
    DOWN_MOVE Pulse_output(1000000/Fh_f,65535);
	}
	else
	{
		printf("set fall\r\n");
	}
}
//�ֶ�����
void Handle_up_start(u16 Fh_f)
{

	if(yizai2_runing)
	{
		UP_MOVE	Pulse_output(1000000/Fh_f,65535);
	}
	else
	{
		printf("set fall\r\n");
	}
}

//�ֶ�ֹͣ
void Handle_stop()
{
	if(yizai2_runing)
	{
		TIM_dis();
	}
	else
	{
		printf("set fall\r\n");
	}
	
}
//����ɲ��
void Break_(u8 s)
{
	TIM_BREAK=s;	
	printf("set fall\r\n");
}
void START()
{
	

}





































