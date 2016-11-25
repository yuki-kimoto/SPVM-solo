#ifndef SPERL_TYPE_COMPONENT_WORD_H
#define SPERL_TYPE_COMPONENT_WORD_H

#include "sperl_base.h"

struct SPerl_type_component_word {
  SPerl_OP* op_name;
};

SPerl_TYPE_COMPONENT_WORD* SPerl_TYPE_COMPONENT_WORD_new(SPerl_PARSER* parser);

#endif
