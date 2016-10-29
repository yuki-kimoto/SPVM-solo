#ifndef SPERL_CORE_DATA
#define SPERL_CORE_DATA

#include "sperl_base.h"

enum {
  SPerl_CORE_DATA_C_CODE_BOOLEAN,
  SPerl_CORE_DATA_C_CODE_CHAR,
  SPerl_CORE_DATA_C_CODE_BYTE,
  SPerl_CORE_DATA_C_CODE_SHORT,
  SPerl_CORE_DATA_C_CODE_INT,
  SPerl_CORE_DATA_C_CODE_LONG,
  SPerl_CORE_DATA_C_CODE_FLOAT,
  SPerl_CORE_DATA_C_CODE_DOUBLE
};

extern SPerl_char* const SPerl_CORE_DATA_C_CODE_NAMES[];

struct SPerl_core_data {
  SPerl_char code;
  SPerl_int size;
};

SPerl_CORE_DATA* SPerl_CORE_DATA_new(SPerl_PARSER* parser);

#endif
