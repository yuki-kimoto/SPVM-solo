#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include "sperl_type.h"

// Array
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;
struct SPerl_array {
  SPerl_long block_size;
  SPerl_long length;
  SPerl_long capacity;
  void* elements;
};

SPerl_ARRAY* SPerl_new_array(SPerl_long length);
void SPerl_array_push(SPerl_ARRAY* array, void* element);

// Hash
struct SPerl_hash_entry;
typedef struct SPerl_hash_entry SPerl_HASH_ENTRY;
struct SPerl_hash_entry {
  SPerl_char* key;
  void* value;
  SPerl_HASH_ENTRY* next;
};

struct SPerl_hash;
typedef struct SPerl_hash SPerl_HASH;
struct SPerl_hash {
  SPerl_HASH_ENTRY* entries;
};

SPerl_long SPerl_hash_func(SPerl_char* str, SPerl_long len);

#endif
