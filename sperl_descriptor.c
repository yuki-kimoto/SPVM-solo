#include "sperl_descriptor.h"
#include "sperl.h"
#include "sperl_allocator.h"

const char* const SPerl_DESCRIPTOR_CODE_NAMES[] = {
  "native",
  "const",
};

SPerl_DESCRIPTOR* SPerl_DESCRIPTOR_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_DESCRIPTOR));
}
