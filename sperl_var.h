#ifndef SPERL_VAR_H
#define SPERL_VAR_H

#include "sperl_base.h"

struct SPerl_var {
  SPerl_OP* op_name;
  SPerl_OP* op_my_var;
  int32_t version;
};

SPerl_VAR* SPerl_VAR_new(SPerl* sperl);

#endif
