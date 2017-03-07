#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_memory_pool.h"
#include "sperl_allocator.h"
#include "sperl_parser.h"

static void exit_with_malloc_failure(SPerl* sperl) {
  (void)sperl;
  fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
  exit(EXIT_FAILURE);
  /*NOTREACHED*/
}

// This function exports "exit_with_malloc_failure".
void SPerl_ALLOCATOR_exit_with_malloc_failure(SPerl* sperl) {
  exit_with_malloc_failure(sperl);
}

void* SPerl_ALLOCATOR_safe_malloc(SPerl* sperl, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    exit_with_malloc_failure(sperl);
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    exit_with_malloc_failure(sperl);
  }

  void* const block = malloc(allocation_size);
  if (!block) {
    exit_with_malloc_failure(sperl);
  }

  return block;
}

void* SPerl_ALLOCATOR_safe_malloc_zero(SPerl* sperl, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    exit_with_malloc_failure(sperl);
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    exit_with_malloc_failure(sperl);
  }

  void* const block = calloc(count, size);
  if (!block) {
    exit_with_malloc_failure(sperl);
  }

  return block;
}

void* SPerl_ALLOCATOR_safe_realloc(SPerl* sperl, void* ptr, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    exit_with_malloc_failure(sperl);
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    exit_with_malloc_failure(sperl);
  }

  void* const block = realloc(ptr, allocation_size);
  if (!block) {
    free(ptr);
    exit_with_malloc_failure(sperl);
  }

  return block;
}

void* SPerl_ALLOCATOR_alloc_memory_pool(SPerl* sperl, size_t size) {
  return SPerl_MEMORY_POOL_alloc(sperl, sperl->parser->memory_pool, size);
}

SPerl_ARRAY* SPerl_ALLOCATOR_new_array(SPerl* sperl, size_t capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, sperl->parser->arrays, array);
  
  return array;
}

SPerl_HASH* SPerl_ALLOCATOR_new_hash(SPerl* sperl, size_t capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, sperl->parser->hashes, hash);
  
  return hash;
}

int32_t* SPerl_ALLOCATOR_new_int(SPerl* sperl) {
  int32_t* value = SPerl_MEMORY_POOL_alloc(sperl, sperl->parser->memory_pool, sizeof(int32_t));
  
  return value;
}

char* SPerl_ALLOCATOR_new_string(SPerl* sperl, size_t length) {
  char* str;
  
  if (length < 0xFF) {
    str = (char*) SPerl_MEMORY_POOL_alloc(sperl, sperl->parser->memory_pool, length + 1);
  }
  else {
    str = (char*) SPerl_ALLOCATOR_safe_malloc(sperl, length + 1, sizeof(char));
    SPerl_ARRAY_push(sperl, sperl->parser->long_strings, str);
  }
  
  return str;
}
