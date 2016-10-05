#ifndef SPERL_HASH_ENTRY_H
#define SPERL_HASH_ENTRY_H

#include "sperl_type.h"

// Hash entry
struct SPerl_hash_entry {
  SPerl_char* key;
  void* value;
  SPerl_HASH_ENTRY* next;
};

#endif
