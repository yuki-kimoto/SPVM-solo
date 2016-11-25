#ifndef SPERL_VAR_H
#define SPERL_VAR_H

#include "sperl_base.h"

struct SPerl_var {
  SPerl_OP* op_name;
  SPerl_MY_VAR* my_var;
  SPerl_int version;
};

SPerl_VAR* SPerl_VAR_new(SPerl_PARSER* parser);

#endif
