#ifndef SPERL_TYPE_WORD_H
#define SPERL_TYPE_WORD_H

#include "sperl_base.h"

struct SPerl_type_word {
  SPerl_TYPE* type;
};

SPerl_TYPE_WORD* SPerl_TYPE_WORD_new(SPerl_PARSER* parser);

#endif
