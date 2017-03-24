#include <stdlib.h>

#include "sperl.h"
#include "sperl_heap.h"
#include "sperl_allocator_util.h"

void* SPerl_HEAP_alloc(SPerl* sperl, int64_t size) {
  (void) sperl;
  if (size < 0 || INT64_MAX < (int64_t) size) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }
  return SPerl_ALLOCATOR_UTIL_safe_malloc(size, sizeof(uint8_t));
}
