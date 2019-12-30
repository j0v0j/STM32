#include "set_f.h"


struct set__f *set_f;

/*分别设置每个阶段的运行频率*/
void set_F_AB(u16 ab)
{
    set_f->AB=ab;
}
void set_F_BC(u16 bc)
{
    set_f->BC=bc;
}
void set_F_CD(u16 cd)
{
    set_f->CD=cd;
}
void set_F_DE(u16 de)
{
    set_f->DE=de;
}
void set_F_EF(u16 ef)
{
    set_f->EF=ef;
}
void set_F_FG(u16 fg)
{
    set_f->FG=fg;
}
void set_F_GF(u16 gf)
{
    set_f->GF=gf;
}
void set_F_FA(u16 fa)
{
    set_f->FA=fa;
}
//一次性设置所有运行阶段的频率

void set_F_all(u16 ab,u16 bc,u16 cd,u16 de,u16 ef,u16 fg,u16 gf,u16 fa)
{
    set_F_AB(ab);
    set_F_BC(bc);
    set_F_CD(cd);
    set_F_DE(de);
    set_F_EF(ef);
    set_F_FG(fg);
    set_F_GF(gf);
    set_F_FA(fa);
}
void F_init()
{
    set_F_all(1000,1000,1000,1000,1000,1000,1000,1000);
}


