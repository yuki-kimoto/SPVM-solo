#ifndef SPERL_TYPE_INFO_H
#define SPERL_TYPE_INFO_H

#include "sperl_core_type.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_word_info.h"
#include "sperl_subtype_info.h"

enum SPerl_type_info_type {
  SPerl_TYPE_INFO_TYPE_WORDTYPE,
  SPerl_TYPE_INFO_TYPE_SUBTYPE
};

struct SPerl_type_info {
  SPerl_char type;
  union {
    SPerl_WORD_INFO* name_word_info;
    SPerl_SUBTYPE_INFO* subtype_info;
  } uv;
};

SPerl_TYPE_INFO* SPerl_TYPE_INFO_new(SPerl_PARSER* parser);

#endif
