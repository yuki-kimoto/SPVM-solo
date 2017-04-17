#include <assert.h>

#include "sperl.h"
#include "sperl_env.h"
#include "sperl_allocator_util.h"

SPerl_ENV* SPerl_ENV_new(SPerl* sperl) {
  (void)sperl;
  
  SPerl_ENV* env = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPerl_ENV));
  
  env->call_stack_capacity = 0xFF;
  env->call_stack = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(env->call_stack_capacity, sizeof(intmax_t));
  env->call_stack_base = -1;
  env->operand_stack_top = -1;
  env->abort = 0;
  
  return env;
}

void SPerl_ENV_free(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  
  // Free call stack
  free(env->call_stack);
  
  free(env);
}
