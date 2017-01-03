#include "sperl_enumeration.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_ENUMERATION* SPerl_ENUMERATION_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_ENUMERATION));
}
