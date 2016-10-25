#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include "sperl_core_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_word.h"
#include "sperl_subtype.h"

enum SPerl_type_type {
  SPerl_TYPE_TYPE_WORDTYPE,
  SPerl_TYPE_TYPE_SUBTYPE
};

struct SPerl_type {
  SPerl_char type;
  union {
    SPerl_WORD* name_word;
    SPerl_SUBTYPE* subtype;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);

#endif
