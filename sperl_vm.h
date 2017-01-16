#ifndef SPERL_VM_H
#define SPERL_VM_H

#include "sperl_base.h"

// Field information
struct SPerl_vm {
  int32_t tmp;
};

SPerl_VM* SPerl_VM_new(SPerl* sperl);
void SPerl_VM_call_sub(SPerl* sperl, SPerl_VM* vm, const char* sub_base_name);

#endif
