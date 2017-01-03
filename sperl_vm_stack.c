#include "sperl_vm_stack.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_VM_STACK* SPerl_VM_STACK_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VM_STACK));
}
