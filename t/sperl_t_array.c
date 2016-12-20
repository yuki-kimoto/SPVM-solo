#include <stdio.h>

#include "../sperl_array.h"

#define OK(condition) \
  if (condition) {\
    printf("OK\n");\
  }\
  else {\
    printf("Not OK at line %d\n", __LINE__);\
  }

int main(int argc, char *argv[])
{
  // Array - new and free
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
    
    SPerl_ARRAY_free(array);
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
    int32_t value1 = 10;
    SPerl_ARRAY_push(array, &value1);
    OK(*(int32_t*)array->values[0] == 10);
    OK(array->length == 1);
    
    // push long value next
    int32_t value2 = 15;
    SPerl_ARRAY_push(array, &value2);
    OK(*(int32_t*)array->values[1] == 15);
    OK(array->length == 2);
    
    // push pointer value
    uint8_t* value3 = "foo";
    SPerl_ARRAY_push(array, value3);
    OK(array->values[2] == value3);
  }

  // Array - pop
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    
    // push long value at first
    int32_t value1 = 10;
    SPerl_ARRAY_push(array, &value1);
    OK(*(int32_t*)array->values[0] == 10);
    OK(array->length == 1);
    
    // push long value next
    int32_t value2 = 15;
    SPerl_ARRAY_push(array, &value2);
    OK(*(int32_t*)array->values[1] == 15);
    OK(array->length == 2);
    
    // pop
    void* pop_value = SPerl_ARRAY_pop(array);
    OK(*(int32_t*)pop_value == 15);
    OK(array->length == 1);
  }
  
  // Array - push capacity resize
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(2);
    
    int32_t value1 = 1;
    int32_t value2 = 2;
    SPerl_ARRAY_push(array, &value1);
    SPerl_ARRAY_push(array, &value2);
    OK(array->capacity == 2);
    
    int32_t value3 = 3;
    SPerl_ARRAY_push(array, &value3);
    OK(array->capacity == 4);
    OK(array->length == 3);
    OK(*(int32_t*)array->values[0] == 1);
    OK(*(int32_t*)array->values[1] == 2);
    OK(*(int32_t*)array->values[2] == 3);
  }

  // Array - fetch
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    int32_t value1 = 3;
    int32_t value2 = 5;
    
    SPerl_ARRAY_push(array, &value1);
    SPerl_ARRAY_push(array, &value2);
    
    OK(*(int32_t*)SPerl_ARRAY_fetch(array, 0) == 3);
    OK(*(int32_t*)SPerl_ARRAY_fetch(array, 1) == 5);
    OK(SPerl_ARRAY_fetch(array, 2) == NULL);
  }

  // Array - push many values
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(1);
    
    int32_t value1 = 1;
    int32_t value2 = 2;
    int32_t value3 = 3;
    int32_t value4 = 4;
    int32_t value5 = 5;
    int32_t value6 = 6;
    int32_t value7 = 7;
    int32_t value8 = 8;
    int32_t value9 = 9;
    int32_t value10 = 10;
    int32_t value11 = 11;
    int32_t value12 = 12;
    int32_t value13 = 13;
    int32_t value14 = 14;
    
    SPerl_ARRAY_push(array, &value1);
    SPerl_ARRAY_push(array, &value2);
    SPerl_ARRAY_push(array, &value3);
    SPerl_ARRAY_push(array, &value4);
    SPerl_ARRAY_push(array, &value5);
    SPerl_ARRAY_push(array, &value6);
    SPerl_ARRAY_push(array, &value7);
    SPerl_ARRAY_push(array, &value8);
    SPerl_ARRAY_push(array, &value9);
    SPerl_ARRAY_push(array, &value10);
    SPerl_ARRAY_push(array, &value11);
    SPerl_ARRAY_push(array, &value12);
    SPerl_ARRAY_push(array, &value13);
    SPerl_ARRAY_push(array, &value14);
  }
  
  return 1;
}
