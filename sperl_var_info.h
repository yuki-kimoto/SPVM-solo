#ifndef SPERL_VAR_INFO_H
#define SPERL_VAR_INFO_H

#include "sperl_type.h"
#include "sperl_allocator.h"
#include "sperl_parser.h"

struct SPerl_var_info {
  SPerl_char* name;
  SPerl_MY_VAR_INFO* my_var_info;
  SPerl_int version;
};

SPerl_VAR_INFO* SPerl_VAR_INFO_new(SPerl_PARSER* parser);

#endif
