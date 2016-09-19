#ifndef SPERL_COLLECTION_H
#define SPERL_COLLECTION_H

#include <stdint.h>

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

#endif
