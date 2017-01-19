#ifndef SPERL_FRAME_H
#define SPERL_FRAME_H

#include "sperl.h"

struct SPerl_frame {
  int64_t* operand_stack;
  int64_t* call_stack;
};

SPerl_OP* SPerl_FRAME_newOP(SPerl* sperl);

#endif
