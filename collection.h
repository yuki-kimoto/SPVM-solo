#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include <stdint.h>

struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;
struct SPerl_array {
  int64_t block_size;
  int64_t length;
  int64_t capacity;
  uintptr_t* elements;
};

SPerl_ARRAY* SPerl_new_array(int64_t length);

#endif
