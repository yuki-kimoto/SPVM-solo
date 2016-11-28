#include <string.h>
#include <stdlib.h>

#include "sperl_vmcodes.h"
#include "sperl_vmcode.h"

SPerl_VMCODES* SPerl_VMCODES_new() {
  
  SPerl_VMCODES* vmcodes = malloc(sizeof(SPerl_VMCODES));
  vmcodes->capacity = 64;
  vmcodes->length = 0;
  
  SPerl_VMCODE* values = calloc(vmcodes->capacity, sizeof(SPerl_VMCODE));
  vmcodes->values = values;
  
  return vmcodes;
}

void SPerl_VMCODES_push(SPerl_VMCODES* vmcodes, SPerl_VMCODE value) {
  SPerl_int length = vmcodes->length;
  SPerl_int capacity = vmcodes->capacity;
  
  if (length >= capacity) {
    SPerl_int new_capacity = capacity * 2;
    vmcodes->values = realloc(vmcodes->values, new_capacity * sizeof(SPerl_VMCODE));
    memset(vmcodes->values + capacity, 0, (new_capacity - capacity) * sizeof(SPerl_VMCODE));
    vmcodes->capacity = new_capacity;
  }
  
  vmcodes->values[length] = value;
  vmcodes->length++;
}

void SPerl_VMCODES_free(SPerl_VMCODES* vmcodes) {
  free(vmcodes->values);
  free(vmcodes);
}
