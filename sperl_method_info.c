#include <string.h>
#include <stdio.h>

#include "sperl_type.h"
#include "sperl_method_info.h"
#include "sperl_array.h"
#include "sperl_argument_info.h"

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new() {
  SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)malloc(sizeof(SPerl_METHOD_INFO));
  memset(method_info, 0, sizeof(SPerl_METHOD_INFO));
  
  method_info->argument_infos = (SPerl_ARGUMENT_INFO*)SPerl_ARRAY_new(0);
  
  return method_info;
}

void SPerl_METHOD_INFO_dump_method_infos(SPerl_ARRAY* method_infos) {
  SPerl_int i;
  
  if (!method_infos) {
    return;
  }
  
  for (i = 0; i < method_infos->length; i++) {
    SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)SPerl_ARRAY_fetch(method_infos, i);
    if (method_info) {
      SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
      SPerl_DESCRIPTER_to_str(desc_str, method_info->desc_flags);
      
      warn("aaaaaaaaaa %d", method_info->desc_flags);
      
      printf("[%d]\n", i);
      printf("  name => \"%s\"\n", method_info->name);
      printf("  argument_count => %d\n", method_info->argument_count);
      printf("  desc_flags => \"%s\"\n", desc_str);
      printf("  treturn_type => \"%s\"\n", method_info->return_type);
      printf("  op_block => %x\n", method_info->op_block);
      
      free(desc_str);
    }
    else {
      printf("None\n");
    }
  }
}
