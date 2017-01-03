#include "sperl_type_component_array.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_TYPE_COMPONENT_ARRAY* SPerl_TYPE_COMPONENT_ARRAY_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_TYPE_COMPONENT_ARRAY));
}
