#include <string.h>
#include <stdlib.h>

#include "sperl_hash.h"
#include "sperl_hash_entry.h"
#include "sperl_hash_func.h"

SPerl_HASH* SPerl_HASH_new(SPerl_int capacity) {
  
  if (capacity == 0) {
    capacity = 101;
  }
  
  SPerl_HASH* hash = (SPerl_HASH*)malloc(sizeof(SPerl_HASH) * capacity);
  memset(hash, 0, sizeof(SPerl_HASH));
  
  hash->count = 0;
  hash->capacity = capacity;
  
  SPerl_int byte_size = sizeof(SPerl_HASH_ENTRY*) * hash->capacity;
  SPerl_HASH_ENTRY** entries = (SPerl_HASH_ENTRY**)malloc(byte_size);
  memset(entries, 0, byte_size);
  hash->entries = entries;
  
  return hash;
}

SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new(SPerl_uchar* key, SPerl_int length, void* value) {
  
  SPerl_HASH_ENTRY* new_entry = malloc(sizeof(SPerl_HASH_ENTRY));
  memset(new_entry, 0, sizeof(SPerl_HASH_ENTRY));
  
  new_entry->key = (SPerl_uchar*)malloc(sizeof(SPerl_uchar) * (length + 1));
  strncpy(new_entry->key, key, length);
  new_entry->key[length] = '\0';
  new_entry->value = value;
  
  return new_entry;
}

void* SPerl_HASH_insert_norehash(SPerl_HASH* hash, SPerl_uchar* key, SPerl_int length, void* value) {

  SPerl_long hash_value = SPerl_HASH_FUNC_calc_hash(key, length);
  SPerl_int index = hash_value % hash->capacity;
  
  SPerl_HASH_ENTRY** next_entry_ptr = hash->entries + index;
  SPerl_HASH_ENTRY* next_entry = hash->entries[index];

  SPerl_int countup = 0;
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
      SPerl_HASH_ENTRY* new_entry = SPerl_HASH_ENTRY_new(key, length, value);
      *next_entry_ptr = new_entry;
      if (countup) {
        hash->count++;
      }
      return NULL;
    }
  }
}

void SPerl_HASH_rehash(SPerl_HASH* hash, SPerl_int new_capacity) {

  SPerl_HASH_ENTRY** entries = hash->entries;
  
  // Create new hash
  SPerl_HASH* new_hash = SPerl_HASH_new(new_capacity);
  
  // Rehash
  SPerl_int i;
  for (i = 0; i < hash->count; i++) {
    SPerl_HASH_ENTRY* entry = entries[i];
    
    SPerl_uchar* key = entry->key;
    if (key) {
      SPerl_int length = strlen(key);
      void* value = SPerl_HASH_search(hash, key, length);
      SPerl_HASH_insert_norehash(new_hash, key, length, value);
    }
    
    SPerl_HASH_ENTRY* next_entry = entry->next;
    while (next_entry) {
      SPerl_uchar* key = next_entry->key;
      if (key) {
        SPerl_int length = strlen(key);
        void* value = SPerl_HASH_search(hash, key, length);
        SPerl_HASH_insert_norehash(new_hash, key, length, value);
      }
      
      next_entry = next_entry->next;
    }
  }
  
  hash->count = new_hash->count;
  hash->capacity = new_hash->capacity;
  free(hash->entries);
  hash->entries = new_hash->entries;
}

void* SPerl_HASH_insert(SPerl_HASH* hash, SPerl_uchar* key, SPerl_int length, void* value) {
  if (hash == NULL) {
    return 0;
  }
  
  // Rehash
  if (hash->count > hash->capacity * 0.75) {
    SPerl_int new_capacity = (hash->capacity * 2) + 1;

    SPerl_HASH_rehash(hash, new_capacity);
  }
  
  SPerl_HASH_insert_norehash(hash, key, length, value);
}

void* SPerl_HASH_search(SPerl_HASH* hash, SPerl_uchar* key, SPerl_int length) {
  if (!hash) {
    return 0;
  }
  SPerl_long hash_value = SPerl_HASH_FUNC_calc_hash(key, length);
  SPerl_int index = hash_value % hash->capacity;
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

void SPerl_HASH_free(SPerl_HASH* hash) {
  SPerl_int capacity = hash->capacity;
  
  SPerl_int i;
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
