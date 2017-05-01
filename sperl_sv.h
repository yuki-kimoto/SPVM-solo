#ifndef SPERL_SV_H
#define SPERL_SV_H

#include "sperl_base.h"

// Field information
struct SPerl_sv {
  char* buffer;
  int64_t ref_count;
  int64_t length;
};

SPerl_SV* SPerl_SV_new_pvn(SPerl* sperl, const char* pv, size_t length);

#endif
