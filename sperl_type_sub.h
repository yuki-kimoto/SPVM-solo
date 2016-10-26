#ifndef SPERL_SUBTYPE_H
#define SPERL_SUBTYPE_H

#include "sperl_base.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"

struct SPerl_type_sub {
  SPerl_ARRAY* argument_types;
  SPerl_TYPE* return_type;
};

SPerl_TYPE_SUB* SPerl_TYPE_SUB_new(SPerl_PARSER* parser);

#endif
