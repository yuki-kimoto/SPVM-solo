#ifndef SPERL_NAME_INFO_H
#define SPERL_NAME_INFO_H

#include "sperl_base.h"

enum {
  SPerl_NAME_INFO_C_CODE_VARBASENAME,
  SPerl_NAME_INFO_C_CODE_BASENAME,
  SPerl_NAME_INFO_C_CODE_ABSNAME,
  SPerl_NAME_INFO_C_CODE_ANON,
};

struct SPerl_name_info {
  uint8_t code;
  SPerl_OP* op_var;
  SPerl_OP* op_name;
  const char* resolved_name;
  int32_t constant_pool_address;
};

SPerl_NAME_INFO* SPerl_NAME_INFO_new(SPerl* sperl);

#endif
