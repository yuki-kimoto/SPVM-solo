#include <string.h>
#include <stdlib.h>

#include "sperl_hash.h"
#include "sperl_hash_entry.h"
#include "sperl_hash_func.h"
#include "sperl_allocator.h"

SPerl_HASH* SPerl_HASH_new(SPerl* sperl, int32_t capacity) {
  (void)sperl;
  
  if (capacity == 0) {
    capacity = 101;
  }
  
  SPerl_HASH* hash = SPerl_ALLOCATOR_safe_malloc(sperl, capacity, sizeof(SPerl_HASH));
  memset(hash, 0, sizeof(SPerl_HASH));
  
  hash->count = 0;
  hash->capacity = capacity;
  
  SPerl_HASH_ENTRY** entries = SPerl_ALLOCATOR_safe_malloc_zero(sperl, hash->capacity, sizeof(SPerl_HASH_ENTRY*));
  hash->entries = entries;
  
  return hash;
}

void* SPerl_HASH_insert_norehash(SPerl* sperl, SPerl_HASH* hash, const char* key, int32_t length, void* value) {

  int64_t hash_value = SPerl_HASH_FUNC_calc_hash(sperl, key, length);
  int32_t index = hash_value % hash->capacity;
  
  SPerl_HASH_ENTRY** next_entry_ptr = hash->entries + index;
  SPerl_HASH_ENTRY* next_entry = hash->entries[index];

  int32_t countup = 0;
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
        hash->count++;
      }
      return NULL;
    }
  }
}

void SPerl_HASH_rehash(SPerl* sperl, SPerl_HASH* hash, int32_t new_capacity) {

  SPerl_HASH_ENTRY** entries = hash->entries;
  
  // Create new hash
  SPerl_HASH* new_hash = SPerl_HASH_new(sperl, new_capacity);
  
  // Rehash
  int32_t i;
  for (i = 0; i < hash->count; i++) {
    SPerl_HASH_ENTRY* entry = entries[i];
    
    const char* key = entry->key;
    if (key) {
      int32_t length = strlen(key);
      void* value = SPerl_HASH_search(sperl, hash, key, length);
      SPerl_HASH_insert_norehash(sperl, new_hash, key, length, value);
    }
    
    SPerl_HASH_ENTRY* next_entry = entry->next;
    while (next_entry) {
      const char* key = next_entry->key;
      if (key) {
        int32_t length = strlen(key);
        void* value = SPerl_HASH_search(sperl, hash, key, length);
        SPerl_HASH_insert_norehash(sperl, new_hash, key, length, value);
      }
      
      next_entry = next_entry->next;
    }
  }
  
  hash->count = new_hash->count;
  hash->capacity = new_hash->capacity;
  free(hash->entries);
  hash->entries = new_hash->entries;
}

void* SPerl_HASH_insert(SPerl* sperl, SPerl_HASH* hash, const char* key, int32_t length, void* value) {
  if (hash == NULL) {
    return 0;
  }
  
  // Rehash
  if (hash->count > hash->capacity * 0.75) {
    int32_t new_capacity = (hash->capacity * 2) + 1;

    SPerl_HASH_rehash(sperl, hash, new_capacity);
  }
  
  return SPerl_HASH_insert_norehash(sperl, hash, key, length, value);
}

void* SPerl_HASH_search(SPerl* sperl, SPerl_HASH* hash, const char* key, int32_t length) {
  if (!hash) {
    return 0;
  }
  int64_t hash_value = SPerl_HASH_FUNC_calc_hash(sperl, key, length);
  int32_t index = hash_value % hash->capacity;
  SPerl_HASH_ENTRY* next_entry = hash->entries[index];
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
  int32_t capacity = hash->capacity;
  
  int32_t i;
  for (i = 0; i < capacity; i++) {
    SPerl_HASH_ENTRY* next_entry = hash->entries[i];
    while (next_entry) {
      SPerl_HASH_ENTRY* tmp_entry = next_entry->next;
      if (next_entry->key) {
        free(next_entry->key);
      }
      free(next_entry);
      next_entry = tmp_entry;
    }
  }
  
  free(hash->entries);
  free(hash);
}
