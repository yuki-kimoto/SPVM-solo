#include <stdio.h>

#include "../sperl_hash.h"

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

  // Hash - first same key
  {
    SPerl_HASH* hash = SPerl_HASH_new(101);
    SPerl_int value1 = 3;
    SPerl_HASH_insert_norehash(hash, "key1", 4, &value1);

    SPerl_int value1_ret = *(SPerl_int*)SPerl_HASH_search(hash, "key1", 4);
    OK(value1_ret == 3);

    SPerl_int value2 = 5;
    SPerl_HASH_insert_norehash(hash, "key", 3, &value2);
    
    SPerl_int value2_ret = *(SPerl_int*)SPerl_HASH_search(hash, "key", 3);
    OK(value2_ret == 5);
  }
  
  // Hash - insert and search
  {
    SPerl_HASH* hash = SPerl_HASH_new(101);
    SPerl_int value1 = 3;
    SPerl_HASH_insert_norehash(hash, "key1", 4, &value1);
    SPerl_int hash_value1 = SPerl_hash_func("key1", 4);
    SPerl_int index1 = hash_value1 % 101;
    
    OK(*(SPerl_int*)hash->entries[index1]->value == 3);
    OK(hash->count == 1);
    SPerl_int i;

    SPerl_int value2 = 5;
    SPerl_HASH_insert_norehash(hash, "key2", 4, &value2);
    SPerl_int hash_value2 = SPerl_hash_func("key2", 4);
    SPerl_int index2 = hash_value2 % 101;
    
    OK(*(SPerl_int*)hash->entries[index2]->value == 5);
    OK(hash->count == 2);
    
    // Replace
    SPerl_int value3 = 7;
    SPerl_int return_value3 = *(SPerl_int*)SPerl_HASH_insert_norehash(hash, "key1", 4, &value3);
    SPerl_int hash_value3 = SPerl_hash_func("key1", 4);
    SPerl_int index3 = hash_value3 % 101;
    
    OK(*(SPerl_int*)hash->entries[index3]->value == 7);
    OK(return_value3 == 3);
    OK(hash->count == 2);
    
    // Search
    SPerl_int value1_1 = *(SPerl_int*)SPerl_HASH_search(hash, "key1", 4);
    OK(value1_1 == 7);
    SPerl_int value2_1 = *(SPerl_int*)SPerl_HASH_search(hash, "key2", 4);
    OK(value2_1 == 5);
    void* value3_1 = SPerl_HASH_search(hash, "none", 4);
    OK(!value3_1);
  }
  
  // Hash - insert link
  {
    SPerl_HASH* hash = SPerl_HASH_new(1);
    SPerl_int value1 = 3;
    SPerl_HASH_insert_norehash(hash, "key1", 4, &value1);
    
    OK(*(SPerl_int*)hash->entries[0]->value == 3);
    OK(hash->count == 1);
    SPerl_int i;

    SPerl_int value2 = 5;
    SPerl_HASH_insert_norehash(hash, "key2", 4, &value2);
    
    OK(*(SPerl_int*)hash->entries[0]->next->value == 5);
    OK(hash->count == 1);
    
    SPerl_int value3 = 7;
    SPerl_HASH_insert_norehash(hash, "key3", 4, &value3);
    
    OK(*(SPerl_int*)hash->entries[0]->next->next->value == 7);
    OK(hash->count == 1);
    
    // Replace key2
    SPerl_int value4 = 11;
    SPerl_int return_value4 = *(SPerl_int*)SPerl_HASH_insert_norehash(hash, "key2", 4, &value4);
    
    OK(*(SPerl_int*)hash->entries[0]->next->value == 11);
    OK(return_value4 == 5);
    OK(hash->count == 1);
    
    // Replace key3
    SPerl_int value5 = 13;
    SPerl_int return_value5 = *(SPerl_int*)SPerl_HASH_insert_norehash(hash, "key3", 4, &value5);
    
    OK(*(SPerl_int*)hash->entries[0]->next->next->value == 13);
    OK(return_value5 == 7);
    OK(hash->count == 1);

    // Search
    SPerl_int value1_1 = *(SPerl_int*)SPerl_HASH_search(hash, "key1", 4);
    OK(value1_1 == 3);
    SPerl_int value2_1 = *(SPerl_int*)SPerl_HASH_search(hash, "key2", 4);
    OK(value2_1 == 11);
    SPerl_int value3_1 = *(SPerl_int*)SPerl_HASH_search(hash, "key3", 4);
    OK(value3_1 == 13);
  }
  
  return 1;
}
