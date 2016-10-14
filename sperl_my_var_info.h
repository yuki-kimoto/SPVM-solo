#ifndef SPERL_MY_VAR_INFO_H
#define SPERL_MY_VAR_INFO_H

#include "sperl_type.h"

struct SPerl_my_var_info {
  SPerl_char* type;
  SPerl_char desc_flags;
  SPerl_char* name;
  SPerl_int id;
  SPerl_METHOD_INFO* method_info;
  SPerl_ARRAY* descripters;
  SPerl_OP* op;
};

SPerl_MY_VAR_INFO* SPerl_MY_VAR_INFO_new();

#endif
