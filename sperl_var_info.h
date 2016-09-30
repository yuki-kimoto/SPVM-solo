#ifndef SPERL_VAR_INFO_H
#define SPERL_VAR_INFO_H

#include "sperl_type.h"

struct SPerl_var_info {
  SPerl_char* name;
  SPerl_MY_VAR_INFO* my_var_info;
  SPerl_int version;
};

SPerl_VAR_INFO* SPerl_VAR_INFO_new();

#endif
