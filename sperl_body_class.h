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
  SPerl_char* name;
  SPerl_ARRAY* descripters;
  SPerl_ARRAY* fields;
  SPerl_HASH* field_symtable;
  SPerl_ARRAY* subs;
  SPerl_ARRAY* uses;
  SPerl_HASH* use_symtable;
  SPerl_OP* op_block;
  SPerl_HASH* alias;
  SPerl_OP* op;
};

SPerl_BODY_CLASS* SPerl_BODY_CLASS_new(SPerl_PARSER* parser);

#endif
