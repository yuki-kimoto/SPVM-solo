#include <stdlib.h>
#include <stdio.h>

#include "sperl_class_info.h"

SPerl_CLASS_INFO* SPerl_CLASS_INFO_new() {
  SPerl_CLASS_INFO* class_info = (SPerl_CLASS_INFO*)calloc(1, sizeof(SPerl_CLASS_INFO));
  
  
  
  return class_info;
}
