#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "sperl_allocator_util.h"

void SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure() {
  fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
  exit(EXIT_FAILURE);
  /*NOTREACHED*/
}

void* SPerl_ALLOCATOR_UTIL_safe_malloc(size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  void* const block = malloc(allocation_size);
  if (!block) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  return block;
}

void* SPerl_ALLOCATOR_UTIL_safe_malloc_zero(size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  void* const block = calloc(count, size);
  if (!block) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  return block;
}

void* SPerl_ALLOCATOR_UTIL_safe_realloc(void* ptr, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  void* const block = realloc(ptr, allocation_size);
  if (!block) {
    free(ptr);
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }

  return block;
}
