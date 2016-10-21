#ifndef SPERL_MY_VAR_INFO_H
#define SPERL_MY_VAR_INFO_H

#include "sperl_core_type.h"
#include "sperl_type_info.h"

struct SPerl_my_var_info {
  SPerl_TYPE_INFO* type_info;
  SPerl_WORD_INFO* name;
  SPerl_int id;
  SPerl_METHOD_INFO* method_info;
  SPerl_ARRAY* descripter_infos;
  SPerl_OP* op;
};

SPerl_MY_VAR_INFO* SPerl_MY_VAR_INFO_new();

#endif
