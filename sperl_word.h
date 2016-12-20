#ifndef SPERL_WORD_H
#define SPERL_WORD_H

#include "sperl_base.h"

// Field information
struct SPerl_word {
  uint8_t* value;
  SPerl_OP* op;
};

SPerl_WORD* SPerl_WORD_new(SPerl_PARSER* parser);

#endif
