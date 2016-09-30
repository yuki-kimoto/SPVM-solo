#include <stdlib.h>

#include "sperl_field_info.h"

SPerl_FIELD_INFO* SPerl_FIELD_INFO_new() {
  return (SPerl_FIELD_INFO*)calloc(1, sizeof(SPerl_FIELD_INFO));
}
