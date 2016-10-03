#ifndef SPERL_MEMORY_NODE
#define SPERL_MEMORY_NODE

#include "sperl_type.h"

struct SPerl_memory_node {
  SPerl_char* data;
  SPerl_MEMORY_NODE* next;
};

SPerl_MEMORY_NODE* SPerl_MEMORY_NODE_new();
void SPerl_MEMORY_NODE_free (SPerl_MEMORY_NODE* memory_node);

#endif
