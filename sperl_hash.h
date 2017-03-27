#ifndef SPERL_HASH_H
#define SPERL_HASH_H

#include "sperl_base.h"

// Hash table
struct SPerl_hash {
  SPerl_HASH_ENTRY** table;
  SPerl_HASH_ENTRY* entries;
  int64_t table_capacity;
  int64_t entries_capacity;
  int64_t entries_length;
};

// Hash function
SPerl_HASH* SPerl_HASH_new(SPerl* sperl, int64_t capacity);
void SPerl_HASH_free(SPerl* sperl, SPerl_HASH* hash);
void SPerl_HASH_maybe_extend_entries(SPerl* sperl, SPerl_HASH* hash);

int64_t SPerl_HASH_new_hash_entry(SPerl* sperl, SPerl_HASH* hash, const char* key, SPerl_VALUE_T value);
void SPerl_HASH_rehash(SPerl* sperl, SPerl_HASH* hash, int64_t new_table_capacity);
void SPerl_HASH_insert_norehash(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, SPerl_VALUE_T value);
void SPerl_HASH_insert(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, SPerl_VALUE_T value);
SPerl_VALUE_T SPerl_HASH_search(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length);

#define SPerl_HASH_new_hash_entry_address(sperl, hash, key, value) SPerl_HASH_new_hash_entry(sperl, hash, key, (SPerl_VALUE_T)value);
#define SPerl_HASH_rehash_address(sperl, hash, new_capacity) SPerl_HASH_rehash(sperl, hash, new_capacity);
#define SPerl_HASH_insert_norehash_address(sperl, hash, key, length, value) SPerl_HASH_insert_norehash(sperl, hash, key, length, (SPerl_VALUE_T)value);
#define SPerl_HASH_insert_address(sperl, hash, key, length, value) SPerl_HASH_insert(sperl, hash, key, length, (SPerl_VALUE_T)value);
#define SPerl_HASH_search_address(sperl, hash, key, length) (void*)SPerl_HASH_search(sperl, hash, key, length);

#define SPerl_HASH_new_hash_entry_long(sperl, hash, key, value) SPerl_HASH_new_hash_entry(sperl, hash, key, (SPerl_VALUE_T)value);
#define SPerl_HASH_rehash_long(sperl, hash, new_capacity) SPerl_HASH_rehash(sperl, hash, new_capacity);
#define SPerl_HASH_insert_norehash_long(sperl, hash, key, length, value) SPerl_HASH_insert_norehash(sperl, hash, key, length, (SPerl_VALUE_T)value);
#define SPerl_HASH_insert_long(sperl, hash, key, length, value) SPerl_HASH_insert(sperl, hash, key, length, (SPerl_VALUE_T)value);
#define SPerl_HASH_search_long(sperl, hash, key, length) (int64_t)SPerl_HASH_search(sperl, hash, key, length);

#endif
