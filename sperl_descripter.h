#ifndef SPERL_DESCRIPTER_H
#define SPERL_DESCRIPTER_H

#include "sperl_core_type.h"

enum {
  SPerl_DESCRIPTER_C_TYPE_CONST,
  SPerl_DESCRIPTER_C_TYPE_STATIC,
  SPerl_DESCRIPTER_C_TYPE_VALUE,
  SPerl_DESCRIPTER_C_TYPE_ENUM
};

extern SPerl_char* const SPerl_DESCRIPTER_type_names[];

// Field information
struct SPerl_descripter {
  SPerl_char type;
  SPerl_OP* op;
};

SPerl_DESCRIPTER* SPerl_DESCRIPTER_new(SPerl_PARSER* parser);

#endif
