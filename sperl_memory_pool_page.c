#include <stdlib.h>

#include "sperl.h"
#include "sperl_memory_pool_page.h"
#include "sperl_allocator.h"

SPerl_MEMORY_POOL_PAGE* SPerl_MEMORY_POOL_PAGE_new(SPerl* sperl) {
  return (SPerl_MEMORY_POOL_PAGE*) SPerl_ALLOCATOR_safe_malloc_zero(sperl, 1, sizeof(SPerl_MEMORY_POOL_PAGE));
}

void SPerl_MEMORY_POOL_PAGE_free(SPerl* sperl, SPerl_MEMORY_POOL_PAGE* page) {
  if (page->data) {
    free(page->data);
  }
  free(page);
}
