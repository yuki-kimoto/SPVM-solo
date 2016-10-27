#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include "sperl_base.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_word.h"
#include "sperl_class.h"
#include "sperl_type_sub.h"
#include "sperl_type_core.h"

enum {
  SPerl_TYPE_C_CODE_UNKNOWN,
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
    SPerl_TYPE_CORE* type_core;
    SPerl_TYPE_ENUM* type_enum;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);

#endif
