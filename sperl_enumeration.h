#ifndef SPERL_ENUMERATION
#define SPERL_ENUMERATION

#include "sperl_base.h"

struct SPerl_enumeration {
  SPerl_ARRAY* enumeration_values;
};

SPerl_ENUMERATION* SPerl_ENUMERATION_new(SPerl_PARSER* parser);

#endif
