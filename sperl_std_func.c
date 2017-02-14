#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "sperl_std_func.h"
#include "sperl_frame.h"
#include "sperl_vm.h"

void SPerl_STD_FUNC_printb(SPerl_VM* vm) {
  
  int8_t value = *(int8_t*)&vm->frame->vars[0];
  
  printf("TEST: %" PRId8 "\n", value);
}

void SPerl_STD_FUNC_prints(SPerl_VM* vm) {
  
  int16_t value = *(int16_t*)&vm->frame->vars[0];
  
  printf("TEST: %" PRId16 "\n", value);
}

void SPerl_STD_FUNC_printi(SPerl_VM* vm) {
  
  int32_t value = *(int32_t*)&vm->frame->vars[0];
  
  printf("TEST: %" PRId32 "\n", value);
}

void SPerl_STD_FUNC_printl(SPerl_VM* vm) {
  
  int64_t value = vm->frame->vars[0];
  
  printf("TEST: %" PRId64 "\n", value);
}

void SPerl_STD_FUNC_printf(SPerl_VM* vm) {
  
  float value = *(float*)&vm->frame->vars[0];
  
  printf("TEST: %f\n", value);
}

void SPerl_STD_FUNC_printd(SPerl_VM* vm) {
  
  double value = *(double*)&vm->frame->vars[0];
  
  printf("TEST: %f\n", value);
}
