#ifndef SPERL_FRAME_H
#define SPERL_FRAME_H

#include "sperl.h"

struct SPerl_frame {
  int32_t vars_base;
  int64_t return_address;
  int64_t* vars;
  int64_t* operand_stack;
};

SPerl_OP* SPerl_FRAME_newOP(SPerl* sperl);

#endif
