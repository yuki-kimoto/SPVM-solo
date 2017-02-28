#include <stdlib.h>

#include "sperl_virtual_memory.h"
#include "sperl_allocator.h"

SPerl_VIRTUAL_MEMORY* SPerl_VIRTUAL_MEMORY_new(SPerl* sperl) {
  SPerl_VIRTUAL_MEMORY* virtual_memory = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VIRTUAL_MEMORY));
  
  return virtual_memory;
}
