#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include "sperl_type.h"

// Array
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;
struct SPerl_array {
  SPerl_long length;
  SPerl_long capacity;
  void** values;
};

// Array function
SPerl_ARRAY* SPerl_ARRAY_new(SPerl_long capacity);
void SPerl_ARRAY_push(SPerl_ARRAY* array, void* value);
void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_long index);

// Hash entry
struct SPerl_hash_entry;
typedef struct SPerl_hash_entry SPerl_HASH_ENTRY;
struct SPerl_hash_entry {
  SPerl_char* key;
  void* value;
  SPerl_HASH_ENTRY* next;
};

// Hash table
struct SPerl_hash;
typedef struct SPerl_hash SPerl_HASH;
struct SPerl_hash {
  SPerl_long count;
  SPerl_long capacity;
  SPerl_HASH_ENTRY** entries;
};

// Hash function
SPerl_HASH_ENTRY* SPerl_HASH_ENTRY_new();
SPerl_HASH* SPerl_HASH_new(SPerl_long capacity);
SPerl_long SPerl_hash_func(SPerl_char* str, SPerl_long len);
void* SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, void* value);
void* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_long length);

#endif
