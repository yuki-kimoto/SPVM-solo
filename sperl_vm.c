#include "sperl_vm.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_VM* SPerl_VM_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_VM));
}

void SPerl_VM_run(SPerl_PARSER* parser) {
  uint8_t* entry_point = parser->entry_point;
  
  SPerl_SUB* sub_entry_point = SPerl_HASH_search(parser->sub_abs_name_symtable, entry_point, strlen(entry_point));
  
}
