#ifndef SPERL_SUBTYPE_H
#define SPERL_SUBTYPE_H

#include "sperl_base.h"

struct SPerl_type_component_sub {
  SPerl_ARRAY* argument_types;
  SPerl_TYPE* return_type;
};

SPerl_TYPE_COMPONENT_SUB* SPerl_TYPE_COMPONENT_SUB_new(SPerl_PARSER* parser);

#endif
