#ifndef SPERL_FRAME_H
#define SPERL_FRAME_H

#include "sperl.h"

struct SPerl_frame {
  int32_t* operand_stack;
  int32_t* call_stack;
};

SPerl_OP* SPerl_FRAME_newOP(SPerl* sperl);

#endif
