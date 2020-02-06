#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加)
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "sdram.h"
#include "ltdc.h"

#include "timer.h"
#include "fdcan.h"

#include "transplant.h"
#include  "set_io.h"
#include "exti.h"
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
    (void*)read_addr,(u8*)"u32 read_addr(u32 addr)",
    (void*)write_addr,(u8*)"void write_addr(u32 addr,u32 val)",
#endif
    (void*)delay_ms,(u8*)"void delay_ms(u16 nms)",
    (void*)delay_us,(u8*)"void delay_us(u32 nus)",
//    (void*)LCD_Clear,(u8*)"void LCD_Clear(u16 Color)",
//    (void*)LCD_Fill,(u8*)"void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)",
//    (void*)LCD_DrawLine,(u8*)"void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)",
//    (void*)LCD_DrawRectangle,(u8*)"void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)",
//    (void*)LCD_Draw_Circle,(u8*)"void Draw_Circle(u16 x0,u16 y0,u8 r)",
//    (void*)LCD_ShowNum,(u8*)"void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)",
//    (void*)LCD_ShowString,(u8*)"void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)",
//    (void*)LCD_Fast_DrawPoint,(u8*)"void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)",
//    (void*)LCD_ReadPoint,(u8*)"u16 LCD_ReadPoint(u16 x,u16 y)",
//    (void*)LCD_Display_Dir,(u8*)"void LCD_Display_Dir(u8 dir)",
//    (void*)LCD_ShowxNum,(u8*)"void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)",

//    (void*)LTDC_Switch,(u8*)"void LTDC_Switch(u8 sw)",
//    (void*)LTDC_Layer_Switch,(u8*)"void LTDC_Layer_Switch(u8 layerx,u8 sw)",
//    (void*)LTDC_Select_Layer,(u8*)"void LTDC_Select_Layer(u8 layerx)",
//    (void*)LTDC_Display_Dir,(u8*)"void LTDC_Display_Dir(u8 dir)",
//    (void*)LTDC_Draw_Point,(u8*)"void LTDC_Draw_Point(u16 x,u16 y,u32 color)",
//    (void*)LTDC_Read_Point,(u8*)"u32 LTDC_Read_Point(u16 x,u16 y)",
//    (void*)LTDC_Fill,(u8*)"void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)",
//    (void*)LTDC_Layer_Window_Config,(u8*)"void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)",
//    (void*)LTDC_Clear,(u8*)"void LTDC_Clear(u32 color)",
    (void*)Plus_out,(u8*)"void Plus_out(u32 HZ,u32 plus_n)",
    (void*)Plus_out_stop,(u8*)"void Plus_out_stop()",
    (void*)planting_stop,(u8*)"void planting_stop()",
    (void*)planting_start,(u8*)"void planting_start()",
		(void*)REY_n,(u8*)"void REY_n(u8 i,u8 n);",
		(void*)set_zero,(u8*)"void set_zero(void)",
		(void*)show_n,(u8*)"void show_n(void)",			
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

