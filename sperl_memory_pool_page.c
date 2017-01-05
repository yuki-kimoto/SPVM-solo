#include <stdlib.h>
#include "sperl_memory_pool_page.h"

SPerl_MEMORY_POOL_PAGE* SPerl_MEMORY_POOL_PAGE_new() {
  return (SPerl_MEMORY_POOL_PAGE*)calloc(1, sizeof(SPerl_MEMORY_POOL_PAGE));
}

void SPerl_MEMORY_POOL_PAGE_free (SPerl_MEMORY_POOL_PAGE* page) {
  if (page->data) {
    free(page->data);
  }
  free(page);
}
