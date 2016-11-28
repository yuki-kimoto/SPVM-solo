#ifndef SPERL_VMCODES_H
#define SPERL_VMCODES_H

#include "sperl_base.h"

// Array
struct SPerl_vmcodes {
  SPerl_int length;
  SPerl_int capacity;
  SPerl_VMCODE* values;
};

// Array function
SPerl_VMCODES* SPerl_VMCODES_new();
void SPerl_VMCODES_push(SPerl_VMCODES* vmcodes, SPerl_VMCODE value);

#endif
