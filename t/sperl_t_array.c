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
