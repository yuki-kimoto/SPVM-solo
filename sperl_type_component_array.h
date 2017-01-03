#ifndef SPERL_TYPE_COMPONENT_ARRAY_H
#define SPERL_TYPE_COMPONENT_ARRAY_H

#include "sperl_base.h"

struct SPerl_type_component_array {
  SPerl_TYPE* type;
  SPerl_OP* op_term;
};

SPerl_TYPE_COMPONENT_ARRAY* SPerl_TYPE_COMPONENT_ARRAY_new(SPerl* sperl);

#endif
