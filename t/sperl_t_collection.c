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
    
    // push long value at first
    SPerl_VALUE* value1 = SPerl_VALUE_new();
    value1->uv.long_value = 10;
    SPerl_ARRAY_push(array, value1);
    if (array->values[0]->uv.long_value == 10) {
      printf("OK\n");
    }
    else {
      printf("Not OK\n");
    }
    if (array->length == 1) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    
    // push long value next
    SPerl_VALUE* value2 = SPerl_VALUE_new();
    value2->uv.long_value = 15;
    SPerl_ARRAY_push(array, value2);
    if (array->values[1]->uv.long_value == 15) {
      printf("OK\n");
    }
    else {
      printf("Not OK\n");
    }
    if (array->length == 2) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    
    // push pointer value
    SPerl_VALUE* value3 = SPerl_VALUE_new();
    value3->uv.ptr_value = "foo";
    SPerl_ARRAY_push(array, value3);
    if (array->values[2]->uv.ptr_value == value3->uv.ptr_value) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }

  // Array - push capacity resize
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(2);
    
    SPerl_VALUE* value1 = SPerl_VALUE_new();
    value1->uv.long_value = 1;
    SPerl_VALUE* value2 = SPerl_VALUE_new();
    value2->uv.long_value = 2;
    SPerl_ARRAY_push(array, value1);
    SPerl_ARRAY_push(array, value2);
    if (array->capacity == 2) {
      printf("OK\n");
    }
    else {
      printf("Not OK\n", __LINE__);
    }
    
    SPerl_VALUE* value3 = SPerl_VALUE_new();
    value3->uv.long_value = 3;
    SPerl_ARRAY_push(array, value3);
    if (array->capacity == 4) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (array->length == 3) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (array->values[0]->uv.long_value == 1) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (array->values[1]->uv.long_value == 2) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (array->values[2]->uv.long_value == 3) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }  }
  
  return 1;
}
