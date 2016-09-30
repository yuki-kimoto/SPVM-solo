#ifndef SPERL_DESCRIPTER_H
#define SPERL_DESCRIPTER_H

#include "sperl_type.h"

#define SPerl_DESCRIPTER_CONST 1
#define SPerl_DESCRIPTER_STATIC 2
#define SPerl_DESCRIPTER_INTERFACE 4
#define SPerl_DESCRIPTER_VALUE 8

SPerl_char SPerl_DESCRIPTER_get_flag(SPerl_char* desc_str);
void SPerl_DESCRIPTER_to_str(SPerl_char* str, SPerl_char flags);

#endif
