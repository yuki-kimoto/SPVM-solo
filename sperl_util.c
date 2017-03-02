#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_util.h"

static void exit_with_malloc_failure(SPerl* sperl) {
  fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
  exit(EXIT_FAILURE);
  /*NOTREACHED*/
}

// This function exports "exit_with_malloc_failure".
void SPerl_UTIL_exit_with_malloc_failure(SPerl* sperl) {
  exit_with_malloc_failure(sperl);
}

void* SPerl_UTIL_safe_malloc(SPerl* sperl, size_t count, size_t size) {
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

void* SPerl_UTIL_safe_malloc_zero(SPerl* sperl, size_t count, size_t size) {
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

void* SPerl_UTIL_safe_realloc(SPerl* sperl, void* ptr, size_t count, size_t size) {
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