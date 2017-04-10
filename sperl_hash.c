#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl_hash.h"
#include "sperl_hash_entry.h"
#include "sperl_hash_func.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"
#include "sperl.h"

SPerl_HASH* SPerl_HASH_new(SPerl* sperl, size_t table_capacity) {
  (void)sperl;
  
  assert(table_capacity >= 0);

  // Create hash
  SPerl_HASH* hash = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_HASH));

  // Default table capacity
  if (table_capacity == 0) {
    hash->table_capacity = 101;
  }
  else {
    hash->table_capacity = table_capacity;
  }
  
  // Initialize table
  hash->table = SPerl_ALLOCATOR_UTIL_safe_malloc_zero(hash->table_capacity, sizeof(SPerl_HASH_ENTRY*));
  
  // Initialize entries
  hash->entries_capacity = 255;
  hash->entries = SPerl_ALLOCATOR_UTIL_safe_malloc(hash->entries_capacity, sizeof(SPerl_HASH_ENTRY));
  hash->entries_length = 0;
  
  // Initialize key buffer
  hash->key_buffer_capacity = 0xFF;
  hash->key_buffer = SPerl_ALLOCATOR_UTIL_safe_malloc(hash->key_buffer_capacity, sizeof(char));
  hash->key_buffer_length = 0;
  
  return hash;
}

