#ifndef SPERL_SV_H
#define SPERL_SV_H

#include "sperl_base.h"

// Field information
struct SPerl_sv {
  char* buffer;
  int64_t ref_count;
  int64_t length;
};

#endif
