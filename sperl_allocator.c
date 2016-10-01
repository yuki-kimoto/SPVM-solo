#include <stdlib.h>

#include "sperl_allocator.h"
#include "sperl_array.h"

SPerl_ALLOCATOR* SPerl_ALLOCATOR_new(SPerl_int block_size) {
  SPerl_ALLOCATOR* allocator = (SPerl_ALLOCATOR*)calloc(1, sizeof(SPerl_ALLOCATOR));
  allocator->block_size = block_size;
  allocator->start_length = 64;
  allocator->memory_nodes = SPerl_ARRAY_new(0);
  SPerl_char* byte_data_ptr = (SPerl_char*)calloc(allocator->start_length, block_size);
  SPerl_ARRAY_push(allocator->memory_nodes, byte_data_ptr);
}

void* SPerl_ALLOCATOR_alloc(SPerl_ALLOCATOR* allocator) {
  
  SPerl_int next_memory_row = allocator->next_memory_row;
  SPerl_int next_memory_col = allocator->next_memory_col;
  SPerl_int start_length = allocator->start_length;
  SPerl_int block_size = allocator->block_size;
  SPerl_ARRAY* memory_nodes = allocator->memory_nodes;
  
  SPerl_char* memory_node = (SPerl_char*)SPerl_ARRAY_fetch(memory_nodes, next_memory_row);
  SPerl_int capacity_col = start_length * (next_memory_row + 1);
  
  // Create next memroy node
  if (next_memory_col >= capacity_col) {
    next_memory_row++;
    next_memory_col = 0;
    
    SPerl_char* new_memory_node = (SPerl_char*)calloc(start_length * (next_memory_row + 1), block_size);
    SPerl_ARRAY_push(memory_nodes, new_memory_node);
    memory_node = new_memory_node;
    allocator->next_memory_row = next_memory_row;
  }
  
  void* ptr = memory_node + (next_memory_col * block_size);
  
  next_memory_col++;
  allocator->next_memory_col = next_memory_col;
  
  return ptr;
}

void SPerl_ALLOCATOR_free(SPerl_ALLOCATOR* allocator) {
  SPerl_ARRAY* memory_nodes = allocator->memory_nodes;
  SPerl_int i;
  for (i = 0; i < memory_nodes->length; i++) {
    SPerl_char* memory_node = (SPerl_char*)SPerl_ARRAY_fetch(memory_nodes, i);
    free(memory_node);
  }
  free(memory_nodes);
  free(allocator);
}
