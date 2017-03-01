#ifndef SPERL_HASH_ENTRY_H
#define SPERL_HASH_ENTRY_H

#include "sperl_base.h"

// Hash entry
struct SPerl_hash_entry {
  char* key;
  void* value;
  SPerl_HASH_ENTRY* next;
};

SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new(SPerl* sperl, const char* key, int32_t length, void* value);

#endif
