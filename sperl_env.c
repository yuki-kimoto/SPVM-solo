#include <stdint.h>

#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_env.h"
#include "sperl_vm.h"

SPerl_ENV* SPerl_ENV_new(SPerl* sperl) {
  SPerl_ENV* env = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_ENV));
  
  return env;
}

int64_t SPerl_ENV_get_array_length(SPerl_ENV* env, intptr_t array) {
  
  return *(int64_t*)array;
}

int8_t* SPerl_ENV_get_byte_array_data(SPerl_ENV* env, intptr_t array) {
  return array +  SPerl_VM_C_ARRAY_HEADER_LENGTH;
}
