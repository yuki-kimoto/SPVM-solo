#ifndef SPERL_HASH_H
#define SPERL_HASH_H

#include "sperl_base.h"

// Hash table
struct SPerl_hash {
  SPerl_HASH_ENTRY** entries;
  int32_t count;
  int32_t capacity;
};

// Hash function
SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new();
SPerl_HASH* SPerl_HASH_new(int32_t capacity);
void SPerl_HASH_rehash(SPerl_HASH* hash_ptr, int32_t new_capacity);
void* SPerl_HASH_insert_norehash(SPerl_HASH* hash, const char* key, int32_t length, void* value);
void* SPerl_HASH_insert(SPerl_HASH* hash, const char* key, int32_t length, void* value);
void* SPerl_HASH_search(SPerl_HASH* hash, const char* key, int32_t length);
void SPerl_HASH_free(SPerl_HASH* hash);

#endif
