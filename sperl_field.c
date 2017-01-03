#include "sperl_field.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_FIELD* SPerl_FIELD_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_FIELD));
}
