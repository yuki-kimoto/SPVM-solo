#ifndef SPERL_HASH_H
#define SPERL_HASH_H

#include "sperl_type.h"

// Hash entry
struct SPerl_hash_entry {
  SPerl_char* key;
  void* value;
  SPerl_HASH_ENTRY* next;
};

// Hash table
struct SPerl_hash {
  SPerl_long count;
  SPerl_long capacity;
  SPerl_HASH_ENTRY** entries;
};

// Hash function
SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new();
SPerl_HASH* SPerl_HASH_new(SPerl_long capacity);
SPerl_long SPerl_hash_func(SPerl_char* str, SPerl_long len);
void SPerl_HASH_rehash(SPerl_HASH* hash_ptr, SPerl_long new_capacity);
void* SPerl_HASH_insert_norehash(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, void* value);
void* SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, void* value);
void* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_long length);

#endif
