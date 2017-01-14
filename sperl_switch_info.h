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
  int32_t code;
  int32_t max;
  int32_t min;
};

SPerl_SWITCH_INFO* SPerl_SWITCH_INFO_new(SPerl* sperl);

#endif
