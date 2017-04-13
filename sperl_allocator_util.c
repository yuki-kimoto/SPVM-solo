#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "sperl_allocator_util.h"

void* SPerl_ALLOCATOR_UTIL_safe_malloc(size_t count, size_t size) {

  if (count <= 0 || size <= 0 || count > SIZE_MAX / size) {
    fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
    exit(EXIT_FAILURE);
  }
  
  size_t allocation_size = count * size;
  
  void* block = malloc(allocation_size);
  
  if (!block) {
    fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
    exit(EXIT_FAILURE);
  }
  
  return block;
}

void* SPerl_ALLOCATOR_UTIL_safe_realloc(void* ptr, size_t count, size_t size) {
  if (count <= 0 || size <= 0 || count > SIZE_MAX / size) {
    fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
    exit(EXIT_FAILURE);
  }

  size_t allocation_size = count * size;

  void* block = realloc(ptr, allocation_size);
  
  if (!block) {
    fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
    exit(EXIT_FAILURE);
  }

  return block;
}
