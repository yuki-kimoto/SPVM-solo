#ifndef SPERL_TYPE_ARRAY_H
#define SPERL_TYPE_ARRAY_H

#include "sperl_base.h"

struct SPerl_type_array {
  SPerl_TYPE* type;
};

SPerl_TYPE_ARRAY* SPerl_TYPE_ARRAY_new(SPerl_PARSER* parser);

#endif
