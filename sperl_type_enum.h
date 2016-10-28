#ifndef SPERL_TYPE_ENUM
#define SPERL_TYPE_ENUM

#include "sperl_base.h"

struct SPerl_type_enum {
  SPerl_ARRAY* enum_values;
};

SPerl_TYPE_ENUM* SPerl_TYPE_ENUM_new(SPerl_PARSER* parser);

#endif
