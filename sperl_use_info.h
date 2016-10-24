#ifndef SPERL_USE_INFO_H
#define SPERL_USE_INFO_H

#include "sperl_core_type.h"

// Use information
struct SPerl_use_info {
  SPerl_WORD_INFO* class_name_word_info;
  SPerl_WORD_INFO* alias_name_word_info;
  SPerl_OP* op;
};

SPerl_USE_INFO* SPerl_USE_INFO_new(SPerl_PARSER* parser);

#endif
