#include "usmart.h"
#include "usmart_str.h" 

////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"		
#include "sys.h"

#include "can_motor.h"
#include "advance_time.h"
//#include "motor_big.h"
#include "yizai2.h"
#include "Big_motor.h"
#include "relay.h"
#include "byhandle.h"
 
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif
	
	/*小伺服电机复位*/
	(void*)Can_rest,"void Can_rest(void)",

//	/*小伺服0模式速度和位置*/
	(void*)Can_v0_p0,"void Can_v0_p0(u16 v,u32 p)",

//	/*小伺服0模式速度和位置*/
	(void*)Can_v1_p1,"void Can_v1_p1(u16 v,u32 p)",
		
	//小伺服光耦
	(void*)can_set_relay_out,"void can_set_relay_out(char Group,char Number,u8 status)",

//	//小伺服位置反馈打开
//	(void*)Now_open,"void Now_open(void)",
//		
//	//小伺服位置反馈关闭
//	(void*)Now_close,"void Now_close(void)",
//	
//	//小伺服
//	(void*)move_y,"void move_y(u32 MY)",
//	//大伺服
//	(void*)move_x,"void move_x(u32 MX)",
//	//移栽机自动程序
//	(void*)yizai_run,"void yizai_run(void)",
//	(void*)yizai_stop,"void yizai_stop(void)",
//	(void*)yizai_restart,"void yizai_restart(void)",
//	//参数初始化
//	(void*)Init_number,"void Init_number(void)",
//	//改变初始化参数
//	(void*)Clear_st,"void Clear_st(void)",
//	(void*)Init_number,"void Init_number(void)",
		
	
//	(void*)Set_miaopan,"void Set_miaopan(u8 detal,u8 C, u8 R)",
//	(void*)Start_init,"void Start_init(u16 lx,u16 ly,u32 z,u16 d1,u16 d2,u32 dz)",
//	(void*)get_x,"u32 get_x(void)",
//	(void*)get_z,"u32 get_z(void)",
//	(void*)set_Z0,"void set_Z0(void)",
//	(void*)set_X0,"void set_X0(void)",
//	(void*)set_Y0,"void set_Y0(void)",
//	//限位开关是否正常
//	(void*)Limit_key,"u8 Limit_key(void)",

	(void*)Relay_test,"void Relay_test(u8 i,u8 sta)",
	(void*)Set_F,"void Set_F(u16 f)",

		
	//手动模式操作函数
	(void*)Handle_right_start,"void Handle_right_start(u16 Fh_f)",
	(void*)Handle_left_start,"void Handle_left_start(u16 Fh_f)",
	(void*)Handle_down_start,"void Handle_down_start(u16 Fh_f)",
	(void*)Handle_up_start,"void Handle_up_start(u16 Fh_f)",
	(void*)Handle_stop,"void Handle_stop()",
	(void*)Break_,"void Break_(u8 s)",
	(void*)xiaosifu_v,"void xiaosifu_v(u16 XV)",//设置小伺服的速度
	(void*)chushi_weizhi,"void chushi_weizhi()",//回到初始的位置
	(void*)yizai2_restart,"void yizai2_restart()",//删除任务重新开始新任务
	(void*)yizai2_stop,"void yizai2_stop(u8 yn)",	//移栽任务的挂起和恢复
	(void*)piaoman_canshu,"void piaoman_canshu(u8 hang,u8 lie,u8 l,u16 d1x,u16 d2x)",//设置苗盘参数
	(void*)set_yizai2_F,"void set_yizai2_F(u16 i)",//设置大伺服的运行速度即频率f
	(void*)yudingwei,"void yudingwei(u16 h)",//初始位置预定
	(void*)START_DSF,"void START_DSF(u8 K_G)",
	(void*)yizai2_shoudong,"void yizai2_shoudong(void)",
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















