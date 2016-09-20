#include "sperl_type.h"
#include <string.h>
#include <stdlib.h>

#include "sperl_collection.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl_long length) {
  
  SPerl_ARRAY* array = (SPerl_ARRAY*)malloc(sizeof(SPerl_ARRAY));
  array->length = length;
  if (length > 32) {
    array->capacity = length;
  }
  else {
    array->capacity = 32;
  }
  
  SPerl_long values_byte_size = sizeof(SPerl_VALUE*) * array->capacity;
  SPerl_VALUE** values = (SPerl_VALUE**)malloc(values_byte_size);
  memset(values, 0, values_byte_size);
  
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl_ARRAY* array, SPerl_VALUE* value) {
  SPerl_long length = array->length;
  SPerl_long capacity = array->capacity;
  
  length++;
  if (length > capacity) {
    SPerl_long capacity_old = capacity;
    capacity = capacity * 2;
    array->values = realloc(array->values, capacity);
    memset(array->values + capacity_old, 0, capacity - capacity_old);
    array->capacity = capacity;
  }
  
  array->values[length - 1] = value;
}

SPerl_VALUE* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_long index) {
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
  hash->count = 0;
  hash->capacity = capacity;
  
  SPerl_long byte_size = sizeof(SPerl_HASH_ENTRY*) * hash->capacity;
  SPerl_HASH_ENTRY** entries = (SPerl_HASH_ENTRY**)malloc(byte_size);
  memset(entries, 0, byte_size);
  hash->entries = entries;
  
  return hash;
}

SPerl_boolean SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, SPerl_VALUE* value) {
  if (hash == NULL) {
    return 0;
  }
  
  // Rehash
  if (hash->count > hash->capacity * 0.75) {
    SPerl_long capacity = hash->capacity;
    SPerl_long new_capacity = (capacity * 2) + 1;
    
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
        SPerl_VALUE* value = SPerl_HASH_search(hash, key, length);
        SPerl_HASH_insert(new_hash, key, length, value);
      }
      
      SPerl_HASH_ENTRY* next_entry = entry->next;
      while (next_entry) {
        SPerl_char* key = next_entry->key;
        if (key) {
          SPerl_long length = strlen(key);
          SPerl_VALUE* value = SPerl_HASH_search(hash, key, length);
          SPerl_HASH_insert(new_hash, key, length, value);
        }
        
        next_entry = next_entry->next;
      }
    }
    
    hash->count = new_hash->count;
    hash->capacity = new_hash->capacity;
    free(hash->entries);
    hash->entries = new_hash->entries;
  }
  
  SPerl_long hash_value = SPerl_hash_func(key, length);
  SPerl_long index = hash_value % hash->capacity;
  
  SPerl_HASH_ENTRY* entry = hash->entries[index];
  if (entry->key) {
    SPerl_HASH_ENTRY* next_entry = entry->next;
    while (1) {
      if (next_entry) {
        next_entry = next_entry->next;
      }
      else {
        SPerl_HASH_ENTRY* new_entry = (SPerl_HASH_ENTRY*)malloc(sizeof(SPerl_HASH_ENTRY));
        new_entry->key = (SPerl_char*)malloc(sizeof(SPerl_char) * (length + 1));
        strncpy(new_entry->key, key, length);
        new_entry->value = value;
        
        break;
      }
    }
  }
  else {
    entry->value = value;
    entry->key = (SPerl_char*)malloc(sizeof(SPerl_char) * (length + 1));
    strncpy(entry->key, key, length);
    entry->key[length] = '\0';
    hash->count++;
  }
  
  return 1;
}

SPerl_VALUE* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_long length) {
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
