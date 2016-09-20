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
    capacity = capacity * 2;
    array->values = realloc(array->values, capacity);
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

/*
SPerl_long SPerL_hash_func(SPerl_char* str, SPerl_long len) {
  SPerl_char* str_tmp = str;
  SPerl_long hash = 5381;
  while (len--) {
    hash = ((hash << 5) + hash) + *str_tmp++;
  }
  return hash;
}

SPerl_HASH* SPerl_HASH_new() {
  
  SPerl_long capacity = 101;
  SPerl_HASH* hash = (SPerl_HASH*)malloc(sizeof(SPerl_HASH) * capacity);
  hash->count = 0;
  hash->capacity = capacity;
  
  return hash;
}

SPerl_HASH* SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, SPerl_VALUE* value) {
  if (hash == NULL) {
    return 0;
  }
  SPerl_long hash = SPerl_hash_func(key, length);
  SPerl_long index = hash % hash->capacity;
  
  return 1;
}

SPerl_HASH* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_long length) {
  
}

SPerl_HASH* SPerl_HASH_rehash(SPerl_HASH* hash) {
  
}
*/
