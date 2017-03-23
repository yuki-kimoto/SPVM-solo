#ifndef SPERL_SWITCH_INFO_H
#define SPERL_SWITCH_INFO_H

#include "sperl_base.h"

enum {
  SPerl_SWITCH_INFO_C_CODE_TABLESWITCH,
  SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH,
};

// Parser information
struct SPerl_switch_info {
  SPerl_ARRAY* op_cases;
  int64_t code;
  int64_t max;
  int64_t min;
};

SPerl_SWITCH_INFO* SPerl_SWITCH_INFO_new(SPerl* sperl);

#endif
