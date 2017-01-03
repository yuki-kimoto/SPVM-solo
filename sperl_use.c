#include "sperl_use.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_USE* SPerl_USE_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_USE));
}
