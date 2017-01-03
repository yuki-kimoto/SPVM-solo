#include "sperl_descripter.h"
#include "sperl.h"
#include "sperl_allocator.h"

const char* const SPerl_DESCRIPTER_CODE_NAMES[] = {
  "const",
  "static",
  "value",
  "enum"
};

SPerl_DESCRIPTER* SPerl_DESCRIPTER_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_DESCRIPTER));
}
