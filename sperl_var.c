#include "sperl_var.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_VAR* SPerl_VAR_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VAR));
}
