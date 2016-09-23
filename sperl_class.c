#include <string.h>
#include <stdio.h>

#include "sperl_type.h"
#include "sperl_class.h"
#include "sperl_collection.h"

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new() {
  SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)malloc(sizeof(SPerl_METHOD_INFO));
  memset(method_info, 0, sizeof(SPerl_METHOD_INFO));
  
  return method_info;
}

void SPerl_dump_method_infos(SPerl_ARRAY* method_infos) {
  SPerl_long i;
  
  if (!method_infos) {
    return;
  }
  
  for (i = 0; i < method_infos->length; i++) {
    SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)SPerl_ARRAY_fetch(method_infos, i);
    if (method_info) {
      printf(
        "[%d]name => %s, argument_count => %d, return_type => %s, op_block => %x\n",
        i,
        method_info->name,
        method_info->argument_count,
        method_info->return_type,
        method_info->op_block
      );
    }
    else {
      printf("None\n");
    }
  }
}

SPerl_char SPerl_get_desc_flag(SPerl_char* desc_str) {
  if (strcmp(desc_str, "const") == 0) {
    return SPerl_DESC_CONST;
  }
  else if (strcmp(desc_str, "static") == 0) {
    return SPerl_DESC_STATIC;
  }
  else {
    return 0;
  }
}
