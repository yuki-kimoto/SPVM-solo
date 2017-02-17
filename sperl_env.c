#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_env.h"

SPerl_ENV* SPerl_ENV_new(SPerl* sperl) {
  SPerl_ENV* env = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_ENV));
  
  return env;
}
