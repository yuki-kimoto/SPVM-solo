#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include "sperl_core_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_word.h"
#include "sperl_subtype.h"
#include "sperl_class.h"

enum SPerl_type_type {
  C_SPerl_TYPE_ARRAY,
  C_SPerl_TYPE_CLASS,
  C_SPerl_TYPE_SUB,
  C_SPerl_TYPE_TYPEDEF,
  C_SPerl_TYPE_CLASS_OR_TYPEDEF
};

struct SPerl_type {
  SPerl_char type;
  SPerl_WORD* name_word;
  union {
    SPerl_CLASS* class;
    SPerl_SUBTYPE* subtype;
    SPerl_TYPE* type;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);

#endif
