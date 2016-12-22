#ifndef SPERL_TYPE_COMPONENT_NAME_H
#define SPERL_TYPE_COMPONENT_NAME_H

#include "sperl_base.h"

struct SPerl_type_component_name {
  SPerl_OP* op_name;
};

SPerl_TYPE_COMPONENT_NAME* SPerl_TYPE_COMPONENT_NAME_new(SPerl_PARSER* parser);

#endif
