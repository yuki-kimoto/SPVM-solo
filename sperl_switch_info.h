#ifndef SPERL_SWITCH_INFO_H
#define SPERL_SWITCH_INFO_H

#include "sperl_base.h"

enum {
  SPerl_SWITCH_INFO_C_CODE_TABLESWITCH,
  SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH,
};

// Parser information
struct SPerl_switch_info {
  uint32_t code;
  SPerl_ARRAY* op_cases;
  uint32_t length;
  uint32_t max;
  uint32_t min;
};

#endif
