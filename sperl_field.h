#ifndef SPERL_FIELD_H
#define SPERL_FIELD_H

#include "sperl_core_type.h"
#include "sperl_type.h"

// Field information
struct SPerl_field {
  SPerl_TYPE* type;
  SPerl_WORD* name_word;
  SPerl_ARRAY* descripters;
  SPerl_OP* op;
  SPerl_CLASS* class;
};

SPerl_FIELD* SPerl_FIELD_new(SPerl_PARSER* parser);

#endif
