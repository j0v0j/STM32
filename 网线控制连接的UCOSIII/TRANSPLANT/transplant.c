#include "transplant.h"
#include "includes.h"

//������ȼ�����

struct dish_RC dish_rc= {9,0};

//void calcu_rankI_xy()    //����һ�����Ŀ��λ��
//{
//    plant_rank_one->_X=	(dish_init->dis_width)+(dish_init->dx1)
//                       +(plant_rank_one->lie )*(dish_init->derta);
//    plant_rank_one->_Y= (plant_rank_one->hang)*(dish_init->derta);

//}
//void calcu_rankII_xy()    //����������Ŀ��λ��
//{
//    plant_rank_two->_X=	(dish_init->dis_width)+(dish_init->dx1)
//                       +(plant_rank_two->lie )*(dish_init->derta);
//    plant_rank_two->_Y= (plant_rank_two->hang)*(dish_init->derta);

//}

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
//���Ի�е��λ�ó�ʼ��

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
//�����Ի�е�ָ߶ȳ�ʼ��,�����ʼ�������ֵ,lxΪʶ��λ�������Ѩ��,ʵ�ʵ�λΪmm
void position__init(u8 hz,u8 lx)
{
    //����λ�ö�����������������lx,����������hz��ԭ���ɲ�������õ�
    //ִ��������к���
    //ִ���������к���
    //����ʼ���ĸ߶ȸ�ֵ��ȫ�ֱ���
    plant_init->hz=hz;
    plant_init->lx=lx;
    real_position->pos_x=(dish_init->col-1)*(dish_init->derta)+lx;//����ʵ�ʻ����ĳ�ʼλ�ã�mm
    real_position->pos_y1=0;//��е����
    real_position->pos_y2=(dish_init->derta)*7;
    real_position->pos_z=hz;
		dish_init->dis_length=dish_init->row*dish_init->derta;   //�������̵ĳ���
		dish_init->dis_width=dish_init->col*dish_init->derta;    //�������̵Ŀ��
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
    struct positionA PA,PB,PC,PD,PE,PF,PG,PH,PI;
//		struct positionB PB;
//		struct positionC PC;
//		struct positionD PD;
//		struct positionE PE;
//		struct positionF PF;
//    struct positionG PG;
    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
    /*�Զ����Ժ�����ʼ����,dish_rc�ṹ��һֱ���ּ�¼�ţ��Ѿ����Ե�������Ϣ*/
    while(1)
    {
        for(dish_rc.col=(dish_init->col)-1;dish_rc.col>=0;dish_rc.col--)							//������˳������
        {
            for(dish_rc.row=0;dish_rc.row <=(dish_init->row/2)-1; dish_rc.row++)		//������
            {
                /*A->B
                		����A��X����λ��=���������-��Ҫ��Ҫ��ǰŲ���ľ��룬Xa=col_i*dt-lab
                		����y1������λ�ã�Y1=row_i*dt
                		����y2������λ�ã�Y2=(row_i+(dish_init->col)/2)*dt
                		����A��Z����λ�ã�Z=hz
                		A->B��B���x,y���겻��
                		����B ��Z����λ�ã�Z=0��
                	*/
                PA._x  = dish_rc.col*(dish_init->derta)+(plant_init->lbc);
                PA._y1 = dish_rc.row*(dish_init->derta);
                PA._y2 = (dish_rc.row+6)*(dish_init->derta);
                PA._z  = Z_up;
                //�ⲿ��д�˶�ִ�к���
                /*
							   //�˶���A��
                	����C ��X����λ��=��������࣬Xc=col_i*dt
                	���е�C �㣬��е��ȡ�硣
                */
								PB._x	=PA._x;
								PB._y1=PA._y1;
								PB._y2=PA._y2;
                PB._z	=Z_down;
							   //�˶���B��
								
                

                PC._x  = dish_rc.col*(dish_init->derta);							
								PC._y1=PB._y1;
								PC._y2=PB._y2;
                PC._z	=PB._z;
//								�˶���C��
                Left_fetch();					//����ץ��
                Right_fetch();				//����ץ��
                OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms//��ʱһ��ʱ�䣬�����׳������
                /*
                	���е���D�㣬ֻ��Z����ı�
                	����D ������λ�ã�Zd=hz
                */
								PD._x =PC._x;
								PD._y1=PC._y1;
								PD._y2=PC._y2;
                PD._z	=Z_up;
																								

                //���е�D��




                /*
                	D����E��ֻ��Z���겻��
                	����E ��X����λ�ã�X=camera_x
                	����E ��Y1����λ��y1=camera_y1,Y2����λ��y2=camera_y2��
                */
								PE._x =plant_init->camera_x;
								PE._y1=plant_init->camera_y1;
								PE._y2=plant_init->camera_y2;
                PE._z	=PD._z;
								
								
               //�˶���E��
                //��е���ƶ������ʶ������
                //�ƶ���E��X����
//					Can_busone(100,plant_init->camera_y1);
//					Can_bustwo(100,plant_init->camera_y2);


                /*
                	�������ʶ��ABCDEFGHI				M_rankΪʶ���纯���궨�壬�õ�����ĵȼ���Ϣ֮��ŷ���
                */
                //M_rankΪ�궨���һ������������ĵȼ�
                switch(M_rank)
                {
                case A_case: //(0,0) δץȡ������ĵȼ�Ϊ��
                    Left_release();				//���ַ��磬�ӵ�
                    Right_release();			//���ַ��磬�ӵ�
                    clear_rank();					//���ǵ������ȼ���Ϣ
                    break;//
                    /****(0,1)����δץȡ����ȼ�Ϊ����ֵȼ�Ϊ1����***
                    1�����ж�Ҫ���õ����У���ȷ�����õ����У������ǣ���������-ż��ż�У�������򽫿�����ִ�б���
                    2������Ҫ���õ�X����=���̿��+�����̼��+���������С�Ѩ�̼��
                    3������Ҫ���õ�Y����=���������С�Ѩ�̼��
                    4���ӵ���
                    5����ʼִ�ж���
                    6��������ɺ󣬶�Ѩ�����״̬���������趨��1����0<=������ż��<=10ʱ��ֻ�е�����Ϊ10���ܼ�1
                    																					 2����1<=����������<=11ʱ��ֻ�е�����Ϊ11���ܼ�1
                    																					 3�������������
                    ************************************/

                case B_case:     //(0,1)Y1Ҫ�ƶ���������λ��
										Left_release();				//���ַ��磬�ӵ�
								
                    if(plant_rank_one->lie%2==0 && plant_rank_one->hang%2==0)//Ҫ���õ���ż���С�ż����
                    {											
											   PF._x	=(dish_init->dis_width)+(dish_init->dx1		)
															+(plant_rank_one->lie )*(dish_init->derta);
												if((plant_rank_one->hang)>4) //���ż���������ԣ�����������4ʶ��λ�þ��ǲ�����λ��
												{
														PF._y1=PE._y1;												
												}
												else if(plant_rank_one->hang==4 ||plant_rank_one->hang==2)
												{
												    PF._y1=(plant_rank_one->hang-3)*(dish_init->derta);
												}
												else if(plant_rank_one->hang==0)
												{
													  PF._y1=PE._y1-(dish_init->derta);
												}												
												PF._y2=(plant_rank_one->hang)*(dish_init->derta);
												PF._z	=PE._z;																				
                    }
                    else if(plant_rank_one->lie%2==1 && plant_rank_one->hang%2==1)//Ҫ���õ�Ϊ�����С�������
                    {
                        PF._x	=(dish_init->dis_width)+(dish_init->dx1		)
															+(plant_rank_one->lie )*(dish_init->derta);
												if(plant_rank_one->hang>5) //���ż���������ԣ�����������4ʶ��λ�þ��ǲ�����λ��
												{
														PF._y1=PE._y1-(dish_init->derta);												
												}
												else if(plant_rank_one->hang==5 ||plant_rank_one->hang==3)
												{
												    PF._y1=(plant_rank_one->hang-3)*(dish_init->derta);
												}
												else if(plant_rank_one->hang==1)
												{
													  PF._y1=PE._y1-2*(dish_init->derta);
												}												
												PF._y2=(plant_rank_one->hang)*(dish_init->derta);
												PF._z	=PE._z;	
                    }
                    else
                    {   //���ִ�����������������������ִ��
                        planting_ERR();
                    }
                    /*ִ�ж���������е���˶���ָ����λ�ã�����δ���·���*/

										//���е�F��
										
										
										
										PG._x =PF._x;
										PG._y1 =PF._y1;
										PG._y2 =PF._y2;
										PG._z =Z_down;
										
										
										//���е�G��
										Right_release();	//���ַ���
										
                    
                    PF._x =PG._x;
										PF._y1 =PG._y1;
										PF._y2 =PG._y2;
										PF._z =Z_up;
                    //���е�F�� ֮��ص�A��

                    clear_rank();																//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                case C_case: //(0,2) ����δץȡ����ȼ�Ϊ����ֵȼ�Ϊ2����
                    //�����ֵ����ӵ������ƶ����������λ��
                    Left_release();       //���ַ��磬�ӵ�
                
                    if(plant_rank_one->lie%2==0 && plant_rank_one->hang%2==0)//Ҫ���õ���ż���С�ż����
                    {                     
                         PH._x  =(dish_init->dis_width)+(dish_init->dx1   )
                              +(plant_rank_one->lie )*(dish_init->derta);
                        if((plant_rank_one->hang)>4) //���ż���������ԣ�����������4ʶ��λ�þ��ǲ�����λ��
                        {
                            PH._y1=PE._y1;                        
                        }
                        else if(plant_rank_one->hang==4 ||plant_rank_one->hang==2)
                        {
                            PH._y1=(plant_rank_one->hang-3)*(dish_init->derta);
                        }
                        else if(plant_rank_one->hang==0)
                        {
                            PH._y1=PE._y1-(dish_init->derta);
                        }                       
                        PH._y2=(plant_rank_one->hang)*(dish_init->derta);
                        PH._z =PE._z;                                       
                    }
                    else if(plant_rank_one->lie%2==1 && plant_rank_one->hang%2==1)//Ҫ���õ�Ϊ�����С�������
                    {
                        PH._x =(dish_init->dis_width)+(dish_init->dx1   )
                              +(plant_rank_one->lie )*(dish_init->derta);
                        if(plant_rank_one->hang>5) //���ż���������ԣ�����������4ʶ��λ�þ��ǲ�����λ��
                        {
                            PH._y1=PE._y1-(dish_init->derta);                       
                        }
                        else if(plant_rank_one->hang==5 ||plant_rank_one->hang==3)
                        {
                            PH._y1=(plant_rank_one->hang-3)*(dish_init->derta);
                        }
                        else if(plant_rank_one->hang==1)
                        {
                            PH._y1=PE._y1-2*(dish_init->derta);
                        }                       
                        PH._y2=(plant_rank_one->hang)*(dish_init->derta);
                        PH._z =PE._z; 
                    }
                    else
                    {   //���ִ�����������������������ִ��
                        planting_ERR();
                    }
                    /*ִ�ж���������е���˶���ָ����λ�ã�����δ���·���*/

                    //���е�H��
                    
                    
                    
                    PI._x =PH._x;
                    PI._y1 =PH._y1;
                    PI._y2 =PH._y2;
                    PI._z =Z_down;
                    
                    
                    //���е�I��
                    Right_release();  //���ַ���                    
                    
                    PH._x =PI._x;
                    PH._y1 =PI._y1;
                    PH._y2 =PI._y2;
                    PH._z =Z_up;
                    //���е�H�� ֮��ص�A��
                    clear_rank();//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                case D_case: //(1,0) ����δץȡ����ȼ�Ϊ����ֵȼ�Ϊ1����
                    //�����ֵ����ӵ������ƶ����������λ��
                    Right_release();				//�����ַ���
                    clear_rank();//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                case E_case: //(1,1)��ֻ�ֶ���1����
                    //������ֻ�֣��ܲ���һ�η��ꡣ
                    clear_rank();//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                case F_case: //(1,2) ����1���磬����2����
                    clear_rank();//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                case G_case: //(2,0) ����2���磬����δץȡ������ȼ�Ϊ��
                    Right_release();				//�����ַ���
                    clear_rank();//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                case H_case: //(2,1) ����2���磬����1����
                    clear_rank();//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                case I_case: //(2,2) ���ֶ���2���磬
                    //������ֻ���Ƿ��ܹ�һ�η���
                    clear_rank();//���ǵ������ȼ���Ϣ
                    break;
                    /************************************/
                default:
                    break;
                }
								 //��������ɺ����б仯�����趨

                    switch(plant_rank_one->hang)
                    {
                    case 10:		//˵��ż����������ɣ�����������������
                        plant_rank_one->lie++;//��1�����̵��������õ���һ������
                        plant_rank_one->hang = 1;//��һ�δ������п�ʼ����
                        break;
                    case 11:	//˵���������������
                        plant_rank_one->lie++;//��1�����̵��������õ���һż����
                        plant_rank_one->hang = 0;//��һ�δ�ż���п�ʼ����
                        break;
                    default:	//��������Ե��в���10��11��˵��Ҫ���õĲ������һ����
                        plant_rank_one->hang = plant_rank_one->hang+2;//��Ϊ�Ǽ����������������+2

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
        for(u8 i=0; i<10; i++)
            printf("Planting err\r\n");
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
    }
}






