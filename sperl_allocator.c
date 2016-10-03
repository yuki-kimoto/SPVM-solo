#include <stdlib.h>
#include <math.h>

#include "sperl_allocator.h"
#include "sperl_array.h"
#include "sperl_memory_node.h"

SPerl_ALLOCATOR* SPerl_ALLOCATOR_new(SPerl_int block_size, SPerl_int start_length) {
  SPerl_ALLOCATOR* allocator = (SPerl_ALLOCATOR*)calloc(1, sizeof(SPerl_ALLOCATOR));
  allocator->block_size = block_size;
  if (start_length == 0) {
    allocator->start_length = 64;
  }
  else {
    allocator->start_length = start_length;
  }
  
  SPerl_MEMORY_NODE* memory_node = (SPerl_MEMORY_NODE*)SPerl_MEMORY_NODE_new();
  memory_node->data = calloc(block_size * start_length, sizeof(SPerl_char));
  allocator->memory_node = memory_node;
  allocator->node_depth = 1;
  
  return allocator;
}

void* SPerl_ALLOCATOR_alloc(SPerl_ALLOCATOR* allocator) {
  
  SPerl_int node_depth = allocator->node_depth;
  SPerl_int next_pos = allocator->next_pos;
  SPerl_int start_length = allocator->start_length;
  SPerl_int block_size = allocator->block_size;
  
  // Calculate capacity
  SPerl_int capacity_pos = start_length * pow(2, node_depth - 1);

  // Create next memroy node
  SPerl_char* data_ptr;
  SPerl_char* memory_node;
  if (next_pos >= capacity_pos) {
    node_depth++;
    next_pos = 0;
    
    SPerl_int new_capacity_pos = capacity_pos * 2;
    SPerl_MEMORY_NODE* new_memory_node = (SPerl_MEMORY_NODE*)SPerl_MEMORY_NODE_new();
    new_memory_node->data = (SPerl_char*)malloc(new_capacity_pos * block_size);
    memset(new_memory_node->data, 0, new_capacity_pos * block_size);
    
    new_memory_node->next = allocator->memory_node;
    allocator->memory_node = new_memory_node;
    allocator->node_depth = node_depth;
  }

  data_ptr = allocator->memory_node->data + (block_size * next_pos);
  
  next_pos++;
  allocator->next_pos = next_pos;
  
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
