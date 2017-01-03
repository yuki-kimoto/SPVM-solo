#include "sperl_enumeration_value.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_ENUMERATION_VALUE* SPerl_ENUMERATION_VALUE_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_ENUMERATION_VALUE));
}
