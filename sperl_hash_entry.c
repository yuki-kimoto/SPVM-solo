#include <string.h>

#include "sperl.h"
#include "sperl_hash_entry.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new(SPerl* sperl, const char* key, int32_t length, void* value) {
  
  SPerl_HASH_ENTRY* new_entry = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_HASH_ENTRY));
  memset(new_entry, 0, sizeof(SPerl_HASH_ENTRY));
  
  if (length >= SIZE_MAX) {
    SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
  }
  new_entry->key = key;
  new_entry->value = value;
  
  return new_entry;
}

