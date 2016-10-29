#ifndef SPERL_BODY_ENUM
#define SPERL_BODY_ENUM

#include "sperl_base.h"

struct SPerl_body_enum {
  SPerl_ARRAY* enum_values;
};

SPerl_BODY_ENUM* SPerl_BODY_ENUM_new(SPerl_PARSER* parser);

#endif
