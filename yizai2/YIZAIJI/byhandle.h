#ifndef __BYHANDLE_H
#define __BYHANDLE_H
#include "sys.h"

void Set_F(u16 f);
void Handle_right_start(u16 Fh_f);
void Handle_left_start(u16 Fh_f);
void Handle_down_start(u16 Fh_f);
void Handle_up_start(u16 Fh_f);
void Handle_stop(void);

void Break_(u8 s);


#endif


