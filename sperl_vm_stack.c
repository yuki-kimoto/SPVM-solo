#include "sperl_vm_stack.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_VM_STACK* SPerl_VM_STACK_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_VM_STACK));
}
