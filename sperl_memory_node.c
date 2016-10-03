#include "malloc.h"
#include "sperl_memory_node.h"

SPerl_MEMORY_NODE* SPerl_MEMORY_NODE_new() {
  return (SPerl_MEMORY_NODE*)calloc(1, sizeof(SPerl_MEMORY_NODE));
}

void SPerl_MEMORY_NODE_free (SPerl_MEMORY_NODE* memory_node) {
  if (memory_node->data) {
    free(memory_node->data);
  }
  free(memory_node);
}
