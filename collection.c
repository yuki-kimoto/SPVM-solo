#include <stdint.h>

#include "collection.h"

SPerl_ARRAY* create_array(int64_t block_size, int64_t length) {
  SPerl_ARRAY* array = (SPerl_ARRAY*)malloc(block_size * length);
  
  return array;
}
