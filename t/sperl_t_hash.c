#include <stdio.h>

#include "../sperl.h"
#include "../sperl_hash.h"
#include "../sperl_hash_entry.h"
#include "../sperl_hash_func.h"

#define OK(condition) \
  if (condition) {\
    printf("OK\n");\
  }\
  else {\
    printf("Not OK at line %d\n", __LINE__);\
  }

int main()
{
  SPerl* sperl = SPerl_new();
  
  // Hash - new
  {
    SPerl_HASH* hash = SPerl_HASH_new(sperl, 10);
    // capacity
    OK(hash->table_capacity == 10);
    OK(hash->entries_length == 0);
  }

  // Hash - capacity default
  {
    SPerl_HASH* hash = SPerl_HASH_new(sperl, 0);
    // capacity
    OK(hash->table_capacity == 101);
  }

  // Hash - first same key
  {
    SPerl_HASH* hash = SPerl_HASH_new(sperl, 101);
    int64_t value1 = 3;
    SPerl_HASH_insert_norehash(sperl, hash, "key1", 4, &value1);

    int64_t value1_ret = *(int64_t*)SPerl_HASH_search(sperl, hash, "key1", 4);
    OK(value1_ret == 3);

    int64_t value2 = 5;
    SPerl_HASH_insert_norehash(sperl, hash, "key", 3, &value2);
    
    int64_t value2_ret = *(int64_t*)SPerl_HASH_search(sperl, hash, "key", 3);
    OK(value2_ret == 5);
  }
  
  // Hash - insert and search
  {
    SPerl_HASH* hash = SPerl_HASH_new(sperl, 101);
    int64_t value1 = 3;
    SPerl_HASH_insert_norehash(sperl, hash, "key1", 4, &value1);
    int64_t hash_value1 = SPerl_HASH_FUNC_calc_hash(sperl, "key1", 4);
    int64_t index1 = hash_value1 % 101;
   
    OK(*(int64_t*)hash->table[index1]->value == 3);
    OK(hash->entries_length == 1);

    int64_t value2 = 5;
    SPerl_HASH_insert_norehash(sperl, hash, "key2", 4, &value2);
    int64_t hash_value2 = SPerl_HASH_FUNC_calc_hash(sperl, "key2", 4);
    int64_t index2 = hash_value2 % 101;
    
    OK(*(int64_t*)hash->table[index2]->value == 5);
    OK(hash->entries_length == 2);
    
    // Replace
    int64_t value3 = 7;
    SPerl_HASH_insert_norehash(sperl, hash, "key1", 4, &value3);
    int64_t hash_value3 = SPerl_HASH_FUNC_calc_hash(sperl, "key1", 4);
    int64_t index3 = hash_value3 % 101;
    
    OK(*(int64_t*)hash->table[index3]->value == 7);
    OK(hash->entries_length == 2);
    
    // Search
    int64_t value1_1 = *(int64_t*)SPerl_HASH_search(sperl, hash, "key1", 4);
    OK(value1_1 == 7);
    int64_t value2_1 = *(int64_t*)SPerl_HASH_search(sperl, hash, "key2", 4);
    OK(value2_1 == 5);
    void* value3_1 = SPerl_HASH_search(sperl, hash, "none", 4);
    OK(!value3_1);
  }
  
  /*
  // Hash - insert link
  {
    SPerl_HASH* hash = SPerl_HASH_new(sperl, 1);
    int64_t value1 = 3;
    SPerl_HASH_insert_norehash(sperl, hash, "key1", 4, &value1);
    
    OK(*(int64_t*)hash->table[0]->value == 3);
    OK(hash->entries_length == 1);

    int64_t value2 = 5;
    SPerl_HASH_insert_norehash(sperl, hash, "key2", 4, &value2);
    
    OK(*(int64_t*)hash->table[0]->next->value == 5);
    OK(hash->entries_length == 1);
    
    int64_t value3 = 7;
    SPerl_HASH_insert_norehash(sperl, hash, "key3", 4, &value3);
    
    OK(*(int64_t*)hash->table[0]->next->next->value == 7);
    OK(hash->entries_length == 1);
    
    // Replace key2
    int64_t value4 = 11;
    SPerl_HASH_insert_norehash(sperl, hash, "key2", 4, &value4);
    
    OK(*(int64_t*)hash->table[0]->next->value == 11);
    OK(hash->entries_length == 1);
    
    // Replace key3
    int64_t value5 = 13;
    SPerl_HASH_insert_norehash(sperl, hash, "key3", 4, &value5);
    
    OK(*(int64_t*)hash->table[0]->next->next->value == 13);
    OK(hash->entries_length == 1);

    // Search
    int64_t value1_1 = *(int64_t*)SPerl_HASH_search(sperl, hash, "key1", 4);
    OK(value1_1 == 3);
    int64_t value2_1 = *(int64_t*)SPerl_HASH_search(sperl, hash, "key2", 4);
    OK(value2_1 == 11);
    int64_t value3_1 = *(int64_t*)SPerl_HASH_search(sperl, hash, "key3", 4);
    OK(value3_1 == 13);
    
    // free
    SPerl_HASH_free(sperl, hash);
  }
  */
  
  return 0;
}
