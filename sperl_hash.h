#ifndef SPERL_HASH_H
#define SPERL_HASH_H

#include "sperl_base.h"

// Hash table
struct SPerl_hash {
  SPerl_HASH_ENTRY** table;
  SPerl_HASH_ENTRY* entries;
  int32_t table_capacity;
  int32_t entries_capacity;
  int32_t entries_length;
  int32_t key_buffer_capacity;
  int32_t key_buffer_length;
  char* key_buffer;
};

// Hash function
SPerl_HASH* SPerl_HASH_new(SPerl* sperl, int32_t capacity);
void SPerl_HASH_free(SPerl* sperl, SPerl_HASH* hash);
void SPerl_HASH_maybe_extend_entries(SPerl* sperl, SPerl_HASH* hash);

int32_t SPerl_HASH_new_hash_entry(SPerl* sperl, SPerl_HASH* hash, const char* key, void* value);
void SPerl_HASH_rehash(SPerl* sperl, SPerl_HASH* hash, int32_t new_table_capacity);
void SPerl_HASH_insert_norehash(SPerl* sperl, SPerl_HASH* hash, const char* key, int32_t length, void* value);
void SPerl_HASH_insert(SPerl* sperl, SPerl_HASH* hash, const char* key, int32_t length, void* value);
void* SPerl_HASH_search(SPerl* sperl, SPerl_HASH* hash, const char* key, int32_t length);

#endif
