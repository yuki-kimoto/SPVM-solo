#ifndef SPERL_FREELIST_H
#define SPERL_FREELIST_H

#include <stdio.h>

#include "sperl_base.h"

struct SPerl_freelist {
  void** addresses;
  int32_t length;
  int32_t capacity;  
};

SPerl_FREELIST* SPerl_FREELIST_new(SPerl* sperl);

#endif