void SPerl_HASH_maybe_extend_entries(SPerl* sperl, SPerl_HASH* hash) {
  (void)sperl;
  
  assert(hash);
  
  size_t entries_length = hash->entries_length;
  
  assert(entries_length >= 0);
  
  size_t entries_capacity = hash->entries_capacity;
  
  if (entries_length >= entries_capacity) {
    if (entries_capacity > INT64_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    size_t new_entries_capacity = entries_capacity * 2;
    hash->entries = SPerl_ALLOCATOR_UTIL_safe_realloc(hash->entries, new_entries_capacity, sizeof(SPerl_HASH_ENTRY));
    hash->entries_capacity = new_entries_capacity;
  }
}

void SPerl_HASH_maybe_extend_key_buffer(SPerl* sperl, SPerl_HASH* hash, size_t length) {
  (void)sperl;
  
  assert(hash);
  
  size_t key_buffer_length = hash->key_buffer_length;
  
  assert(key_buffer_length >= 0);
  
  size_t key_buffer_capacity = hash->key_buffer_capacity;
  
  if (key_buffer_length + length >= key_buffer_capacity) {
    if (key_buffer_capacity > INT64_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    size_t new_key_buffer_capacity = key_buffer_capacity * 2;
    hash->key_buffer = SPerl_ALLOCATOR_UTIL_safe_realloc(hash->key_buffer, new_key_buffer_capacity, sizeof(SPerl_HASH_ENTRY));
    hash->key_buffer_capacity = new_key_buffer_capacity;
  }
}

void SPerl_HASH_free(SPerl* sperl, SPerl_HASH* hash) {
  (void)sperl;
  
  assert(hash);
  
  free(hash->table);
  free(hash->entries);
  free(hash);
}

size_t SPerl_HASH_new_hash_entry(SPerl* sperl, SPerl_HASH* hash, const char* key, void* value) {
  (void)sperl;
  
  assert(hash);
  assert(key);
  
  size_t index = hash->entries_length;
  
  SPerl_HASH_maybe_extend_entries(sperl, hash);
  SPerl_HASH_ENTRY* hash_entry = &hash->entries[index];
  
  size_t key_length = strlen(key);
  SPerl_HASH_maybe_extend_key_buffer(sperl, hash, key_length);
  
  hash_entry->key_index = hash->key_buffer_length;
  
  strncpy(&hash->key_buffer[hash->key_buffer_length], key, key_length);
  hash->key_buffer[hash->key_buffer_length + key_length] = '\0';
  
  hash->key_buffer_length += key_length + 1;
  
  *(void**)&hash_entry->value = value;
  hash_entry->next_index = -1;
  
  hash->entries_length++;
  
  return index;
}

void SPerl_HASH_rehash(SPerl* sperl, SPerl_HASH* hash, size_t new_table_capacity) {
  (void)sperl;
  
  assert(hash);
  assert(new_table_capacity > 0);
  
  // Create new hash
  SPerl_HASH* new_hash = SPerl_HASH_new(sperl, new_table_capacity);
  
  // Rehash
  for (size_t i = 0; i < hash->entries_length; i++) {
    SPerl_HASH_ENTRY* entry = &hash->entries[i];
    
    const char* key = &hash->key_buffer[entry->key_index];
    
    assert(key);
    
    const void* value = *(void**)&entry->value;
    
    SPerl_HASH_insert_norehash(sperl, new_hash, key, strlen(key), value);
  }
  
  
  // Replace hash fields
  free(hash->table);
  free(hash->entries);
  free(hash->key_buffer);
  hash->entries_length = new_hash->entries_length;
  hash->table_capacity = new_hash->table_capacity;
  hash->entries_capacity = new_hash->entries_capacity;
  hash->table = new_hash->table;
  hash->entries = new_hash->entries;
  
  hash->key_buffer_capacity = new_hash->key_buffer_capacity;
  hash->key_buffer_length = new_hash->key_buffer_length;
  hash->key_buffer = new_hash->key_buffer;
}

void SPerl_HASH_insert_norehash(SPerl* sperl, SPerl_HASH* hash, const char* key, size_t length, void* value) {
  (void)sperl;
  
  assert(hash);
  assert(key);
  assert(length > 0);
  
  uint32_t hash_value = SPerl_HASH_FUNC_calc_hash(sperl, key, length);
  size_t index = hash_value % hash->table_capacity;
  
  if (hash->table[index]) {
    SPerl_HASH_ENTRY* next_entry = hash->table[index];
    while (1) {
      if (strncmp(key, &hash->key_buffer[next_entry->key_index], length) == 0) {
        *(void**)&next_entry->value = value;
        break;
      }
      else {
        if (next_entry->next_index != -1) {
          next_entry = &hash->entries[next_entry->next_index];
        }
        else {
          size_t new_entry_index = SPerl_HASH_new_hash_entry(sperl, hash, key, value);
          next_entry->next_index = new_entry_index;
          break;
        }
      }
    }
  }
  else {
    size_t new_entry_index = SPerl_HASH_new_hash_entry(sperl, hash, key, value);
    hash->table[index] = &hash->entries[new_entry_index];
  }
}

void SPerl_HASH_insert(SPerl* sperl, SPerl_HASH* hash, const char* key, size_t length, void* value) {
  (void)sperl;
  
  assert(hash);
  assert(key);
  assert(length > 0);
  
  // Rehash
  if (hash->entries_length > hash->table_capacity * 0.75) {
    size_t new_table_capacity = (hash->table_capacity * 2) + 1;
    
    SPerl_HASH_rehash(sperl, hash, new_table_capacity);
  }
  
  SPerl_HASH_insert_norehash(sperl, hash, key, length, value);
}

void* SPerl_HASH_search(SPerl* sperl, SPerl_HASH* hash, const char* key, size_t length) {
  (void)sperl;
  
  assert(hash);
  assert(key);
  assert(length > 0);

  uint32_t hash_value = SPerl_HASH_FUNC_calc_hash(sperl, key, length);
  size_t index = hash_value % hash->table_capacity;
  
  SPerl_HASH_ENTRY* next_entry = hash->table[index];
  while (1) {
    if (next_entry) {
      
      if (strncmp(key, &hash->key_buffer[next_entry->key_index], length) == 0) {
        return *(void**)&next_entry->value;
      }
      else {
        if (next_entry->next_index == -1) {
          next_entry = NULL;
        }
        else {
          next_entry = &hash->entries[next_entry->next_index];
        }
      }
    }
    else {
      return NULL;
    }
  }
}
