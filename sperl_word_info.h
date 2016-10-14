#ifndef SPERL_WORD_INFO_H
#define SPERL_WORD_INFO_H

#include "sperl_type.h"

// Field information
struct SPerl_word_info {
  SPerl_char* value;
  SPerl_OP* op;
};

SPerl_WORD_INFO* SPerl_WORD_INFO_new(SPerl_PARSER* parser);

#endif
