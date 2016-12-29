#ifndef SPERL_NAME_INFO_H
#define SPERL_NAME_INFO_H

#include "sperl_base.h"

struct SPerl_name_info {
  SPerl_OP* op_var;
  SPerl_OP* op_name;
  _Bool anon;
  const char* abs_name;
};

SPerl_NAME_INFO* SPerl_NAME_INFO_new(SPerl_PARSER* parser);

#endif
