#ifndef 	_SET_F_H
#define 	_SET_F_H
#include 	"sys.h"
struct set__f
{
		u8  f_ok;
    u16 AB;			//AB��Ƶ��
    u16 BC;			//BC��Ƶ��
    u16 CD;			//CD��Ƶ��
    u16 DE;			//DE��Ƶ��
    u16 EF;			//EF��Ƶ��
    u16 FG;			//FG��Ƶ��
    u16 GF;			//FG��Ƶ��
    u16 FA;			//GA��Ƶ��
};//set__f={1000,1000,1000,1000,1000,1000,1000};


//Ƶ������
void F_init(void);
void set_F_all(u16 ab,u16 bc,u16 cd,u16 de,u16 ef,u16 fg,u16 gf,u16 fa);
void set_F_AB(u16 ab);
void set_F_BC(u16 bc);
void set_F_CD(u16 cd);
void set_F_DE(u16 de);
void set_F_EF(u16 ef);
void set_F_FG(u16 fg);
void set_F_GF(u16 gf);
void set_F_FA(u16 fa);

#endif


