#ifndef SPERL_VM_H
#define SPERL_VM_H

#include "sperl_base.h"

// Field information
struct SPerl_vm {
  int32_t tmp;
};

SPerl_VM* SPerl_VM_new(SPerl_PARSER* parser);
void SPerl_VM_run(SPerl_PARSER* parser);

#endif
