#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include "sperl_base.h"

enum {
  SPerl_TYPE_C_CODE_TYPEDEFORCLASS,
  SPerl_TYPE_C_CODE_CORE,
  SPerl_TYPE_C_CODE_CLASS,
  SPerl_TYPE_C_CODE_TYPEDEF,
  SPerl_TYPE_C_CODE_ARRAY,
  SPerl_TYPE_C_CODE_SUB,
  SPerl_TYPE_C_CODE_ENUM
};

extern SPerl_char* const SPerl_TYPE_C_CODE_NAMES[];

struct SPerl_type {
  SPerl_char code;
  SPerl_WORD* name_word;
  union {
    SPerl_TYPE* type;
    SPerl_CLASS* class;
    SPerl_TYPE_SUB* type_sub;
    SPerl_BODY_CORE* body_core;
    SPerl_BODY_ENUM* body_enum;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);

#endif
