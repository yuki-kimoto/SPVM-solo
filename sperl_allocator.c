#include <stdlib.h>
#include <math.h>

#include "sperl_allocator.h"
#include "sperl_array.h"
#include "sperl_memory_node.h"

SPerl_ALLOCATOR* SPerl_ALLOCATOR_new(SPerl_int base_capacity) {
  SPerl_ALLOCATOR* allocator = (SPerl_ALLOCATOR*)calloc(1, sizeof(SPerl_ALLOCATOR));
  if (base_capacity == 0) {
    allocator->base_capacity = 4096;
  }
  else {
    SPerl_int rem = base_capacity % 4;
    if (rem != 0) {
      base_capacity = base_capacity - rem + 4;
    }
    allocator->base_capacity = base_capacity;
  }
  
  SPerl_MEMORY_NODE* memory_node = (SPerl_MEMORY_NODE*)SPerl_MEMORY_NODE_new();
  memory_node->data = calloc(allocator->base_capacity, sizeof(SPerl_char));
  allocator->memory_node = memory_node;
  allocator->node_depth = 1;
  
  return allocator;
}

void* SPerl_ALLOCATOR_alloc(SPerl_ALLOCATOR* allocator, SPerl_int block_size) {
  
  SPerl_int node_depth = allocator->node_depth;
  SPerl_int current_pos = allocator->current_pos;
  SPerl_int base_capacity = allocator->base_capacity;
  
  SPerl_int block_size_rem = block_size % 4;
  if (block_size_rem != 0) {
    block_size = block_size - block_size_rem + 4;
  }
  
  // Calculate capacity
  SPerl_int current_capacity = base_capacity * pow(2, node_depth - 1);

  // Create next memroy node
  SPerl_char* data_ptr;
  SPerl_char* memory_node;
  if (current_pos + block_size > current_capacity) {
    node_depth++;
    current_pos = 0;
    
    SPerl_int new_capacity = current_capacity * 2;
    SPerl_MEMORY_NODE* new_memory_node = (SPerl_MEMORY_NODE*)SPerl_MEMORY_NODE_new();
    new_memory_node->data = (SPerl_char*)calloc(new_capacity, sizeof(SPerl_char));
    
    new_memory_node->next = allocator->memory_node;
    allocator->memory_node = new_memory_node;
    allocator->node_depth = node_depth;
  }

  data_ptr = allocator->memory_node->data + current_pos;
  
  allocator->current_pos = current_pos + block_size;
  
  return data_ptr;
}

void SPerl_ALLOCATOR_free(SPerl_ALLOCATOR* allocator) {
  
  SPerl_MEMORY_NODE* next_node = allocator->memory_node;
  
  while (next_node) {
    SPerl_MEMORY_NODE* tmp_node = next_node->next;
    SPerl_MEMORY_NODE_free(next_node);
    next_node = tmp_node;
  }
  
  free(allocator);
}
