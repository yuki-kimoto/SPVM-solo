#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl_hash.h"
#include "sperl_hash_entry.h"
#include "sperl_hash_func.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"
#include "sperl.h"

SPerl_HASH* SPerl_HASH_new(SPerl* sperl, int64_t table_capacity) {
  (void)sperl;
  
  // Default table capacity
  if (table_capacity == 0) {
    table_capacity = 101;
  }
  
  // Create hash
  SPerl_HASH* hash = SPerl_ALLOCATOR_UTIL_safe_malloc(table_capacity, sizeof(SPerl_HASH));
  
  // Initialize hash fields
  hash->table_capacity = table_capacity;
  hash->table = SPerl_ALLOCATOR_UTIL_safe_malloc_zero(hash->table_capacity, sizeof(SPerl_HASH_ENTRY*));
  hash->entries_capacity = 255;
  hash->entries = SPerl_ALLOCATOR_UTIL_safe_malloc(hash->entries_capacity, sizeof(SPerl_HASH_ENTRY));
  hash->entries_length = 0;
  
  return hash;
}

int64_t SPerl_HASH_new_hash_entry(SPerl* sperl, SPerl_HASH* hash, const char* key, void* value) {
  
  int64_t index = hash->entries_length;
  
  SPerl_HASH_maybe_extend_entries(sperl, hash);
  
  SPerl_HASH_ENTRY* hash_entry = &hash->entries[index];
  
  hash_entry->key = key;
  hash_entry->value = value;
  hash_entry->next = NULL;
  hash_entry->next_index = -1;
  
  hash->entries_length++;
  
  return index;
}

void SPerl_HASH_maybe_extend_entries(SPerl* sperl, SPerl_HASH* hash) {
  int64_t entries_length = hash->entries_length;
  
  assert(entries_length >= 0);
  
  int64_t entries_capacity = hash->entries_capacity;
  
  if (entries_length >= entries_capacity) {
    if (entries_capacity > SIZE_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    int64_t new_entries_capacity = entries_capacity * 2;
    hash->entries = SPerl_ALLOCATOR_UTIL_safe_realloc(hash->entries, new_entries_capacity, sizeof(SPerl_HASH_ENTRY));
    hash->entries_capacity = new_entries_capacity;
  }
}

void SPerl_HASH_insert_norehash(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, void* value) {
  
  assert(hash);
  assert(key);
  assert(length >= 0);
  
  int64_t hash_value = SPerl_HASH_FUNC_calc_hash(sperl, key, length);
  int64_t index = hash_value % hash->table_capacity;
  
  SPerl_HASH_ENTRY* next_entry = hash->table[index];

  while (1) {
    if (next_entry) {
      if (strncmp(next_entry->key, key, length) == 0) {
        next_entry->value = value;
        break;
      }
      else {
        next_entry = next_entry->next;
      }
    }
    else {
      int64_t new_entry_index = SPerl_HASH_new_hash_entry(sperl, hash, key, value);
      hash->table[index] = &hash->entries[new_entry_index];
      break;
    }
  }
}

void SPerl_HASH_rehash(SPerl* sperl, SPerl_HASH* hash, int64_t new_table_capacity) {

  SPerl_HASH_ENTRY** table = hash->table;
  
  // Create new hash
  SPerl_HASH* new_hash = SPerl_HASH_new(sperl, new_table_capacity);
  
  // Rehash
  int64_t i;
  for (i = 0; i < hash->entries_length; i++) {
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
  
  // Replace hash fields
  free(hash->table);
  free(hash->entries);
  hash->entries_length = new_hash->entries_length;
  hash->table_capacity = new_hash->table_capacity;
  hash->entries_capacity = new_hash->entries_capacity;
  hash->table = new_hash->table;
  hash->entries = new_hash->entries;
}

void SPerl_HASH_insert(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, void* value) {
  if (hash == NULL) {
    return 0;
  }
  
  // Rehash
  if (hash->entries_length > hash->table_capacity * 0.75) {
    int64_t new_table_capacity = (hash->table_capacity * 2) + 1;

    SPerl_HASH_rehash(sperl, hash, new_table_capacity);
  }
  
  SPerl_HASH_insert_norehash(sperl, hash, key, length, value);
}

void* SPerl_HASH_search(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length) {
  (void)sperl;
  
  assert(hash);
  assert(key);
  assert(length >= 0);

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
  free(hash->table);
  free(hash->entries);
  free(hash);
}
