#include <stdlib.h>

#include "sperl.h"
#include "sperl_heap.h"

void* SPerl_HEAP_alloc(SPerl* sperl, int32_t size) {
  return malloc(size);
}
