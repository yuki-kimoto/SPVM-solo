#ifndef SPERL_SUB_H
#define SPERL_SUB_H

#include "sperl_base.h"

// Method information
struct SPerl_sub {
  SPerl_WORD* name_word;
  SPerl_TYPE* return_type;
  SPerl_ARRAY* descripters;
  SPerl_int argument_count;
  SPerl_ARRAY* my_vars;
  SPerl_OP* op_block;
  SPerl_OP* op;
  SPerl_BODY_CLASS* body_class;
  SPerl_boolean anon;
  SPerl_int id;
  SPerl_char* package_name;
  SPerl_ARRAY* const_values;
};

SPerl_SUB* SPerl_SUB_new(SPerl_PARSER* parser);

#endif
