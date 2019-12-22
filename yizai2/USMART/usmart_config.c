#include "usmart.h"
#include "usmart_str.h" 

////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"		
#include "sys.h"

#include "can_motor.h"
#include "advance_time.h"
//#include "motor_big.h"
#include "yizai2.h"
#include "Big_motor.h"
#include "relay.h"
#include "byhandle.h"
 
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif
	
	/*С�ŷ������λ*/
	(void*)Can_rest,"void Can_rest(void)",

//	/*С�ŷ�0ģʽ�ٶȺ�λ��*/
	(void*)Can_v0_p0,"void Can_v0_p0(u16 v,u32 p)",

//	/*С�ŷ�0ģʽ�ٶȺ�λ��*/
	(void*)Can_v1_p1,"void Can_v1_p1(u16 v,u32 p)",
		
	//С�ŷ�����
	(void*)can_set_relay_out,"void can_set_relay_out(char Group,char Number,u8 status)",

//	//С�ŷ�λ�÷�����
//	(void*)Now_open,"void Now_open(void)",
//		
//	//С�ŷ�λ�÷����ر�
//	(void*)Now_close,"void Now_close(void)",
//	
//	//С�ŷ�
//	(void*)move_y,"void move_y(u32 MY)",
//	//���ŷ�
//	(void*)move_x,"void move_x(u32 MX)",
//	//���Ի��Զ�����
//	(void*)yizai_run,"void yizai_run(void)",
//	(void*)yizai_stop,"void yizai_stop(void)",
//	(void*)yizai_restart,"void yizai_restart(void)",
//	//������ʼ��
//	(void*)Init_number,"void Init_number(void)",
//	//�ı��ʼ������
//	(void*)Clear_st,"void Clear_st(void)",
//	(void*)Init_number,"void Init_number(void)",
		
	
//	(void*)Set_miaopan,"void Set_miaopan(u8 detal,u8 C, u8 R)",
//	(void*)Start_init,"void Start_init(u16 lx,u16 ly,u32 z,u16 d1,u16 d2,u32 dz)",
//	(void*)get_x,"u32 get_x(void)",
//	(void*)get_z,"u32 get_z(void)",
//	(void*)set_Z0,"void set_Z0(void)",
//	(void*)set_X0,"void set_X0(void)",
//	(void*)set_Y0,"void set_Y0(void)",
//	//��λ�����Ƿ�����
//	(void*)Limit_key,"u8 Limit_key(void)",

	(void*)Relay_test,"void Relay_test(u8 i,u8 sta)",
	(void*)Set_F,"void Set_F(u16 f)",

		
	//�ֶ�ģʽ��������
	(void*)Handle_right_start,"void Handle_right_start(u16 Fh_f)",
	(void*)Handle_left_start,"void Handle_left_start(u16 Fh_f)",
	(void*)Handle_down_start,"void Handle_down_start(u16 Fh_f)",
	(void*)Handle_up_start,"void Handle_up_start(u16 Fh_f)",
	(void*)Handle_stop,"void Handle_stop()",
	(void*)Break_,"void Break_(u8 s)",
	(void*)xiaosifu_v,"void xiaosifu_v(u16 XV)",//����С�ŷ����ٶ�
	(void*)chushi_weizhi,"void chushi_weizhi()",//�ص���ʼ��λ��
	(void*)yizai2_restart,"void yizai2_restart()",//ɾ���������¿�ʼ������
	(void*)yizai2_stop,"void yizai2_stop(u8 yn)",	//��������Ĺ���ͻָ�
	(void*)piaoman_canshu,"void piaoman_canshu(u8 hang,u8 lie,u8 l,u16 d1x,u16 d2x)",//�������̲���
	(void*)set_yizai2_F,"void set_yizai2_F(u16 i)",//���ô��ŷ��������ٶȼ�Ƶ��f
	(void*)yudingwei,"void yudingwei(u16 h)",//��ʼλ��Ԥ��
	(void*)START_DSF,"void START_DSF(u8 K_G)",
	(void*)yizai2_shoudong,"void yizai2_shoudong(void)",
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















