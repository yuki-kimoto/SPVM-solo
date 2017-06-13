#ifndef SPVM_HASH_H
#define SPVM_HASH_H

#include "spvm_base.h"

// Hash table
struct SPVM_hash {
  SPVM_HASH_ENTRY** table;
  SPVM_HASH_ENTRY* entries;
  int32_t table_capacity;
  int32_t entries_capacity;
  int32_t entries_length;
  int32_t key_buffer_capacity;
  int32_t key_buffer_length;
  char* key_buffer;
};

// Hash function
SPVM_HASH* SPVM_HASH_new(int32_t capacity);
void SPVM_HASH_free(SPVM_HASH* hash);
void SPVM_HASH_maybe_extend_entries(SPVM_HASH* hash);

int32_t SPVM_HASH_new_hash_entry(SPVM_HASH* hash, const char* key, void* value);
void SPVM_HASH_rehash(SPVM_HASH* hash, int32_t new_table_capacity);
void SPVM_HASH_insert_norehash(SPVM_HASH* hash, const char* key, int32_t length, void* value);
void SPVM_HASH_insert(SPVM_HASH* hash, const char* key, int32_t length, void* value);
void* SPVM_HASH_search(SPVM_HASH* hash, const char* key, int32_t length);

#endif
