#ifndef SPERL_FIELD_H
#define SPERL_FIELD_H

#include "sperl_base.h"

// Field information
struct SPerl_field {
  SPerl_TYPE* type;
  SPerl_WORD* name_word;
  SPerl_ARRAY* op_descripters;
  SPerl_BODY_CLASS* body_class;
};

SPerl_FIELD* SPerl_FIELD_new(SPerl_PARSER* parser);

#endif
