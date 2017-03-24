#ifndef SPERL_HASH_ENTRY_H
#define SPERL_HASH_ENTRY_H

#include "sperl_base.h"

// Hash entry
struct SPerl_hash_entry {
  const char* key;
  SPerl_VALUE_T value;
  int64_t next_index;
};

#endif
