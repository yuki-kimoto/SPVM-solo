#ifndef SPERL_VAR_INFO_H
#define SPERL_VAR_INFO_H

struct SPerl_var_info {
  SPerl_char* name;
  SPerl_int pos;
};

SPerl_VAR_INFO* SPerl_VAR_INFO_new();

#endif
