#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUCKET_SIZE 4

struct cell{
  char *key;
  char *data;
  struct cell *next;
};
typedef struct cell cell_t;

void
hash_init(cell_t **table)
{
  int cnt = 0;

  for(cnt = 0; cnt < BUCKET_SIZE; cnt++){
    table[cnt] = NULL;
  }
  return;
}

static void
cell_free(cell_t *cell)
{
  if(cell->key  != NULL) free(cell->key);
  if(cell->data != NULL) free(cell->data);
  free(cell);
  return;
}

void
hash_free(cell_t **table)
{
  int cnt = 0;
  cell_t *temp = NULL;
  cell_t *swap = NULL;

  for(cnt = 0; cnt < BUCKET_SIZE; cnt++){
    temp = table[cnt];
    while(temp != NULL){
      swap = temp->next;
      cell_free(temp);
      temp = swap;
    }
  }
  hash_init(table);
  return;
}

static int
get_hash_value(char *key)
{
  int hashval = 0;

  while(*key){
    hashval += *key++;
  }
  return(hashval % BUCKET_SIZE);
}

char *
hash_search(cell_t **table, char *key)
{
  int hashval = get_hash_value(key);
  cell_t *hp  = table[hashval];

  for(; hp != NULL; hp = hp->next){
    if(strcmp(key, hp->key) == 0) return(hp->data);
  }

  return(NULL);
}

static int
strcpy_alloc(char **dest, char *src)
{
  int length = 0;

  length = strlen(src);
  if(length <= 0){
    fprintf(stderr, "ERROR: Invalid parameter(%d line)\n", __LINE__);
    return(-1);
  }

  *dest = (char *)malloc(length);
  if(*dest == NULL){
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return(-1);
  }

  if(strncpy(*dest, src, length) == NULL){
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return(-1);
  }

  return(0);
}

int
hash_insert(cell_t **table, char *key, char *data)
{
  cell_t *p = NULL;
  int hashval = 0;

  if(hash_search(table, key) != NULL){
    fprintf(stderr, "key[%s] is already exist in hash table.\n", key);
    return(-1);
  }

  p = (cell_t *)malloc(sizeof(cell_t));
  if(p == NULL){
    fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
    return(-1);
  }

  if(strcpy_alloc(&(p->key), key) != 0) return(-1);
  if(strcpy_alloc(&(p->data), data) != 0) return(-1);

  hashval = get_hash_value(key);
  p->next = table[hashval];
  table[hashval] = p;

  return( 0 );
}

int
hash_delete(cell_t **table, char *key)
{
  cell_t *target = NULL;
  cell_t *chain  = NULL;
  int hashval = get_hash_value(key);

  target = table[hashval];
  if(target == NULL){
    fprintf(stderr, "target[%s] is not exist in hash table.\n", key);
    return(-1);
  }
  chain = target->next;

  if(strcmp(key, target->key) == 0){
    table[hashval] = chain;
    cell_free(target);
    return(0);
  }

  while(target != NULL){
    if(strcmp(key, target->key) == 0){
      chain->next = target->next;
      cell_free(target);
      return( 0 );
    }
    chain  = target;
    target = target->next;
  }

  return(-1);
}

static void
hash_print_table(cell_t **table)
{
  int cnt = 0;
  cell_t *chain = NULL;

  for(cnt = 0; cnt < BUCKET_SIZE; cnt++){
    if(table[cnt] != NULL){
      printf("table[%d]:{", cnt);
      chain = table[cnt];
      for(; chain != NULL; chain = chain->next){
        printf("{%s:", (chain->key));
        printf("%s}", (chain->data));
      }
      printf("}\n");
    }
  }
}

/*

int
main(void)
{
  cell_t *table[BUCKET_SIZE];

  hash_init(table);

  hash_insert(table, "one",   "val1");
  hash_insert(table, "two",   "val2");
  hash_insert(table, "three", "val3");
  hash_insert(table, "four",  "val4");
  hash_insert(table, "five",  "val5");
  hash_insert(table, "seven", "val7");
  hash_print_table(table);

  printf("key:two=val:%s\n", hash_search(table, "two"));

  hash_delete(table, "two");
  hash_print_table(table);

  hash_free(table);

  return(0);
}
*/
