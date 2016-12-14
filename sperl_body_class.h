#ifndef SPERL_BODY_CLASS_H
#define SPERL_BODY_CLASS_H

#include <stdlib.h>
#include <string.h>

#include "sperl_base.h"

enum {
  SPerl_BODY_CLASS_C_CODE_NORMAL,
  SPerl_BODY_CLASS_C_CODE_VALUE
};

// Class information
struct SPerl_body_class {
  SPerl_int code;
  SPerl_ARRAY* op_descripters;
  SPerl_ARRAY* op_fields;
  SPerl_HASH* field_symtable;
  SPerl_OP* op_block;
  SPerl_boolean is_value_class;
};

SPerl_BODY_CLASS* SPerl_BODY_CLASS_new(SPerl_PARSER* parser);

#endif
