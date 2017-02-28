#include <stdlib.h>

#include "sperl_virtual_memory.h"
#include "sperl_allocator.h"

SPerl_VIRTUAL_MEMORY* SPerl_VIRTUAL_MEMORY_new(SPerl* sperl) {
  SPerl_VIRTUAL_MEMORY* virtual_memory = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VIRTUAL_MEMORY));
  
  virtual_memory->current_index = 0;
  virtual_memory->indexes = malloc(1024 * sizeof(int64_t));
  
  return virtual_memory;
}

SPerl_VIRTUAL_MEMORY* SPerl_VIRTUAL_MEMORY_malloc(SPerl* sperl, SPerl_VIRTUAL_MEMORY* virtual_memory, size_t byte_size) {
  
  int64_t current_index = virtual_memory->current_index;
  
  virtual_memory->indexes[current_index] = (intptr_t)malloc(byte_size);
  
  virtual_memory->current_index++;
  
  return current_index;
}
