#ifndef SPERL_TYPE_BODY_ENUM
#define SPERL_TYPE_BODY_ENUM

#include "sperl_base.h"

struct SPerl_type_body_enum {
  SPerl_ARRAY* enum_values;
};

SPerl_TYPE_BODY_ENUM* SPerl_TYPE_BODY_ENUM_new(SPerl_PARSER* parser);

#endif
