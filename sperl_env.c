#include <assert.h>

#include "sperl.h"
#include "sperl_env.h"

SPerl_ENV* SPerl_ENV_new(SPerl* sperl) {
  (void)sperl;
  
  return SPerl_ALLOCATOR_UTIL_safe_malloc_i32(sperl, 1, sizeof(SPerl_ENV));
}
