#ifndef SPERL_SUB_H
#define SPERL_THREAD_H

#include "sperl_base.h"

// Method information
struct SPerl_thread {
  int32_t* operand_stack;
  int32_t* call_stack;
};

SPerl_THREAD* SPerl_THREAD_new(SPerl* sperl);

#endif
