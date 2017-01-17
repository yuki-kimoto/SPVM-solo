#ifndef SPERL_DESCRIPTOR_H
#define SPERL_DESCRIPTOR_H

#include "sperl_base.h"

enum {
  SPerl_DESCRIPTOR_C_CODE_NATIVE,
  SPerl_DESCRIPTOR_C_CODE_CONST,
};

extern const char* const SPerl_DESCRIPTOR_CODE_NAMES[];

// Field information
struct SPerl_descriptor {
  int32_t code;
};

SPerl_DESCRIPTOR* SPerl_DESCRIPTOR_new(SPerl* sperl);

#endif
