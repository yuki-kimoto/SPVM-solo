#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include "sperl_type.h"

struct SPerl_value;
typedef struct SPerl_value SPerl_VALUE;
struct SPerl_value {
  SPerl_char type;
  union {
    SPerl_char char_value;
    SPerl_byte byte_value;
    SPerl_short short_value;
    SPerl_int int_value;
    SPerl_long long_value;
    SPerl_float float_value;
    SPerl_double double_value;
    void* ptr_value;
  } uv;
};

// Array
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;
struct SPerl_array {
  SPerl_long block_size;
  SPerl_long length;
  SPerl_long capacity;
  SPerl_VALUE* values;
};

SPerl_ARRAY* SPerl_ARRAY_new(SPerl_long length);
void SPerl_ARRAY_push(SPerl_ARRAY* array, SPerl_VALUE* value);
SPerl_VALUE* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_long index);

/*

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
  SPerl_HASH_ENTRY* entries;
};

SPerl_HASH* SPerl_new_hash();
SPerl_long SPerl_hash_func(SPerl_char* str, SPerl_long len);
SPerl_boolean SPerl_HASH_insert(SPerl_HASH* hash, SPerl_char* key, SPerl_long length, SPerl_VALUE* value) {
SPerl_HASH* SPerl_HASH_search(SPerl_HASH* hash, SPerl_char* key, SPerl_long length);
SPerl_HASH* SPerl_HASH_rehash(SPerl_HASH* hash);
*/

#endif
