#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"

struct SPerl_op_info {
  SPerl_TYPE* return_type;
  SPerl_ARRAY* argument_types;
  SPerl_int argument_count;
  SPerl_char* symbol;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
