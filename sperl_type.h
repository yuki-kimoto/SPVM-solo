#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include "sperl_core_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_word.h"
#include "sperl_subtype.h"
#include "sperl_class.h"

enum {
  SPerl_TYPE_C_CODE_CLASS_OR_TYPEDEF,
  SPerl_TYPE_C_CODE_CLASS,
  SPerl_TYPE_C_CODE_TYPEDEF,
  SPerl_TYPE_C_CODE_ARRAY,
  SPerl_TYPE_C_CODE_SUB
};

struct SPerl_type {
  SPerl_char code;
  union {
    SPerl_WORD* name_word;
    SPerl_CLASS* class;
    SPerl_TYPE* type;
    SPerl_SUBTYPE* subtype;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);

#endif
