#ifndef SPERL_HASH_H
#define SPERL_HASH_H

#include "sperl_base.h"

// Hash table
struct SPerl_hash {
  SPerl_int count;
  SPerl_int capacity;
  SPerl_HASH_ENTRY** entries;
};

// Hash function
SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new();
SPerl_HASH* SPerl_HASH_new(SPerl_int capacity);
SPerl_int SPerl_hash_func(SPerl_char* str, SPerl_int len);
void SPerl_HASH_rehash(SPerl_HASH* hash_ptr, SPerl_int new_capacity);
void* SPerl_HASH_insert_norehash(SPerl_HASH* hash, SPerl_char* key, SPerl_int length, void* value);
void* SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_int length, void* value);
void* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_int length);
void SPerl_HASH_free(SPerl_HASH* hash);

#endif
