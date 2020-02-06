#include "transplant.h"
#include "includes.h"

#include "timer.h"

//MQTT任务
//任务优先级
#define Auto_runing_TASK_PRIO	12
//任务堆栈大小
#define Auto_runing_STK_SIZE	128
//任务控制块
OS_TCB Auto_runingTaskTCB;
//任务堆栈
CPU_STK	Auto_runing_TASK_STK[Auto_runing_STK_SIZE];
//任务函数




struct dish_RC dish_rc= {9,0};
struct plant__status *plant_status;
extern struct PLUS__NUM PLUS_NUM;
OS_ERR err;

//接收苗等级函数
void plant_rank(u8 dj)
{
    plant_init->rank=dj;

}
u8 wait_rank()
{
    while(1)
    {
        if(plant_init->rank !=9)
            break;
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
    }
    return 1;
}


void planting()
{
//移栽机械手位置初始化
    if(plant_status->init==1)
    {


    }

}

void planting_stop()      //暂停实质上是将脉冲输出定时器的时钟disable
{
    plant_status->stop_or_run=Plant_stop;
    PLUS_NUM.stop_flag=1;
    __HAL_RCC_TIM8_CLK_DISABLE();
    OS_TaskSuspend((OS_TCB*)&Auto_runingTaskTCB,&err);		//挂起开始任务

}
void planting_start()
{
    OSTaskResume((OS_TCB*)&Auto_runingTaskTCB,&err);
    plant_status->stop_or_run=Plant_run;
    PLUS_NUM.stop_flag=0;
    __HAL_RCC_TIM8_CLK_ENABLE();

}
void planting_restart()
{
    plant_status->stop_or_run=Plant_stop;
}
//将移栽机械手高度初始化,这个初始化是相对值,lx为识别位置相对于穴孔,实际单位为mm
void position__init(u8 hz,u8 lx)
{
    //将苗位置对齐后，先向后运行运行lx,再向上运行hz，原点由参数推算得到
    //执行向后运行函数
    //执行向上运行函数
    //将初始化的高度赋值给全局变量
    plant_init->hz=hz;
    plant_init->lx=lx;
    real_position->pos_x=(dish_init->col-1)*(dish_init->derta)+lx;//计算实际换算后的初始位置，mm
    real_position->pos_y1=0;//机械手在
    real_position->pos_y2=(dish_init->derta)*7;
    real_position->pos_z=hz;
    dish_init->dis_length=dish_init->row*dish_init->derta;   //计算苗盘的长度
    dish_init->dis_width=dish_init->col*dish_init->derta;    //计算苗盘的宽度
    plant_status->init=1;
}
/*
			      -------->              ----->				   ----->
			D---------A--------------E------------F-------------------H
		  |			    |						<--------				|			 <----				|
	 /|\|					|		|												|										|	|
		|	|					|		|									 	/|\	|	|							 /|\|	|
		|	|					|	 \|/									 |	|\|/							|	|\|/
			|				  |														|										|
			C---------B														G										I
					<--
*/
void Auto_runing(void *pdata)
{

    struct position PA,PB,PC,PD,PE,PF,PG,PH,PI;
    static u8 i=0;
//    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
    /*自动移栽函数开始运行,dish_rc结构体一直保持记录着，已经移栽的行列信息*/
		
    while(1)
    {
			   i++;
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms

			if(i==200)
			{
				i=0;
				 printf("Auto_runing.....\r\n");
			}
        for(dish_rc.col=(dish_init->col)-1; dish_rc.col>=0; dish_rc.col--)							//列移栽顺序依次
        {
            for(dish_rc.row=0; dish_rc.row <=(dish_init->row/2)-1; dish_rc.row++)		//行移栽
            {
                /*A->B
                		计算A点X坐标位置=列数×间距-需要需要向前挪动的距离，Xa=col_i*dt-lab
                		计算y1点坐标位置，Y1=row_i*dt
                		计算y2点坐标位置，Y2=(row_i+(dish_init->col)/2)*dt
                		计算A点Z坐标位置，Z=hz
                		A->B后，B点的x,y坐标不变
                		计算B 点Z坐标位置，Z=0。
                	*/
                PA._x  = dish_rc.col*(dish_init->derta)+(plant_init->lbc);
                PA._y1 = dish_rc.row*(dish_init->derta);
                PA._y2 = (dish_rc.row+6)*(dish_init->derta);
                PA._z  = Z_up;
                //这部分写运动执行函数
                /*
                			   运动到A点
                	计算C 点X坐标位置=列数×间距，Xc=col_i*dt
                	运行到C 点，机械手取苗。
                */
                PB._x	=PA._x;
                PB._y1=PA._y1;
                PB._y2=PA._y2;
                PB._z	=Z_down;
                //**********************************************运动到B点**********************************************



                PC._x  = dish_rc.col*(dish_init->derta);
                PC._y1=PB._y1;
                PC._y2=PB._y2;
                PC._z	=PB._z;
//								**********************************************运动到C点 **********************************************
                Left_fetch();					//左手抓苗
                Right_fetch();				//右手抓苗
                OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms//延时一段时间，让气缸彻底伸出
                /*
                	运行到达D点，只有Z坐标改变
                	计算D 点坐标位置，Zd=hz
                */
                PD._x =PC._x;
                PD._y1=PC._y1;
                PD._y2=PC._y2;
                PD._z	=Z_up;


                //**********************************************运行到D点**********************************************




                /*
                	D到达E点只有Z坐标不变
                	计算E 点X坐标位置，X=camera_x
                	到达E 点Y1坐标位置y1=camera_y1,Y2坐标位置y2=camera_y2。
                */
                PE._x =plant_init->camera_x;
                PE._y1=plant_init->camera_y1;
                PE._y2=plant_init->camera_y2;
                PE._z	=PD._z;


                //**********************************************运动到E点**********************************************
                //机械手移动到相机识别区域
                //移动到E点X方向
//					Can_busone(100,plant_init->camera_y1);
//					Can_bustwo(100,plant_init->camera_y2);


                /*
                	对苗进行识别ABCDEFGHI				M_rank为识别苗函数宏定义，得到了苗的等级信息之后才返回
                */
                //M_rank为宏定义的一个函数返回苗的等级
                switch(M_rank)
                {
                case A_case: //(0,0) 未抓取或者苗的等级为差
                    Left_release();				//左手放苗，扔掉
                    Right_release();			//右手放苗，扔掉
                    clear_rank();					//最后记得清除苗等级信息
                    break;//
                    /****(0,1)左手未抓取或苗等级为差，右手等级为1级苗***
                    1、先判断要放置到哪列，再确定放置到哪行，必须是：奇行奇列-偶行偶列，否则程序将卡死不执行报错
                    2、计算要放置的X坐标=苗盘宽度+两苗盘间距+移栽所在列×穴盘间距
                    3、计算要放置的Y坐标=移栽所在行×穴盘间距
                    4、扔掉苗
                    5、开始执行动作
                    6、动作完成后，对穴盘苗的状态进行整理设定：1）当0<=苗盘是偶列<=10时，只有当行数为10才能加1
                    																					 2）当1<=苗盘是奇列<=11时，只有当行数为11才能加1
                    																					 3）其他情况报错
                    ************************************/

                case B_case:     //(0,1)Y1要移动到不干涉位置
                    Left_release();				//左手放苗，扔掉

                    if(plant_rank_one->lie%2==0 && plant_rank_one->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PF._x	=(dish_init->dis_width)+(dish_init->dx1		)
                                 +(plant_rank_one->lie )*(dish_init->derta);
                        if((plant_rank_one->hang)>4) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PF._y1=PE._y1;
                        }
                        else if(plant_rank_one->hang==4 ||plant_rank_one->hang==2)
                        {
                            PF._y1=(plant_rank_one->hang-3)*(dish_init->derta);
                        }
                        else if(plant_rank_one->hang==0)
                        {
                            PF._y1=PE._y1-5*(dish_init->derta);
                        }
                        PF._y2=(plant_rank_one->hang)*(dish_init->derta);
                        PF._z	=PE._z;
                    }
                    else if(plant_rank_one->lie%2==1 && plant_rank_one->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PF._x	=(dish_init->dis_width)+(dish_init->dx1		)
                                 +(plant_rank_one->lie )*(dish_init->derta);
                        if(plant_rank_one->hang>5) //如果奇数行列移栽，当右手目标行数大于5左手识别位置减一个小格位置就是不干涉位置
                        {
                            PF._y1=PE._y1-(dish_init->derta);
                        }
                        else if(plant_rank_one->hang==5 ||plant_rank_one->hang==3)
                        {
                            PF._y1=(plant_rank_one->hang-3)*(dish_init->derta);
                        }
                        else if(plant_rank_one->hang==1)
                        {
                            PF._y1=PE._y1-4*(dish_init->derta);
                        }
                        PF._y2=(plant_rank_one->hang)*(dish_init->derta);
                        PF._z	=PE._z;
                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    /*执行动作，将机械手运动到指定的位置，但还未向下放苗*/

                    //**********************************************运行到F点**********************************************



                    PG._x =PF._x;
                    PG._y1 =PF._y1;
                    PG._y2 =PF._y2;
                    PG._z =Z_down;


                    //运行到G点
                    Right_release();	//左手放苗


                    PF._x =PG._x;
                    PF._y1 =PG._y1;
                    PF._y2 =PG._y2;
                    PF._z =Z_up;
                    //**********************************************运行到F点 之后回到A点**********************************************

                    clear_rank();																//最后记得清除苗等级信息
                    break;
                    /************************************/
                case C_case: //(0,2) 左手未抓取或苗等级为差，右手等级为2级苗
                    //将左手的苗扔掉，并移动到不干涉的位置
                    Left_release();       //左手放苗，扔掉

                    if(plant_rank_two->lie%2==0 && plant_rank_two->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PH._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_two->lie )*(dish_init->derta);
                        if((plant_rank_two->hang)>4) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PH._y1=PE._y1;
                        }
                        else if(plant_rank_two->hang==4 ||plant_rank_two->hang==2)
                        {
                            PH._y1=(plant_rank_two->hang-3)*(dish_init->derta);
                        }
                        else if(plant_rank_two->hang==0)
                        {
                            PH._y1=PE._y1-5*(dish_init->derta);
                        }
                        PH._y2=(plant_rank_two->hang)*(dish_init->derta);
                        PH._z =PE._z;
                    }
                    else if(plant_rank_two->lie%2==1 && plant_rank_two->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PH._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_two->lie )*(dish_init->derta);
                        if(plant_rank_two->hang>5) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PH._y1=PE._y1-(dish_init->derta);
                        }
                        else if(plant_rank_two->hang==5 ||plant_rank_two->hang==3)
                        {
                            PH._y1=(plant_rank_two->hang-3)*(dish_init->derta);
                        }
                        else if(plant_rank_two->hang==1)
                        {
                            PH._y1=PE._y1-4*(dish_init->derta);
                        }
                        PH._y2=(plant_rank_two->hang)*(dish_init->derta);
                        PH._z =PE._z;
                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    /*执行动作，将机械手运动到指定的位置，但还未向下放苗*/

                    //**********************************************运行到H点**********************************************



                    PI._x =PH._x;
                    PI._y1 =PH._y1;
                    PI._y2 =PH._y2;
                    PI._z =Z_down;


                    //运行到I点
                    Right_release();  //左手放苗

                    PH._x =PI._x;
                    PH._y1 =PI._y1;
                    PH._y2 =PI._y2;
                    PH._z =Z_up;
                    //**********************************************运行到H点 之后回到A点**********************************************
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case D_case: //(1,0) 左手等级为1级苗，右手未抓取或苗等级为差
                    //将右手的苗扔掉，并移动到不干涉的位置
                    Right_release();				//右手放苗

                    if(plant_rank_two->lie%2==0 && plant_rank_two->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PF._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_two->lie )*(dish_init->derta);
                        if((plant_rank_two->hang)<=4) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PF._y2=PE._y2+(dish_init->derta);
                        }
                        else if(plant_rank_two->hang==6 ||plant_rank_two->hang==8)
                        {
                            PF._y2=(plant_rank_two->hang+3)*(dish_init->derta);
                        }
                        else if(plant_rank_two->hang==10)
                        {
                            PF._y2=PE._y2+4*(dish_init->derta);
                        }
                        PF._y1=(plant_rank_two->hang)*(dish_init->derta);
                        PF._z =PE._z;
                    }
                    else if(plant_rank_two->lie%2==1 && plant_rank_two->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PF._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_two->lie )*(dish_init->derta);
                        if(plant_rank_two->hang<=5) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PF._y2=PE._y2;
                        }
                        else if(plant_rank_two->hang==7 ||plant_rank_two->hang==9)
                        {
                            PF._y1=(plant_rank_two->hang+3)*(dish_init->derta);
                        }
                        else if(plant_rank_two->hang==11)
                        {
                            PF._y1=PE._y1+5*(dish_init->derta);
                        }
                        PF._y2=(plant_rank_two->hang)*(dish_init->derta);
                        PF._z =PE._z;
                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    /*执行动作，将机械手运动到指定的位置，但还未向下放苗*/

                    //**********************************************运行到F点**********************************************




                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case E_case: //(1,1)两只手都是1级苗
                    //区分两只手，能不能一次放完。
                    if(plant_rank_one->lie%2==0 && plant_rank_one->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PF._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_one->lie )*(dish_init->derta);
                        if((plant_rank_one->hang)<=8) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PF._y1=(plant_rank_one->hang)*(dish_init->derta);
                            PF._y2=(plant_rank_one->hang+2)*(dish_init->derta);
                            PF._z =PE._z;
                        }
                        else if(plant_rank_one->hang==10)       //目标列只能放一棵苗，先把Y2放到目标位置再把Y1需要单独放到下一列，
                        {
                            PF._y1=PE._y1-4*(dish_init->derta);
                            PF._y2=(plant_rank_one->hang)*(dish_init->derta);
                            PF._z =PE._z;
                            //**********************************************移动到F点**********************************************

                            PG._x =PF._x;
                            PG._y1 =PF._y1;
                            PG._y2 =PF._y2;
                            PG._z =Z_down;


                            //**********************************************运行到G点**********************************************
                            Right_release();//右手放苗
                            Left_release();	//左手放苗

                            PF._x =PG._x;
                            PF._y1 =PG._y1;
                            PF._y2 =PG._y2;
                            PF._z =Z_up;
                            //**********************************************移动到F点**********************************************




                            PF._x =PF._x-(dish_init->derta);
                            PF._y1 =1*(dish_init->derta);
                            PF._y2 =PF._y2;
                            PF._z =Z_up;
                            //************************移动到下一列的F点************************



                            PG._x =PF._x;
                            PG._y1 =PF._y1;
                            PG._y2 =PF._y2;
                            PG._z =Z_down;


                            //**********************************************运行到下一列G点**********************************************
                            PF._x =PG._x;
                            PF._y1 =PG._y1;
                            PF._y2 =PG._y2;
                            PF._z =Z_up;
                            //**********************************************返回到F点**********************************************

                        }

                    }
                    else if(plant_rank_one->lie%2==1 && plant_rank_one->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PF._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_one->lie )*(dish_init->derta);
                        if(plant_rank_one->hang<=9) //如果奇数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PF._y1=(plant_rank_one->hang)*(dish_init->derta);
                            PF._y2=(plant_rank_one->hang+2)*(dish_init->derta);
                            PF._z=PE._z;
                            //**********************************************运动到F点**********************************************



                            PG._x =PF._x;
                            PG._y1 =PF._y1;
                            PG._y2 =PF._y2;
                            PG._z =Z_down;
                            //**********************************************运动到G点**********************************************



                            PF._x =PG._x;
                            PF._y1 =PG._y1;
                            PF._y2 =PG._y2;
                            PF._z =Z_up;
                            //**********************************************返回到F点**********************************************



                        }
                        else if(plant_rank_one->hang==11)
                        {
                            PF._y1=		PE._y1-4*(dish_init->derta);
                            PF._y2=(plant_rank_one->hang)*(dish_init->derta);
                            PF._z =PE._z;
                            //**********************************************运行到F点**********************************************



                            PG._x =PF._x;
                            PG._y1 =PF._y1;
                            PG._y2 =PF._y2;
                            PG._z =Z_down;
                            //**********************************************运动到G点**********************************************



                            PF._x =PG._x;
                            PF._y1 =PG._y1;
                            PF._y2 =PG._y2;
                            PF._z =Z_up;
                            //**********************************************返回到F点**********************************************


                            PF._x =PF._x-(dish_init->derta);
                            PF._y1 =0*(dish_init->derta);
                            PF._y2 =PF._y2;
                            PF._z =Z_up;
                            //************************移动到下一列的F点************************



                            PG._x =PF._x;
                            PG._y1 =PF._y1;
                            PG._y2 =PF._y2;
                            PG._z =Z_down;
                            //**********************************************再次运动到G点**********************************************



                            PF._x =PG._x;
                            PF._y1 =PG._y1;
                            PF._y2 =PG._y2;
                            PF._z =Z_up;
                            //**********************************************再次返回到F点**********************************************


                        }
                        else
                        {   //出现错误阻塞在这里，正常情况不会执行
                            planting_ERR();

                        }

                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case F_case: //(1,2) 左手1级苗，右手2级苗   2020-01-09  先放1级苗，再放2级苗
                    if(plant_rank_one->lie%2==0 && plant_rank_one->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PF._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_one->lie )*(dish_init->derta);
                        PF._y1=(plant_rank_one->hang)*(dish_init->derta);
                        if((plant_rank_one->hang)<=6) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {

                            PF._y2=PE._y2+(dish_init->derta);

                        }
                        else if((plant_rank_one->hang)==8)
                        {
                            PF._y2=PE._y2+3*(dish_init->derta);

                        }
                        else if((plant_rank_one->hang)==10)
                        {
                            PF._y2=PE._y2+2*(dish_init->derta);

                        }
                        PF._z =PE._z;


                        //****************移动到F点



                        PG._x= PF._x;
                        PG._y1= PF._y1;
                        PG._y2= PF._y2;
                        PG._z= Z_down;
                        //****************移动到G点




                        PF._x= PG._x;
                        PF._y1= PG._y1;
                        PF._y2= PG._y2;
                        PF._z= Z_up;
                        //****************移动到F点




                    }
                    else if(plant_rank_one->lie%2==1 && plant_rank_one->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PF._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_one->lie )*(dish_init->derta);


                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
										
										
										
										//开始移栽2级苗
                    if(plant_rank_two->lie%2==0 && plant_rank_two->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PH._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_two->lie )*(dish_init->derta);


                    }
                    else if(plant_rank_two->lie%2==1 && plant_rank_two->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PH._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_two->lie )*(dish_init->derta);


                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }





                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case G_case: //(2,0) 左手2级苗，右手未抓取或者苗等级为差
                    Right_release();				//有右手放苗
								    if(plant_rank_two->lie%2==0 && plant_rank_two->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PH._x	=(dish_init->dis_width)+(dish_init->dx2		)
                                 +(plant_rank_two->lie )*(dish_init->derta);
                        if((plant_rank_two->hang)>8) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PH._y2=PE._y2+1*(dish_init->derta);
                        }
                        else if(plant_rank_two->hang==8)
                        {
                            PH._y2=(plant_rank_two->hang+3)*(dish_init->derta);
                        }
												else if(plant_rank_two->hang==10)
                        {
                            PH._y2=(plant_rank_two->hang+2)*(dish_init->derta);
                        }
                       
                        PH._y1=(plant_rank_two->hang)*(dish_init->derta);
                        PH._z	=PE._z;
                    }
                    else if(plant_rank_two->lie%2==1 && plant_rank_two->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PH._x	=(dish_init->dis_width)+(dish_init->dx1		)
                                 +(plant_rank_two->lie )*(dish_init->derta);
                        if(plant_rank_two->hang<=5) //如果奇数行列移栽，当右手目标行数大于5左手识别位置减一个小格位置就是不干涉位置
                        {
                            PH._y2=PE._y2;
                        }
                        else if(plant_rank_two->hang<=9)
                        {
                            PH._y2=(plant_rank_two->hang+3)*(dish_init->derta);
                        }
												else if(plant_rank_two->hang==11)
                        {
                            PH._y2=(plant_rank_two->hang+2)*(dish_init->derta);
                        }
                       
                        PH._y1=(plant_rank_two->hang)*(dish_init->derta);
                        PH._z	=PH._z;
                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    /*执行动作，将机械手运动到指定的位置，但还未向下放苗*/

                    //**********************************************运行到H点**********************************************



                    PI._x =PH._x;
                    PI._y1 =PH._y1;
                    PI._y2 =PH._y2;
                    PI._z =Z_down;


                    //**********************************************运行到I点
                    Right_release();	//左手放苗


                    PH._x =PI._x;
                    PH._y1 =PI._y1;
                    PH._y2 =PI._y2;
                    PH._z =Z_up;
                    //******************************************************************//
								
								
									
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case H_case: //(2,1) 左手2级苗，右手1级苗
										 if(plant_rank_one->lie%2==0 && plant_rank_one->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PF._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_one->lie )*(dish_init->derta);
                        PF._y1=(plant_rank_one->hang)*(dish_init->derta);
                        if((plant_rank_one->hang)<=6) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {

                            PF._y2=PE._y2+(dish_init->derta);

                        }
                        else if((plant_rank_one->hang)==8)
                        {
                            PF._y2=PE._y2+3*(dish_init->derta);

                        }
                        else if((plant_rank_one->hang)==10)
                        {
                            PF._y2=PE._y2+2*(dish_init->derta);

                        }
                        PF._z =PE._z;


                        //****************移动到F点



                        PG._x= PF._x;
                        PG._y1= PF._y1;
                        PG._y2= PF._y2;
                        PG._z= Z_down;
                        //****************移动到G点




                        PF._x= PG._x;
                        PF._y1= PG._y1;
                        PF._y2= PG._y2;
                        PF._z= Z_up;
                        //****************移动到F点




                    }
                    else if(plant_rank_one->lie%2==1 && plant_rank_one->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PF._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_one->lie )*(dish_init->derta);


                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
										
										
										
										//开始移栽2级苗
                    if(plant_rank_two->lie%2==0 && plant_rank_two->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PH._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_two->lie )*(dish_init->derta);


                    }
                    else if(plant_rank_two->lie%2==1 && plant_rank_two->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PH._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_two->lie )*(dish_init->derta);


                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case I_case: //(2,2) 两手都是2级苗，
                    //区分两只手是否能够一次放完
										 if(plant_rank_two->lie%2==0 && plant_rank_two->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        PH._x  =(dish_init->dis_width)+(dish_init->dx1   )
                                +(plant_rank_two->lie )*(dish_init->derta);
                        if((plant_rank_two->hang)<=8) //如果偶数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PH._y1=(plant_rank_two->hang)*(dish_init->derta);
                            PH._y2=(plant_rank_two->hang+2)*(dish_init->derta);
                            PH._z =PE._z;
                        }
                        else if(plant_rank_two->hang==10)       //目标列只能放一棵苗，先把Y2放到目标位置再把Y1需要单独放到下一列，
                        {
                            PH._y1=PE._y1-4*(dish_init->derta);
                            PH._y2=(plant_rank_two->hang)*(dish_init->derta);
                            PH._z =PE._z;
                            //**********************************************移动到F点**********************************************

                            PI._x =PH._x;
                            PI._y1 =PH._y1;
                            PI._y2 =PH._y2;
                            PI._z =Z_down;


                            //**********************************************运行到G点**********************************************
                            Right_release();//右手放苗
                            Left_release();	//左手放苗

                            PH._x =PI._x;
                            PH._y1 =PI._y1;
                            PH._y2 =PI._y2;
                            PH._z =Z_up;
                            //**********************************************移动到F点**********************************************




                            PH._x =PH._x-(dish_init->derta);
                            PH._y1 =1*(dish_init->derta);
                            PH._y2 =PH._y2;
                            PH._z =Z_up;
                            //************************移动到下一列的F点************************



                            PI._x =PH._x;
                            PI._y1 =PH._y1;
                            PI._y2 =PH._y2;
                            PI._z =Z_down;


                            //**********************************************运行到下一列G点**********************************************
                            PH._x =PI._x;
                            PH._y1 =PI._y1;
                            PH._y2 =PI._y2;
                            PH._z =Z_up;
                            //**********************************************返回到F点**********************************************

                        }

                    }
                    else if(plant_rank_two->lie%2==1 && plant_rank_two->hang%2==1)//要放置的为奇数列、奇数行
                    {
                        PH._x =(dish_init->dis_width)+(dish_init->dx1   )
                               +(plant_rank_two->lie )*(dish_init->derta);
                        if(plant_rank_two->hang<=9) //如果奇数行列移栽，当行数大于4识别位置就是不干涉位置
                        {
                            PH._y1=(plant_rank_two->hang)*(dish_init->derta);
                            PH._y2=(plant_rank_two->hang+2)*(dish_init->derta);
                            PH._z=PE._z;
                            //**********************************************运动到F点**********************************************



                            PI._x =PH._x;
                            PI._y1 =PH._y1;
                            PI._y2 =PH._y2;
                            PI._z =Z_down;
                            //**********************************************运动到G点**********************************************



                            PH._x =PI._x;
                            PH._y1 =PI._y1;
                            PH._y2 =PI._y2;
                            PH._z =Z_up;
                            //**********************************************返回到F点**********************************************



                        }
                        else if(plant_rank_two->hang==11)
                        {
                            PH._y1=		PE._y1-4*(dish_init->derta);
                            PH._y2=(plant_rank_two->hang)*(dish_init->derta);
                            PH._z =PE._z;
                            //**********************************************运行到F点**********************************************



                            PI._x =PH._x;
                            PI._y1 =PH._y1;
                            PI._y2 =PH._y2;
                            PI._z =Z_down;
                            //**********************************************运动到G点**********************************************



                            PH._x =PI._x;
                            PH._y1 =PI._y1;
                            PH._y2 =PI._y2;
                            PH._z =Z_up;
                            //**********************************************返回到F点**********************************************


                            PH._x =PH._x-(dish_init->derta);
                            PH._y1 =0*(dish_init->derta);
                            PH._y2 =PH._y2;
                            PH._z =Z_up;
                            //************************移动到下一列的F点************************



                            PI._x =PH._x;
                            PI._y1 =PH._y1;
                            PI._y2 =PH._y2;
                            PI._z =Z_down;
                            //**********************************************再次运动到G点**********************************************



                            PH._x =PI._x;
                            PH._y1 =PI._y1;
                            PH._y2 =PI._y2;
                            PH._z =Z_up;
                            //**********************************************再次返回到F点**********************************************


                        }
                        else
                        {   //出现错误阻塞在这里，正常情况不会执行
                            planting_ERR();

                        }

                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                default:
                    break;
                }
                //对移栽完成后行列变化进行设定

                switch(plant_rank_one->hang)
                {
                case 10:		//说明偶数列移栽完成，设置奇行奇列移栽
                    plant_rank_one->lie++;//将1级苗盘的移栽设置到下一奇数列
                    plant_rank_one->hang = 1;//下一次从奇数行开始放苗
                    break;
                case 11:	//说明奇数行移栽完成
                    plant_rank_one->lie++;//将1级苗盘的移栽设置到下一偶数列
                    plant_rank_one->hang = 0;//下一次从偶数行开始放苗
                    break;
                default:	//如果所移栽的行不是10或11，说明要放置的不是最后一个苗
                    plant_rank_one->hang = plant_rank_one->hang+2;//因为是间隔放置所以这里是+2

                }
								switch(plant_rank_two->hang)
                {
                case 10:		//说明偶数列移栽完成，设置奇行奇列移栽
                    plant_rank_two->lie++;//将1级苗盘的移栽设置到下一奇数列
                    plant_rank_two->hang = 1;//下一次从奇数行开始放苗
                    break;
                case 11:	//说明奇数行移栽完成
                    plant_rank_two->lie++;//将1级苗盘的移栽设置到下一偶数列
                    plant_rank_two->hang = 0;//下一次从偶数行开始放苗
                    break;
                default:	//如果所移栽的行不是10或11，说明要放置的不是最后一个苗
                    plant_rank_two->hang = plant_rank_two->hang+2;//因为是间隔放置所以这里是+2

                }




            }
        }
    }
}


void planting_ERR(void)
{
    OS_ERR err;
    while(1)
    {
        for(u8 i=0; i<10; i++) {
            printf("Planting err\r\n");
            OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
        }
    }
}








