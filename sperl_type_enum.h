#ifndef SPERL_TYPE_ENUM
#define SPERL_TYPE_ENUM

#include "sperl_base.h"
#include "sperl_word.h"
#include "sperl_array.h"

struct SPerl_type_enum {
  SPerl_WORD* name_word;
  SPerl_ARRAY* enum_values;
};

SPerl_TYPE_ENUM* SPerl_TYPE_ENUM_new(SPerl_PARSER* parser);

#endif
