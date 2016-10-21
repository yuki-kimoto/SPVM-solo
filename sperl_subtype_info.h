#ifndef SPERL_SUBTYPE_INFO_H
#define SPERL_SUBTYPE_INFO_H

#include "sperl_core_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"

struct SPerl_subtype_info {
  SPerl_ARRAY* argument_type_infos;
  SPerl_TYPE_INFO* return_type_info;
};

SPerl_SUBTYPE_INFO* SPerl_SUBTYPE_INFO_new(SPerl_PARSER* parser);

#endif
