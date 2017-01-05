#ifndef SPERL_DESCRIPTER_H
#define SPERL_DESCRIPTER_H

#include "sperl_base.h"

enum {
  SPerl_DESCRIPTER_C_CODE_CONST,
  SPerl_DESCRIPTER_C_CODE_STATIC,
  SPerl_DESCRIPTER_C_CODE_VALUE,
  SPerl_DESCRIPTER_C_CODE_ENUM
};

extern const char* const SPerl_DESCRIPTER_CODE_NAMES[];

// Field information
struct SPerl_descriptor {
  int32_t code;
};

SPerl_DESCRIPTER* SPerl_DESCRIPTER_new(SPerl* sperl);

#endif
