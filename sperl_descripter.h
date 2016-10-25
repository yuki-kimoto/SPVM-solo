#ifndef SPERL_DESCRIPTER_H
#define SPERL_DESCRIPTER_H

#include "sperl_core_type.h"

enum {
  SPerl_DESCRIPTER_C_CODE_CONST,
  SPerl_DESCRIPTER_C_CODE_STATIC,
  SPerl_DESCRIPTER_C_CODE_VALUE,
  SPerl_DESCRIPTER_C_CODE_ENUM
};

extern SPerl_char* const SPerl_DESCRIPTER_type_names[];

// Field information
struct SPerl_descripter {
  SPerl_char code;
  SPerl_OP* op;
};

SPerl_DESCRIPTER* SPerl_DESCRIPTER_new(SPerl_PARSER* parser);

#endif
