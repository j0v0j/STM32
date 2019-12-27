#include "transplant.h"
#include "includes.h"

//接收苗等级函数

struct dish_RC dish_rc= {9,0};

static void calcu_rankI_xy()    //计算一级苗的目标位置
{
    plant_rank_one->_X=	(dish_init->dis_width)+(dish_init->dx1)
                       +(plant_rank_one->lie )*(dish_init->dta);
    plant_rank_one->_Y= (plant_rank_one->hang)*(dish_init->dta);

}
static void calcu_rankII_xy()    //计算一级苗的目标位置
{
    plant_rank_two->_X=	(dish_init->dis_width)+(dish_init->dx1)
                       +(plant_rank_two->lie )*(dish_init->dta);
    plant_rank_two->_Y= (plant_rank_two->hang)*(dish_init->dta);

}

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
    }
    return 1;
}


void planting()
{
//移栽机械手位置初始化

}

void planting_stop()
{
    plant_status->stop_or_run=Plant_stop;
}
void planting_start()
{
    plant_status->stop_or_run=Plant_run;
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
    real_position->pos_x=(dish_init->col-1)*(dish_init->dta)+lx;//计算实际换算后的初始位置，mm
    real_position->pos_y1=0;//机械手在
    real_position->pos_y2=(dish_init->dta)*7;
    real_position->pos_z=hz;
		dish_init->dis_length=dish_init->row*dish_init->dta;   //计算苗盘的长度
		dish_init->dis_width=dish_init->col*dish_init->dta;    //计算苗盘的宽度
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
    OS_ERR err;
    struct positionA PA,PF;
    struct positionC PC;
    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
    /*自动移栽函数开始运行,dish_rc结构体一直保持记录着，已经移栽的行列信息*/
    while(1)
    {
        dish_rc.col=dish_init->col-1;										//将初始化的苗盘列参数赋值给列记录变量
        while(0<=dish_rc.col && dish_rc.col<dish_init->col)							//列移栽顺序依次
        {
            dish_rc.row=((dish_init->row)/2);//列开始之前
            while(0<=dish_rc.row  &&  dish_rc.row <  ((dish_init->row)/2))		//行移栽
            {
                /*A->B
                		计算A点X坐标位置=列数×间距-需要需要向前挪动的距离，Xa=col_i*dt-lab
                		计算y1点坐标位置，Y1=row_i*dt
                		计算y2点坐标位置，Y2=(row_i+(dish_init->col)/2)*dt
                		计算A点Z坐标位置，Z=hz
                		A->B后，B点的x,y坐标不变
                		计算B 点Z坐标位置，Z=0。
                	*/
                PA._x  = dish_rc.col*(dish_init->dta)-(plant_init->lbc);
                PA._y1 = dish_rc.row*(dish_init->dta);
                PA._y2 = (dish_rc.row+(dish_init->col)/2)*(dish_init->dta);
                PA._z  = Z_down;
                //这部分写运动执行函数
                //
                /*
                	计算C 点X坐标位置=列数×间距，Xc=col_i*dt
                	运行到C 点，机械手取苗。

                */

                //执行运行到C点

                PC._x  = dish_rc.col*(dish_init->dta);
                Left_fetch();					//左手抓苗
                Right_fetch();				//右手抓苗
                OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms//延时一段时间，让气缸彻底伸出
                /*
                	运行到达D点，只有Z坐标改变
                	计算D 点坐标位置，Zd=hz
                */

                //运行到D点




                /*
                	D到达E点只有Z坐标不变
                	计算E 点X坐标位置，X=camera_x
                	到达E 点Y1坐标位置y1=camera_y1,Y2坐标位置y2=camera_y2。
                */

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

                case B_case:     //Y1要移动到不干涉位置
                    if(plant_rank_one->lie%2==0 && plant_rank_one->hang%2==0)//要放置的是偶数列、偶数行
                    {
                        calcu_rankI_xy();
                        //计算不干涉位置，因为是偶数行所以目标行大于6的时候，识别位置就是不干涉位置
                        if(plant_rank_one->hang>=6)
                        {
                            plant_init->Y1_Ncrash_dis=plant_init->camera_y1;//左机械手不干涉位置
                        }
                        else//小于6时，即目标行等于0、2、4
                        {
                            plant_init->Y1_Ncrash_dis=(plant_rank_one->hang-3)*(dish_init->dta);//左机械手不干涉位置
                        }
                    }
                    else if(plant_rank_one->lie%2==1 && plant_rank_one->hang%2==1)//要放置的为奇数列、奇数行
                    {
//                        plant_rank_one->_X=(dish_init->dis_width)+(dish_init->dx1)
//                                           +(plant_rank_one->lie)*(dish_init->dta);
//                        plant_rank_one->_Y=(plant_rank_one->hang)*(dish_init->dta);
												calcu_rankI_xy();
                        //计算不干涉位置，因为是偶数行所以目标行大于6的时候，识别位置就是不干涉位置
                        if(plant_rank_one->hang>=6)
                        {
                            plant_init->Y1_Ncrash_dis=plant_init->camera_y1;//左机械手不干涉位置
                        }
                        else//小于6时，即目标行等于0、2、4
                        {
                            plant_init->Y1_Ncrash_dis=(plant_rank_one->hang-3)*(dish_init->dta);//左机械手不干涉位置
                        }
                    }
                    else
                    {   //出现错误阻塞在这里，正常情况不会执行
                        planting_ERR();
                    }
                    /*执行动作，将机械手运动到指定的位置，但还未向下放苗*/
										
										
										
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
                    Left_release();	//左手放苗
                    //不干涉位置计算
                    Can_busone(100,plant_init->Y1_Ncrash_dis);	//左机械手移动到不干涉的位置

                    Can_bustwo(100,plant_init->Y1_Ncrash_dis+2);//右手要移栽位置













                    clear_rank();																//最后记得清除苗等级信息
                    break;
                    /************************************/
                case C_case: //(0,2) 左手未抓取或苗等级为差，右手等级为2级苗
                    //将左手的苗扔掉，并移动到不干涉的位置
                    Left_release();				//左手放苗
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case D_case: //(1,0) 右手未抓取或苗等级为差，左手等级为1级苗
                    //将右手的苗扔掉，并移动到不干涉的位置
                    Right_release();				//有右手放苗
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case E_case: //(1,1)两只手都是1级苗
                    //区分两只手，能不能一次放完。
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case F_case: //(1,2) 左手1级苗，右手2级苗
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case G_case: //(2,0) 左手2级苗，右手未抓取或者苗等级为差
                    Right_release();				//有右手放苗
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case H_case: //(2,1) 左手2级苗，右手1级苗
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                case I_case: //(2,2) 两手都是2级苗，
                    //区分两只手是否能够一次放完
                    clear_rank();//最后记得清除苗等级信息
                    break;
                    /************************************/
                default:
                    break;
                }

//				dish_rc.row--;
            }


            dish_rc.col--;//完成一列的移栽，将列数减1
        }
//		for(u8 col_i=dish_init->row-1;col_i<=0;col_i--)//列移栽
//		{
//				for(u8 row_i=0;row_i<(dish_init->row)/2;row_i++)//行移栽
//				{
//
//
//				}
//		}
    }
}


void planting_ERR(void)
{
    OS_ERR err;
    while(1)
    {
        for(u8 i=0; i<10; i++)
            printf("Planting err\r\n");
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
    }
}






