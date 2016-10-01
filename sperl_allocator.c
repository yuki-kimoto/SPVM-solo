#include <stdlib.h>
#include "sperl_allocator.h"

SPerl_ALLOCATOR* SPerl_ALLOCATOR_new() {
  return (SPerl_ALLOCATOR*)calloc(1, sizeof(SPerl_ALLOCATOR));
}

