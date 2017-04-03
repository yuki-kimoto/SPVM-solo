#ifndef SPERL_HASH_ENTRY_H
#define SPERL_HASH_ENTRY_H

#include "sperl_base.h"

// Hash entry
struct SPerl_hash_entry {
  void* value;
  int64_t next_index;
  int64_t key_index;
};

#endif
