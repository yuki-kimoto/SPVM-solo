#include <stdio.h>

#include "../sperl_collection.h"

#define OK(condition) \
  if (condition) {\
    printf("OK\n");\
  }\
  else {\
    printf("Not OK at line %d\n", __LINE__);\
  }

int main(int argc, char *argv[])
{
  // Hash - new
  {
    SPerl_HASH* hash = SPerl_HASH_new(10);
    // capacity
    OK(hash->capacity == 10);
    OK(hash->count == 0);
  }

  // Hash - capacity default
  {
    SPerl_HASH* hash = SPerl_HASH_new(0);
    // capacity
    OK(hash->capacity == 101);
  }
  
  // Hash - insert
  {
    SPerl_HASH* hash = SPerl_HASH_new(101);
    SPerl_long value1 = 3;
    SPerl_HASH_insert_norehash(hash, "key1", 4, &value1);
    SPerl_long hash_value1 = SPerl_hash_func("key1", 4);
    SPerl_long index1 = hash_value1 % 101;
    
    OK(*(SPerl_long*)hash->entries[index1]->value == 3);
    OK(hash->count == 1);
    SPerl_long i;

    SPerl_long value2 = 5;
    SPerl_HASH_insert_norehash(hash, "key2", 4, &value2);
    SPerl_long hash_value2 = SPerl_hash_func("key2", 4);
    SPerl_long index2 = hash_value2 % 101;
    
    OK(*(SPerl_long*)hash->entries[index2]->value == 5);
    OK(hash->count == 2);
    
    // Replace
    SPerl_long value3 = 7;
    SPerl_long return_value3 = *(SPerl_long*)SPerl_HASH_insert_norehash(hash, "key1", 4, &value3);
    SPerl_long hash_value3 = SPerl_hash_func("key1", 4);
    SPerl_long index3 = hash_value3 % 101;
    
    OK(*(SPerl_long*)hash->entries[index3]->value == 7);
    OK(return_value3 == 3);
    OK(hash->count == 2);
  }
  
  // Hash - insert link
  {
    SPerl_HASH* hash = SPerl_HASH_new(1);
    SPerl_long value1 = 3;
    SPerl_HASH_insert_norehash(hash, "key1", 4, &value1);
    
    OK(*(SPerl_long*)hash->entries[0]->value == 3);
    OK(hash->count == 1);
    SPerl_long i;

    SPerl_long value2 = 5;
    SPerl_HASH_insert_norehash(hash, "key2", 4, &value2);
    
    OK(*(SPerl_long*)hash->entries[0]->next->value == 5);
    OK(hash->count == 1);
    
    SPerl_long value3 = 7;
    SPerl_long return_value3 = *(SPerl_long*)SPerl_HASH_insert_norehash(hash, "key3", 4, &value3);
    
    OK(*(SPerl_long*)hash->entries[0]->next->next->value == 7);
    OK(hash->count == 1);
  }
  
  // Array - new
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(10);
    
    // capacity
    OK(array->capacity == 10);
    
    // length default is 0
    OK(array->length == 0);
    
    // First element is NULL
    OK(array->values[0] == NULL);
    
    // Last element is NULL
    OK(array->values[9] == NULL);
  }
    
  // Array - capacity default
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    
    OK(array->capacity == 128);
    
    OK(array->length == 0);
  }

  // Array - push
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    
    // push long value at first
    SPerl_long value1 = 10;
    SPerl_ARRAY_push(array, &value1);
    OK(*(SPerl_long*)array->values[0] == 10);
    OK(array->length == 1);
    
    // push long value next
    SPerl_long value2 = 15;
    SPerl_ARRAY_push(array, &value2);
    OK(*(SPerl_long*)array->values[1] == 15);
    OK(array->length == 2);
    
    // push pointer value
    SPerl_char* value3 = "foo";
    SPerl_ARRAY_push(array, value3);
    OK(array->values[2] == value3);
  }

  // Array - push capacity resize
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(2);
    
    SPerl_long value1 = 1;
    SPerl_long value2 = 2;
    SPerl_ARRAY_push(array, &value1);
    SPerl_ARRAY_push(array, &value2);
    OK(array->capacity == 2);
    
    SPerl_long value3 = 3;
    SPerl_ARRAY_push(array, &value3);
    OK(array->capacity == 4);
    OK(array->length == 3);
    OK(*(SPerl_long*)array->values[0] == 1);
    OK(*(SPerl_long*)array->values[1] == 2);
    OK(*(SPerl_long*)array->values[2] == 3);
  }

  // Array - fetch
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    SPerl_long value1 = 3;
    SPerl_long value2 = 5;
    
    SPerl_ARRAY_push(array, &value1);
    SPerl_ARRAY_push(array, &value2);
    
    OK(*(SPerl_long*)SPerl_ARRAY_fetch(array, 0) == 3);
    OK(*(SPerl_long*)SPerl_ARRAY_fetch(array, 1) == 5);
    OK(SPerl_ARRAY_fetch(array, 2) == NULL);
  }
  
  return 1;
}
