#ifndef SPERL_TYPE_CORE
#define SPERL_TYPE_CORE

#include "sperl_base.h"
#include "sperl_word.h"

enum {
  SPERL_TYPE_CORE_C_CODE_BOOLEAN,
  SPERL_TYPE_CORE_C_CODE_CHAR,
  SPERL_TYPE_CORE_C_CODE_BYTE,
  SPERL_TYPE_CORE_C_CODE_SHORT,
  SPERL_TYPE_CORE_C_CODE_INT,
  SPERL_TYPE_CORE_C_CODE_LONG,
  SPERL_TYPE_CORE_C_CODE_FLOAT,
  SPERL_TYPE_CORE_C_CODE_DOUBLE
};

struct SPerl_type_core {
  SPerl_char code;
  SPerl_WORD* name_word;
  SPerl_int size;
};

#endif
