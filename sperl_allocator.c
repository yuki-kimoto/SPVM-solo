#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_memory_pool.h"
#include "sperl_allocator.h"

static void exit_with_malloc_failure() {
  fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
  exit(EXIT_FAILURE);
  /*NOTREACHED*/
}

// This function exports "exit_with_malloc_failure".
void SPerl_ALLOCATOR_exit_with_malloc_failure() {
  exit_with_malloc_failure();
}

void* SPerl_ALLOCATOR_safe_malloc(size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    exit_with_malloc_failure();
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    exit_with_malloc_failure();
  }

  void* const block = malloc(allocation_size);
  if (!block) {
    exit_with_malloc_failure();
  }

  return block;
}

void* SPerl_ALLOCATOR_safe_malloc_zero(size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    exit_with_malloc_failure();
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    exit_with_malloc_failure();
  }

  void* const block = calloc(count, size);
  if (!block) {
    exit_with_malloc_failure();
  }

  return block;
}

void* SPerl_ALLOCATOR_safe_realloc(void* ptr, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    exit_with_malloc_failure();
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    exit_with_malloc_failure();
  }

  void* const block = realloc(ptr, allocation_size);
  if (!block) {
    free(ptr);
    exit_with_malloc_failure();
  }

  return block;
}

void* SPerl_ALLOCATOR_alloc_memory_pool(SPerl* sperl, size_t size) {
  return SPerl_MEMORY_POOL_alloc(sperl, sperl->memory_pool, size);
}

SPerl_ARRAY* SPerl_ALLOCATOR_new_array(SPerl* sperl, size_t capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, sperl->array_ptrs, array);
  
  return array;
}

SPerl_HASH* SPerl_ALLOCATOR_new_hash(SPerl* sperl, size_t capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, sperl->hash_ptrs, hash);
  
  return hash;
}

int32_t* SPerl_ALLOCATOR_new_int(SPerl* sperl) {
  int32_t* value = SPerl_MEMORY_POOL_alloc(sperl, sperl->memory_pool, sizeof(int32_t));
  
  return value;
}

char* SPerl_ALLOCATOR_new_string(SPerl* sperl, size_t length) {
  char* str;
  
  if (length < 40) {
    str = (char*) SPerl_MEMORY_POOL_alloc(sperl, sperl->memory_pool, 40);
  }
  else {
    str = (char*) SPerl_ALLOCATOR_safe_malloc(length + 1, sizeof(char));
    SPerl_ARRAY_push(sperl, sperl->long_str_ptrs, str);
  }
  
  return str;
}
