#ifndef 	_TRANS_BYHAND_H
#define 	_TRANS_BYHAND_H


#include 	"sys.h"

//#define   Handle_right_start(u16 f)
//#define   Handle_left_start(u16 f)
//#define   Handle_up_start(u16 f)
//#define   Handle_down_start(u16 f)
//#define   Handle_stop(void)
/*�ֶ�״̬�£��������У���������*/
#define   Can_busone(v,p) 		Handle_canbus_one(v,p);
#define   Can_bustwo(v,p) 		Handle_canbus_two(v,p);


/*�ֶ�״̬�£�ʮ������*/
void 	Handle_right_start(u16 f);
void 	Handle_left_start(u16 f);
void 	Handle_up_start(u16 f);
void 	Handle_down_start(u16 f);
void 	Handle_stop(void);
/*�ֶ�״̬�£��������У���������*/
void 	Handle_canbus_one(u16 v,u32 p);
void 	Handle_canbus_two(u16 v,u32 p);

//��ֻ�ֵ�ȡ���纯��
void 	leftcan_fetch(void);			//����ץ��
void 	rightcan_fetch(void);			//����ץ��
void 	leftcan_release(void);		//���ַ���
void 	rightcan_release(void);	//���ַ���

//



#endif




