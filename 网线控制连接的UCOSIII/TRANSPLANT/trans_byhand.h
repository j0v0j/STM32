#ifndef 	_TRANS_BYHAND_H
#define 	_TRANS_BYHAND_H


#include 	"sys.h"

//#define   Handle_right_start(u16 f)
//#define   Handle_left_start(u16 f)
//#define   Handle_up_start(u16 f)
//#define   Handle_down_start(u16 f)
//#define   Handle_stop(void)
/*手动状态下，横移运行，及移栽手*/
#define   Can_busone(v,p) 		Handle_canbus_one(v,p);
#define   Can_bustwo(v,p) 		Handle_canbus_two(v,p);


/*手动状态下，十字运行*/
void 	Handle_right_start(u16 f);
void 	Handle_left_start(u16 f);
void 	Handle_up_start(u16 f);
void 	Handle_down_start(u16 f);
void 	Handle_stop(void);
/*手动状态下，横移运行，及移栽手*/
void 	Handle_canbus_one(u16 v,u32 p);
void 	Handle_canbus_two(u16 v,u32 p);

//两只手的取放苗函数
void 	leftcan_fetch(void);			//左手抓苗
void 	rightcan_fetch(void);			//右手抓苗
void 	leftcan_release(void);		//左手放苗
void 	rightcan_release(void);	//右手放苗

//



#endif




