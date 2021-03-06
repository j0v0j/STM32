#ifndef __CAN_H__
#define __CAN_H__
#include "sys.h"


#define OpenLoop_Mode                       0x01    //开环模式
#define Current_Mode                        0x02    //电流模式
#define Velocity_Mode                       0x03		//速度模式
#define Position_Mode                       0x04		//位置模式
#define Velocity_Position_Mode              0x05		//速度位置模式
#define Current_Velocity_Mode               0x06		//电流速度模式
#define Current_Position_Mode               0x07		//电流位置模式
#define Current_Velocity_Position_Mode      0x08		//电流速度位置模式
#define CAN_Failed                           1
#define CAN_Ok                               0

void CAN1_Configuration(void);

void CAN_DRV_Reset(unsigned char Group,unsigned char Number);
u8 CAN_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode);
void CAN_DRV_OpenLoop_Mode(unsigned char Group,unsigned char Number,short Temp_PWM);
void CAN_DRV_Current_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current);
void CAN_DRV_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity);
void CAN_DRV_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position);
void CAN_DRV_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position);
void CAN_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity);
void CAN_DRV_Current_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position);
void CAN_DRV_Current_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,long Temp_Position);

void can_set_relay_out(unsigned char Group,unsigned char Number,u8 status);


void CAN_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2);
void CAN_DRV_Online_Check(unsigned char Group,unsigned char Number);

extern short Real_Current_Value[4];
extern short Real_Velocity_Value[4];
extern long Real_Position_Value[4];
extern char Real_Online[4];
extern char Real_Ctl1_Value[4];
extern char Real_Ctl2_Value[4];
#define abx(x) ((x)>0? (x):(-(x)));

#endif

