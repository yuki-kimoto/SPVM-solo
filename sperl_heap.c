#include <stdlib.h>

#include "sperl.h"
#include "sperl_heap.h"
#include "sperl_allocator_parser.h"

void* SPerl_HEAP_alloc(SPerl* sperl, int32_t size) {
  (void) sperl;
  if (size < 0 || SIZE_MAX < (uint32_t) size) {
    SPerl_ALLOCATOR_PARSER_exit_with_malloc_failure(sperl, sperl->parser);
  }
  return SPerl_ALLOCATOR_PARSER_safe_malloc(sperl, sperl->parser, size, sizeof(uint8_t));
}
