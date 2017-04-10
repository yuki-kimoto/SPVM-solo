#ifndef SPERL_HASH_ENTRY_H
#define SPERL_HASH_ENTRY_H

#include "sperl_base.h"

// Hash entry
struct SPerl_hash_entry {
  void* value;
  size_t next_index;
  size_t key_index;
};

#endif
