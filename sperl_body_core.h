#ifndef SPERL_BODY_CORE
#define SPERL_BODY_CORE

#include "sperl_base.h"

enum {
  SPerl_BODY_CORE_C_CODE_BOOLEAN,
  SPerl_BODY_CORE_C_CODE_CHAR,
  SPerl_BODY_CORE_C_CODE_BYTE,
  SPerl_BODY_CORE_C_CODE_SHORT,
  SPerl_BODY_CORE_C_CODE_INT,
  SPerl_BODY_CORE_C_CODE_LONG,
  SPerl_BODY_CORE_C_CODE_FLOAT,
  SPerl_BODY_CORE_C_CODE_DOUBLE
};

extern SPerl_char* const SPerl_BODY_CORE_C_CODE_NAMES[];
extern SPerl_int const SPerl_BODY_CORE_C_SIZES[];

struct SPerl_body_core {
  SPerl_char code;
  SPerl_int size;
};

SPerl_BODY_CORE* SPerl_BODY_CORE_new(SPerl_PARSER* parser);

#endif
