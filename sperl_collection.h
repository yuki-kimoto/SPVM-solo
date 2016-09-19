#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include <stdint.h>

// Array
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;
struct SPerl_array {
  int32_t block_size;
  int32_t length;
  int32_t capacity;
  uintptr_t* elements;
};

SPerl_ARRAY* SPerl_new_array(int32_t length);
void SPerl_array_push(SPerl_ARRAY* array, uintptr_t* element);

// Hash
struct SPerl_hash_entry;
typedef struct SPerl_hash_entry SPerl_HASH_ENTRY;
struct SPerl_hash_entry {
  uint8_t* key;
  void* value;
  SPerl_HASH_ENTRY* next;
};

struct SPerl_hash;
typedef struct SPerl_hash SPerl_HASH;
struct SPerl_hash {
  SPerl_HASH_ENTRY* entries;
};

int64_t SPerl_hash_func(uint8_t* str, int64_t len);

#endif
