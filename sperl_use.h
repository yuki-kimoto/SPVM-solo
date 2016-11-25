#ifndef SPERL_USE_H
#define SPERL_USE_H

#include "sperl_base.h"

// Use information
struct SPerl_use {
  SPerl_WORD* package_name_word;
};

SPerl_USE* SPerl_USE_new(SPerl_PARSER* parser);

#endif
