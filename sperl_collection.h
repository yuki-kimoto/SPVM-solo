#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include "sperl_type.h"

// Declaration of array
#define SPerl_DECL_ARRAY(TYPE) \
struct SPerl_array_##TYPE; \
typedef struct SPerl_array_##TYPE SPerl_ARRAY_##TYPE; \
struct SPerl_array_##TYPE {\
  SPerl_long block_size;\
  SPerl_long length;\
  SPerl_long capacity;\
  TYPE* elements;\
}; \
\
SPerl_ARRAY_##TYPE* SPerl_new_array_##TYPE(SPerl_long length);\
void SPerl_array_push_##TYPE(SPerl_ARRAY_##TYPE* array, TYPE* element);\

// Implementation of array

/*
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
*/

#endif
