#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include <stdlib.h>
#include <string.h>

#include "sperl_core_type.h"

enum {
  SPerl_CLASS_C_TYPE_CORE,
  SPerl_CLASS_C_TYPE_NORMAL,
  SPerl_CLASS_C_TYPE_VALUE,
  SPerl_CLASS_C_TYPE_ENUM
};

// Class information
struct SPerl_class {
  SPerl_char type;
  SPerl_WORD* name;
  SPerl_ARRAY* descripters;
  SPerl_ARRAY* fields;
  SPerl_HASH* field_symtable;
  SPerl_ARRAY* methods;
  SPerl_HASH* method_symtable;
  SPerl_ARRAY* enum_values;
  SPerl_ARRAY* uses;
  SPerl_HASH* use_symtable;
  SPerl_OP* op_block;
  SPerl_HASH* alias;
  SPerl_OP* op;
};

SPerl_CLASS* SPerl_CLASS_new(SPerl_PARSER* parser);

#endif
