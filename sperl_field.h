#ifndef SPERL_FIELD_H
#define SPERL_FIELD_H

#include "sperl_base.h"

// Field information
struct SPerl_field {
  SPerl_OP* op_type;
  SPerl_OP* op_name;
  uint32_t id;
};

SPerl_FIELD* SPerl_FIELD_new(SPerl_PARSER* parser);

#endif
