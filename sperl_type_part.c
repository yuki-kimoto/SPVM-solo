#include "sperl_type_part.h"
#include "sperl.h"
#include "sperl_allocator.h"

const char* const SPerl_TYPE_PART_C_CODE_NAMES[] = {
  "sub",
  "name",
  "char"
};

SPerl_TYPE_PART* SPerl_TYPE_PART_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_TYPE_PART));
}
