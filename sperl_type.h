#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include "sperl_base.h"

enum {
  SPerl_TYPE_C_CODE_WORD,
  SPerl_TYPE_C_CODE_ARRAY,
  SPerl_TYPE_C_CODE_SUB,
};

extern SPerl_char* const SPerl_TYPE_C_CODE_NAMES[];

struct SPerl_type {
  SPerl_char code;
  SPerl_WORD* name_word;
  union {
    SPerl_TYPE* type;
    SPerl_TYPE_SUB* type_sub;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);
SPerl_char* SPerl_TYPE_to_string(SPerl_PARSER* parser, SPerl_TYPE* type);

#endif
