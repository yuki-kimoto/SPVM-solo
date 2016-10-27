#ifndef SPERL_TYPE_CORE
#define SPERL_TYPE_CORE

#include "sperl_base.h"
#include "sperl_word.h"

enum {
  SPerl_TYPE_CORE_C_CODE_BOOLEAN,
  SPerl_TYPE_CORE_C_CODE_CHAR,
  SPerl_TYPE_CORE_C_CODE_BYTE,
  SPerl_TYPE_CORE_C_CODE_SHORT,
  SPerl_TYPE_CORE_C_CODE_INT,
  SPerl_TYPE_CORE_C_CODE_LONG,
  SPerl_TYPE_CORE_C_CODE_FLOAT,
  SPerl_TYPE_CORE_C_CODE_DOUBLE
};

extern SPerl_char* const SPerl_TYPE_CORE_C_CODE_NAMES[];

struct SPerl_type_core {
  SPerl_char code;
  SPerl_int size;
};

SPerl_TYPE_CORE* SPerl_TYPE_CORE_new(SPerl_PARSER* parser);

#endif
