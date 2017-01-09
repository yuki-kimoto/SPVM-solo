#ifndef SPERL_FRAME_H
#define SPERL_FRAME_H

#include "sperl_base.h"

// Use information
struct SPerl_frame {
  int32_t* operand_stack;
  int32_t* call_stack;
};

SPerl_FRAME* SPerl_FRAME_new(SPerl* sperl);

#endif
