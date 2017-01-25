#ifndef SPERL_VM_H
#define SPERL_VM_H

#include "sperl_base.h"

// Field information
struct SPerl_vm {
  int64_t* call_stack;
  int32_t call_stack_capacity;
  SPerl_FRAME* frame;
};

SPerl_VM* SPerl_VM_new(SPerl* sperl);
void SPerl_VM_call_sub(SPerl* sperl, SPerl_VM* vm, const char* sub_base_name);
SPerl_FRAME* SPerl_VM_init_frame(SPerl* sperl, SPerl_VM* vm);

#endif
