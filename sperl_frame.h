#ifndef SPERL_FRAME_H
#define SPERL_FRAME_H

#include "sperl.h"

struct SPerl_frame {
  int32_t operand_stack_base;
  int32_t vars_base;
  int32_t operand_stack_top;
  int64_t return_address;
};

SPerl_OP* SPerl_FRAME_newOP(SPerl* sperl);

#endif
