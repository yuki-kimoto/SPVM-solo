#ifndef SPERL_VAR_H
#define SPERL_VAR_H

#include "sperl_base.h"

struct SPerl_var {
  SPerl_WORD* name_word;
  SPerl_MY_VAR* my_var;
  SPerl_int version;
  SPerl_OP* op;
};

SPerl_VAR* SPerl_VAR_new(SPerl_PARSER* parser);

#endif
