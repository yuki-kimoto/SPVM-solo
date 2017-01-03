#ifndef SPERL_VM_STACK_H
#define SPERL_VM_STACK_H

#include "sperl_base.h"

// Use information
struct SPerl_vm_stack {
  int32_t frames;
};

SPerl_VM_STACK* SPerl_VM_STACK_new(SPerl* sperl);

#endif
