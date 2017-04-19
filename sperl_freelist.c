#include <stdlib.h>
#include <string.h>

#include "sperl.h"
#include "sperl_freelist.h"
#include "sperl_allocator_util.h"

SPerl_FREELIST* SPerl_FREELIST_new(SPerl* sperl) {
  SPerl_FREELIST* freelist = SPerl_ALLOCATOR_UTIL_alloc(1, sizeof(SPerl_FREELIST));
  
  return freelist;
}
