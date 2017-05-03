#ifndef SPVM_SWITCH_INFO_H
#define SPVM_SWITCH_INFO_H

#include "spvm_base.h"

enum {
  SPVM_SWITCH_INFO_C_CODE_TABLESWITCH,
  SPVM_SWITCH_INFO_C_CODE_LOOKUPSWITCH,
};

// Parser information
struct SPVM_switch_info {
  SPVM_ARRAY* op_cases;
  int64_t max;
  int64_t min;
  int32_t code;
};

SPVM_SWITCH_INFO* SPVM_SWITCH_INFO_new(SPVM* spvm);

#endif
