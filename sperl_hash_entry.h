#ifndef SPERL_HASH_ENTRY_H
#define SPERL_HASH_ENTRY_H

#include "sperl_base.h"

// Hash entry
struct SPerl_hash_entry {
  uint8_t* key;
  void* value;
  SPerl_HASH_ENTRY* next;
};

#endif
