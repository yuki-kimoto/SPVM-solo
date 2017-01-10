#ifndef SPERL_MY_VAR_H
#define SPERL_MY_VAR_H

#include "sperl_base.h"

struct SPerl_my_var {
  SPerl_OP* op_type;
  SPerl_OP* op_name;
  int32_t pos;
};

SPerl_MY_VAR* SPerl_MY_VAR_new();

#endif
