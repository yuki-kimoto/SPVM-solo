#include <stdio.h>

#include "../sperl_collection.h"

int main(int argc, char *argv[])
{
  // Value
  {
    SPerl_VALUE* value = SPerl_VALUE_new();
    if (value->uv.long_value == 0) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }
  
  // Array - new
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(10);
    
    // capacity
    if (array->capacity == 10) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    
    // length default is 0
    if (array->length == 0) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    
    }
    
    // First element is NULL
    if (array->values[0] == NULL) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    
    // Last element is NULL
    if (array->values[9] == NULL) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }
    
  // Array - capacity default
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    
    if (array->capacity == 128) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    
    if (array->length == 0) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }

  // Array - push
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    
    SPerl_VALUE* value = SPerl_VALUE_new();
    value->uv.long_value = 10;
    
    SPerl_ARRAY_push(array, value);
    
    if (array->values[0]->uv.long_value == 10) {
      printf("OK\n");
    }
    else {
      printf("Not OK\n");
    }
  }
  return 1;
}
