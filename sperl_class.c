#include <string.h>

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
      printf("%s\n", method_info->name);
    }
    else {
      printf("None\n");
    }
  }
}
