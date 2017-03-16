#include <string.h>

#include "sperl.h"
#include "sperl_hash_entry.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new(SPerl* sperl, const char* key, void* value) {
  
  SPerl_HASH_ENTRY* new_entry = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_HASH_ENTRY));

  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = NULL;
  new_entry->next_index = -1;
  
  return new_entry;
}

