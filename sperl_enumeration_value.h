#ifndef SPERL_ENUMERATION_VALUE_H
#define SPERL_ENUMERATION_VALUE_H

#include "sperl_base.h"

// Field information
struct SPerl_enumeration_value {
  SPerl_OP* op_name;
  SPerl_OP* op_constant;
};

SPerl_ENUMERATION_VALUE* SPerl_ENUMERATION_VALUE_new(SPerl* sperl);

#endif
