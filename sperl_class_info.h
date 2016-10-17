#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"

enum SPerl_class_info_type {
  NORMAL,
  VALUE,
  INTERFACE,
  ENUM
};

// Class information
struct SPerl_class_info {
  SPerl_char type;
  SPerl_WORD_INFO* name;
  SPerl_ARRAY* descripters;
  SPerl_ARRAY* field_infos;
  SPerl_HASH* field_info_symtable;
  SPerl_ARRAY* method_infos;
  SPerl_HASH* method_info_symtable;
  SPerl_OP* op_block;
  SPerl_HASH* alias;
  SPerl_OP* op;
};

SPerl_CLASS_INFO* SPerl_CLASS_INFO_new(SPerl_PARSER* parser);

#endif
