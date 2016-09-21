#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include "sperl_type.h"

// Value
struct SPerl_value;
typedef struct SPerl_value SPerl_VALUE;
struct SPerl_value {
  union {
    SPerl_char cv;
    SPerl_byte bv;
    SPerl_short sv;
    SPerl_int iv;
    SPerl_long lv;
    SPerl_float fv;
    SPerl_double dv;
    void* pv;
  } uv;
};

// Value function
SPerl_VALUE* SPerl_VALUE_new();

// Array
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;
struct SPerl_array {
  SPerl_long length;
  SPerl_long capacity;
  SPerl_long block_size;
  SPerl_char* start;
  SPerl_VALUE** values;
};

// Array function
SPerl_ARRAY* SPerl_ARRAY_new(SPerl_long capacity);
void SPerl_ARRAY_push(SPerl_ARRAY* array, SPerl_VALUE* value);
SPerl_VALUE* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_long index);

SPerl_ARRAY* SPerl_ARRAY_new_(SPerl_long block_size, SPerl_long capacity);
void SPerl_ARRAY_push_(SPerl_ARRAY* array, SPerl_char* value);
SPerl_char* SPerl_ARRAY_fetch_(SPerl_ARRAY* array, SPerl_long index);

// Hash entry
struct SPerl_hash_entry;
typedef struct SPerl_hash_entry SPerl_HASH_ENTRY;
struct SPerl_hash_entry {
  SPerl_char* key;
  SPerl_VALUE* value;
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
SPerl_HASH* SPerl_HASH_new(SPerl_long capacity);
SPerl_long SPerl_hash_func(SPerl_char* str, SPerl_long len);
SPerl_boolean SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, SPerl_VALUE* value);
SPerl_VALUE* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_long length);

#endif
