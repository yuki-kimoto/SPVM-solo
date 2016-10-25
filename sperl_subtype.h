#ifndef SPERL_SUBTYPE_H
#define SPERL_SUBTYPE_H

#include "sperl_core_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"

struct SPerl_subtype {
  SPerl_ARRAY* argument_types;
  SPerl_TYPE* return_type;
};

SPerl_SUBTYPE* SPerl_SUBTYPE_new(SPerl_PARSER* parser);

#endif
