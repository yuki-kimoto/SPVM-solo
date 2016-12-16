#ifndef SPERL_PACKAGE_H
#define SPERL_PACKAGE_H

#include "sperl_base.h"

enum {
  SPerl_PACKAGE_C_CODE_LENGTH = 8
};

enum {
  SPerl_PACKAGE_C_CODE_BOOLEAN,
  SPerl_PACKAGE_C_CODE_CHAR,
  SPerl_PACKAGE_C_CODE_BYTE,
  SPerl_PACKAGE_C_CODE_SHORT,
  SPerl_PACKAGE_C_CODE_INT,
  SPerl_PACKAGE_C_CODE_LONG,
  SPerl_PACKAGE_C_CODE_FLOAT,
  SPerl_PACKAGE_C_CODE_DOUBLE
};

extern SPerl_char* const SPerl_PACKAGE_C_CODE_NAMES[];
extern SPerl_int const SPerl_PACKAGE_C_SIZES[];

struct SPerl_package {
  SPerl_OP* op_name;
  SPerl_OP* op_type;
  SPerl_int code;
  SPerl_int size;

  SPerl_ARRAY* op_descripters;
  SPerl_ARRAY* op_fields;
  SPerl_HASH* field_symtable;
  SPerl_boolean is_value;
  SPerl_ARRAY* op_subs;
  SPerl_ARRAY* op_constants;
  SPerl_CONSTANT_POOL* constant_pool;
};

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl_PARSER* parser);

#endif
