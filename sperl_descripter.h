#ifndef SPERL_DESCRIPTER_H
#define SPERL_DESCRIPTER_H

#define SPerl_DESCRIPTER_CONST 1
#define SPerl_DESCRIPTER_STATIC 2

SPerl_char SPerl_DESCRIPTER_get_flag(SPerl_char* desc_str);
void SPerl_DESCRIPTER_to_str(SPerl_char* str, SPerl_char flags);

#endif
