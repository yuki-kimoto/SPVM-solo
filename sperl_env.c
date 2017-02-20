#include <stdint.h>

#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_env.h"

SPerl_ENV* SPerl_ENV_new(SPerl* sperl) {
  SPerl_ENV* env = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_ENV));
  
  env->call_stack_capacity_default = 255;
  env->call_stack_capacity = -1;
  
  if (env->call_stack_capacity == -1) {
    env->call_stack_capacity = env->call_stack_capacity_default;
  }
  env->call_stack = (int64_t*) SPerl_ALLOCATOR_safe_malloc(env->call_stack_capacity, sizeof(int64_t));
  env->vars = env->call_stack;
  env->ret = env->call_stack;
  env->abort = 0;
  
  return env;
}
