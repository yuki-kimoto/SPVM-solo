#include <stdio.h>
#include <string.h>

#include "sperl_descripter.h"

SPerl_char SPerl_DESCRIPTER_get_flag(SPerl_char* desc_str) {
  if (strcmp(desc_str, "const") == 0) {
    return SPerl_DESCRIPTER_CONST;
  }
  else if (strcmp(desc_str, "static") == 0) {
    return SPerl_DESCRIPTER_STATIC;
  }
  else if (strcmp(desc_str, "interface") == 0) {
    return SPerl_DESCRIPTER_INTERFACE;
  }
  else if (strcmp(desc_str, "value") == 0) {
    return SPerl_DESCRIPTER_VALUE;
  }
  else {
    return 0;
  }
}

void SPerl_DESCRIPTER_to_str(SPerl_char* str, SPerl_char flags) {
  
  sprintf(
    (char *)str,
    "%s %s %s %s",
    (flags & SPerl_DESCRIPTER_CONST) ? "const" : "",
    (flags & SPerl_DESCRIPTER_STATIC) ? "static" : "",
    (flags & SPerl_DESCRIPTER_INTERFACE) ? "interface" : "",
    (flags & SPerl_DESCRIPTER_VALUE) ? "value" : ""
  );
}

