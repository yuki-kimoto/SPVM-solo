#include <stdio.h>

#include "../sperl_collection.h"

int main(int argc, char *argv[])
{
    // Array - new
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new_(sizeof(SPerl_long), 10);
    
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
    if (*(SPerl_long*)array->start == 0) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    
    // Last element is NULL
    if (*((SPerl_long*)array->start + 9) == 0) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }

  // Array - capacity default
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new_(sizeof(SPerl_long), 0);
    
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
    SPerl_ARRAY* array = SPerl_ARRAY_new_(sizeof(SPerl_long), 0);
    
    // push long value at first
    SPerl_long value1 = 10;
    SPerl_ARRAY_push_(array, (SPerl_char*)&value1);
    if (*(SPerl_long*)array->start == 10) {
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
    SPerl_long value2 = 15;
    SPerl_ARRAY_push_(array, (SPerl_char*)&value2);
    if (*((SPerl_long*)array->start + 1) == 15) {
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
  }

  // Array - push capacity resize
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new_(sizeof(SPerl_long), 2);
    
    SPerl_long value1 = 1;
    SPerl_long value2 = 2;

    SPerl_ARRAY_push_(array, (SPerl_char*)&value1);
    SPerl_ARRAY_push_(array, (SPerl_char*)&value2);
    if (array->capacity == 2) {
      printf("OK\n");
    }
    else {
      printf("Not OK\n", __LINE__);
    }
    
    SPerl_long value3 = 3;
    SPerl_ARRAY_push_(array, (SPerl_char*)&value3);
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
    if (*((SPerl_long*)array->start) == 1) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (*((SPerl_long*)array->start + 1) == 2) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (*((SPerl_long*)array->start + 2) == 3) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }

  // Array - fetch
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new_(sizeof(SPerl_long), 0);
    SPerl_VALUE* value1 = 3;
    SPerl_VALUE* value2 = 5;
    
    SPerl_ARRAY_push_(array, (SPerl_char*)&value1);
    SPerl_ARRAY_push_(array, (SPerl_char*)&value2);
    
    if (*(SPerl_long*)SPerl_ARRAY_fetch_(array, 0) == 3) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (*(SPerl_long*)SPerl_ARRAY_fetch_(array, 1) == 5) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (SPerl_ARRAY_fetch(array, 2) == NULL) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }
    
  /*
  // Value
  {
    SPerl_VALUE* value = SPerl_VALUE_new();
    if (value->uv.lv == 0) {
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
    value1->uv.lv = 10;
    SPerl_ARRAY_push(array, value1);
    if (array->values[0]->uv.lv == 10) {
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
    value2->uv.lv = 15;
    SPerl_ARRAY_push(array, value2);
    if (array->values[1]->uv.lv == 15) {
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
    value3->uv.pv = "foo";
    SPerl_ARRAY_push(array, value3);
    if (array->values[2]->uv.pv == value3->uv.pv) {
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
    value1->uv.lv = 1;
    SPerl_VALUE* value2 = SPerl_VALUE_new();
    value2->uv.lv = 2;
    SPerl_ARRAY_push(array, value1);
    SPerl_ARRAY_push(array, value2);
    if (array->capacity == 2) {
      printf("OK\n");
    }
    else {
      printf("Not OK\n", __LINE__);
    }
    
    SPerl_VALUE* value3 = SPerl_VALUE_new();
    value3->uv.lv = 3;
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
    if (array->values[0]->uv.lv == 1) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (array->values[1]->uv.lv == 2) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (array->values[2]->uv.lv == 3) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }

  // Array - fetch
  {
    SPerl_ARRAY* array = SPerl_ARRAY_new(0);
    SPerl_VALUE* value1 = SPerl_VALUE_new();
    value1->uv.lv = 3;
    SPerl_VALUE* value2 = SPerl_VALUE_new();
    value2->uv.lv = 5;
    
    SPerl_ARRAY_push(array, value1);
    SPerl_ARRAY_push(array, value2);
    
    if (SPerl_ARRAY_fetch(array, 0)->uv.lv == 3) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (SPerl_ARRAY_fetch(array, 1)->uv.lv == 5) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
    if (SPerl_ARRAY_fetch(array, 2) == NULL) {
      printf("OK\n");
    }
    else {
      printf("Not OK %d\n", __LINE__);
    }
  }
  */
  
  return 1;
}
