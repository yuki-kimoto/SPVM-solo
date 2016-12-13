#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"

enum {
  SPerl_OP_INFO_C_CODE_FLAG_CONDITION_LOOP = 1,
};

enum {
  SPerl_OP_INFO_C_CODE_FLAG_BLOCK_CONDITION_TRUE_BLOCK = 1,
};

struct SPerl_op_info {
  SPerl_RESOLVED_TYPE* resolved_type;
  SPerl_int flag;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
