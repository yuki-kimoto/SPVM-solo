#include "sperl_switch_info.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_SWITCH_INFO* SPerl_SWITCH_INFO_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_SWITCH_INFO));
}
