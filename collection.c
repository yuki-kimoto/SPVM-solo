#include <stdint.h>
#include <string.h>

#include "collection.h"

SPerl_ARRAY* new_array(int64_t block_size, int64_t length) {
  
  int64_t total_size = block_size * length;
  SPerl_ARRAY* array = (SPerl_ARRAY*)malloc(total_size);
  memset(array, 0, total_size);
  
  return array;
}
