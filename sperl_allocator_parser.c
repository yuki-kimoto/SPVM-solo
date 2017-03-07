#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_memory_pool.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

void SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(SPerl* sperl, SPerl_PARSER* parser) {
  (void)sperl;
  fprintf(stderr, "Failed to allocate memory. Sperl will exit.\n");
  exit(EXIT_FAILURE);
  /*NOTREACHED*/
}

void* SPerl_ALLOCATOR_PARSER_safe_malloc(SPerl* sperl, SPerl_PARSER* parser, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  void* const block = malloc(allocation_size);
  if (!block) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  return block;
}

void* SPerl_ALLOCATOR_PARSER_safe_malloc_zero(SPerl* sperl, SPerl_PARSER* parser, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  void* const block = calloc(count, size);
  if (!block) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  return block;
}

void* SPerl_ALLOCATOR_PARSER_safe_realloc(SPerl* sperl, SPerl_PARSER* parser, void* ptr, size_t count, size_t size) {
  if (count > SIZE_MAX / size) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  size_t const allocation_size = count * size;
  if (allocation_size == 0) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  void* const block = realloc(ptr, allocation_size);
  if (!block) {
    free(ptr);
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, parser);
  }

  return block;
}

void* SPerl_ALLOCATOR_PARSER_alloc_memory_pool(SPerl* sperl, SPerl_PARSER* parser, size_t size) {
  return SPerl_MEMORY_POOL_alloc(sperl, parser->memory_pool, size);
}

SPerl_ARRAY* SPerl_ALLOCATOR_PARSER_new_array(SPerl* sperl, SPerl_PARSER* parser, size_t capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, parser->arrays, array);
  
  return array;
}

SPerl_HASH* SPerl_ALLOCATOR_PARSER_new_hash(SPerl* sperl, SPerl_PARSER* parser, size_t capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, parser->hashes, hash);
  
  return hash;
}

int32_t* SPerl_ALLOCATOR_PARSER_new_int(SPerl* sperl, SPerl_PARSER* parser) {
  int32_t* value = SPerl_MEMORY_POOL_alloc(sperl, parser->memory_pool, sizeof(int32_t));
  
  return value;
}

char* SPerl_ALLOCATOR_PARSER_new_string(SPerl* sperl, SPerl_PARSER* parser, size_t length) {
  char* str;
  
  if (length < 0xFF) {
    str = (char*) SPerl_MEMORY_POOL_alloc(sperl, parser->memory_pool, length + 1);
  }
  else {
    str = (char*) SPerl_ALLOCATOR_PARSER_safe_malloc(sperl, parser, length + 1, sizeof(char));
    SPerl_ARRAY_push(sperl, parser->long_strings, str);
  }
  
  return str;
}
