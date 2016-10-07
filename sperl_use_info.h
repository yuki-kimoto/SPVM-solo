#ifndef SPERL_USE_INFO_H
#define SPERL_USE_INFO_H

#include "sperl_type.h"

// Use information
struct SPerl_use_info {
  SPerl_char* class_name;
  SPerl_char* alias_name;
};

SPerl_USE_INFO* SPerl_USE_INFO_new(SPerl_PARSER* parser);

#endif
