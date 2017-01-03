#include "sperl_name_info.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_NAME_INFO* SPerl_NAME_INFO_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_NAME_INFO));
}
