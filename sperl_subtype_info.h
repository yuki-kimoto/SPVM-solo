#ifndef SPERL_SUBTYPE_INFO_H
#define SPERL_SUBTYPE_INFO_H

#include "sperl_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"

struct SPerl_subtype_info {
  SPerl_ARRAY* argument_types;
  SPerl_WORD_INFO* return_type;
};

SPerl_SUBTYPE_INFO* SPerl_SUBTYPE_INFO_new(SPerl_PARSER* parser);

#endif
