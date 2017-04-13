#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_allocator_runtime.h"
#include "sperl_allocator_util.h"

void* SPerl_ALLOCATOR_RUNTIME_alloc(SPerl* sperl, size_t size) {
  (void) sperl;

  return SPerl_ALLOCATOR_UTIL_safe_malloc_i32(size, sizeof(char));
}
