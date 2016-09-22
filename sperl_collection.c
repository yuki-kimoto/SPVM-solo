#include "sperl_type.h"
#include <string.h>
#include <stdlib.h>

#include "sperl_collection.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl_long capacity) {
  
  SPerl_ARRAY* array = (SPerl_ARRAY*)malloc(sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  SPerl_long total_byte = sizeof(void*) * array->capacity;
  void** values = (void**)malloc(total_byte);
  memset(values, 0, total_byte);
  
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl_ARRAY* array, void* value) {
  SPerl_long length = array->length;
  SPerl_long capacity = array->capacity;
  
  if (length >= capacity) {
    SPerl_long new_capacity = capacity * 2;
    array->values = realloc(array->values, new_capacity);
    memset(array->values + capacity, 0, new_capacity - capacity);
    array->capacity = new_capacity;
  }
  
  array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_long index) {
  if (array == NULL || index < 0 || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}

SPerl_long SPerl_hash_func(SPerl_char* str, SPerl_long len) {
  SPerl_char* str_tmp = str;
  SPerl_long hash = 5381;
  while (len--) {
    hash = ((hash << 5) + hash) + *str_tmp++;
  }
  return hash;
}

SPerl_HASH* SPerl_HASH_new(SPerl_long capacity) {
  
  if (capacity == 0) {
    capacity = 101;
  }

  SPerl_HASH* hash = (SPerl_HASH*)malloc(sizeof(SPerl_HASH) * capacity);
  memset(hash, 0, sizeof(SPerl_HASH));

  hash->count = 0;
  hash->capacity = capacity;
  
  SPerl_long byte_size = sizeof(SPerl_HASH_ENTRY*) * hash->capacity;
  SPerl_HASH_ENTRY** entries = (SPerl_HASH_ENTRY**)malloc(byte_size);
  memset(entries, 0, byte_size);
  hash->entries = entries;
  
  return hash;
}

SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new(SPerl_char* key, SPerl_long length, void* value) {
  
  SPerl_HASH_ENTRY* new_entry = (SPerl_HASH_ENTRY*)malloc(sizeof(SPerl_HASH_ENTRY));
  memset(new_entry, 0, sizeof(SPerl_HASH_ENTRY));
  
  new_entry->key = (SPerl_char*)malloc(sizeof(SPerl_char) * (length + 1));
  strncpy(new_entry->key, key, length);
  new_entry->key[length] = '\0';
  new_entry->value = value;
  
  return new_entry;
}

void* SPerl_HASH_insert_norehash(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, void* value) {

  SPerl_long hash_value = SPerl_hash_func(key, length);
  SPerl_long index = hash_value % hash->capacity;
  
  SPerl_HASH_ENTRY** next_entry_ptr = hash->entries + index;
  SPerl_HASH_ENTRY* next_entry = hash->entries[index];

  SPerl_long countup = 0;
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

void SPerl_HASH_rehash(SPerl_HASH* hash, SPerl_long new_capacity) {

  SPerl_HASH_ENTRY** entries = hash->entries;
  
  // Create new hash
  SPerl_HASH* new_hash = SPerl_HASH_new(new_capacity);
  
  // Rehash
  SPerl_long i;
  for (i = 0; i < hash->count; i++) {
    SPerl_HASH_ENTRY* entry = entries[i];
    
    SPerl_char* key = entry->key;
    if (key) {
      SPerl_long length = strlen(key);
      void* value = SPerl_HASH_search(hash, key, length);
      SPerl_HASH_insert_norehash(new_hash, key, length, value);
    }
    
    SPerl_HASH_ENTRY* next_entry = entry->next;
    while (next_entry) {
      SPerl_char* key = next_entry->key;
      if (key) {
        SPerl_long length = strlen(key);
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

void* SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, void* value) {
  if (hash == NULL) {
    return 0;
  }
  
  // Rehash
  if (hash->count > hash->capacity * 0.75) {
    SPerl_long new_capacity = (hash->capacity * 2) + 1;

    SPerl_HASH_rehash(hash, new_capacity);
  }
  
  SPerl_HASH_insert_norehash(hash, key, length, value);
}

void* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_long length) {
  if (hash == NULL) {
    return 0;
  }
  SPerl_long hash_value = SPerl_hash_func(key, length);
  SPerl_long index = hash_value % hash->capacity;
  
  SPerl_HASH_ENTRY* entry = hash->entries[index];
  if (entry->key) {
    if (strncmp(key, entry->key, length) == 0) {
      return entry->value;
    }
    else {
      SPerl_HASH_ENTRY* next_entry = entry->next;
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
  }
  else {
    return NULL;
  }
}
