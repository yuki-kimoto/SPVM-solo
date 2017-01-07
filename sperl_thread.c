#include <stdlib.h>

#include "sperl_thread.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_bytecode_array.h"

SPerl_THREAD* SPerl_THREAD_new(SPerl* sperl) {
  SPerl_THREAD* thread = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_THREAD));  
  
  return thread;
}
