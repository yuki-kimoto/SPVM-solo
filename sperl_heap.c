#include <stdlib.h>

#include "sperl.h"
#include "sperl_heap.h"
#include "sperl_allocator.h"

void* SPerl_HEAP_alloc(SPerl* sperl, int32_t size) {
  (void) sperl;
  if (size < 0 || SIZE_MAX < (uint32_t) size) {
    SPerl_ALLOCATOR_exit_with_malloc_failure();
  }
  return SPerl_ALLOCATOR_safe_malloc(size, sizeof(uint8_t));
}
