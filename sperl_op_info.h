#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"

struct SPerl_op_info {
  SPerl_RESOLVED_TYPE* resolved_type;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
