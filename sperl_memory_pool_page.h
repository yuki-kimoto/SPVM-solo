#ifndef SPERL_MEMORY_POOL_PAGE
#define SPERL_MEMORY_POOL_PAGE

#include "sperl_base.h"

struct SPerl_memory_pool_page {
  uint8_t* data;
  SPerl_MEMORY_POOL_PAGE* next;
};

SPerl_MEMORY_POOL_PAGE* SPerl_MEMORY_POOL_PAGE_new();
void SPerl_MEMORY_POOL_PAGE_free (SPerl_MEMORY_POOL_PAGE* page);

#endif
