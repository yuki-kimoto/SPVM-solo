#ifndef SPERL_PACKAGE_H
#define SPERL_PACKAGE_H

#include "sperl_base.h"

extern SPerl_char* const SPerl_PACKAGE_C_CODE_NAMES[];

struct SPerl_package {
  SPerl_char code;
  SPerl_WORD* name_word;
  SPerl_TYPE* type;
};

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl_PARSER* parser);

#endif
