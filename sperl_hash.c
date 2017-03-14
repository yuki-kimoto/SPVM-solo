#include <string.h>
#include <stdlib.h>

#include "sperl_hash.h"
#include "sperl_hash_entry.h"
#include "sperl_hash_func.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"
#include "sperl.h"

SPerl_HASH* SPerl_HASH_new(SPerl* sperl, int64_t table_capacity) {
  (void)sperl;
  
  if (table_capacity == 0) {
    table_capacity = 101;
  }
  
  SPerl_HASH* hash = SPerl_ALLOCATOR_UTIL_safe_malloc(table_capacity, sizeof(SPerl_HASH));
  
  hash->table_capacity = table_capacity;
  
  SPerl_HASH_ENTRY** table = SPerl_ALLOCATOR_UTIL_safe_malloc(hash->table_capacity, sizeof(SPerl_HASH_ENTRY*));
  hash->table = table;
  
  hash->entries_capacity = 0xFF;
  hash->entries = SPerl_ALLOCATOR_UTIL_safe_malloc(hash->entries_capacity, sizeof(SPerl_HASH_ENTRY));
  
  hash->entries_count = 0;
  
  return hash;
}

void* SPerl_HASH_insert_norehash(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, void* value) {

  int64_t hash_value = SPerl_HASH_FUNC_calc_hash(sperl, key, length);
  int64_t index = hash_value % hash->table_capacity;
  
  SPerl_HASH_ENTRY** next_entry_ptr = hash->table + index;
  SPerl_HASH_ENTRY* next_entry = hash->table[index];

  _Bool countup = 0;
  if (!next_entry) {
    countup = 1;
  }
  
  while (1) {
    if (next_entry) {
      if (strncmp(next_entry->key, key, length) == 0) {
        void* old_value = next_entry->value;
        next_entry->value = value;
        return old_value;
      }
      else {
        next_entry_ptr = &next_entry->next;
        next_entry = next_entry->next;
      }
    }
    else {
      SPerl_HASH_ENTRY* new_entry = SPerl_HASH_ENTRY_new(sperl, key, length, value);
      *next_entry_ptr = new_entry;
      if (countup) {
        hash->entries_count++;
      }
      return NULL;
    }
  }
}

void SPerl_HASH_rehash(SPerl* sperl, SPerl_HASH* hash, int64_t new_table_capacity) {

  SPerl_HASH_ENTRY** table = hash->table;
  
  // Create new hash
  SPerl_HASH* new_hash = SPerl_HASH_new(sperl, new_table_capacity);
  
  // Rehash
  int64_t i;
  for (i = 0; i < hash->entries_count; i++) {
    SPerl_HASH_ENTRY* entry = table[i];
    
    const char* key = entry->key;
    if (key) {
      int64_t length = strlen(key);
      void* value = SPerl_HASH_search(sperl, hash, key, length);
      SPerl_HASH_insert_norehash(sperl, new_hash, key, length, value);
    }
    
    SPerl_HASH_ENTRY* next_entry = entry->next;
    while (next_entry) {
      const char* key = next_entry->key;
      if (key) {
        int64_t length = strlen(key);
        void* value = SPerl_HASH_search(sperl, hash, key, length);
        SPerl_HASH_insert_norehash(sperl, new_hash, key, length, value);
      }
      
      next_entry = next_entry->next;
    }
  }
  
  hash->entries_count = new_hash->entries_count;
  hash->table_capacity = new_hash->table_capacity;
  free(hash->table);
  hash->table = new_hash->table;
}

void* SPerl_HASH_insert(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, void* value) {
  if (hash == NULL) {
    return 0;
  }
  
  // Rehash
  if (hash->entries_count > hash->table_capacity * 0.75) {
    int64_t new_table_capacity = (hash->table_capacity * 2) + 1;

    SPerl_HASH_rehash(sperl, hash, new_table_capacity);
  }
  
  return SPerl_HASH_insert_norehash(sperl, hash, key, length, value);
}

void* SPerl_HASH_search(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length) {
  if (!hash) {
    return 0;
  }
  int64_t hash_value = SPerl_HASH_FUNC_calc_hash(sperl, key, length);
  int64_t index = hash_value % hash->table_capacity;
  SPerl_HASH_ENTRY* next_entry = hash->table[index];
  while (1) {
    if (next_entry) {
      if (strncmp(key, next_entry->key, length) == 0) {
        return next_entry->value;
      }
      else {
        next_entry = next_entry->next;
      }
    }
    else {
      return NULL;
    }
  }
}

void SPerl_HASH_free(SPerl* sperl, SPerl_HASH* hash) {
  int64_t table_capacity = hash->table_capacity;
  
  int64_t i;
  for (i = 0; i < table_capacity; i++) {
    SPerl_HASH_ENTRY* next_entry = hash->table[i];
    while (next_entry) {
      SPerl_HASH_ENTRY* tmp_entry = next_entry->next;
      if (next_entry->key) {
        free(next_entry->key);
      }
      free(next_entry);
      next_entry = tmp_entry;
    }
  }
  
  free(hash->table);
  free(hash);
}
