#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "spvm.h"
#include "spvm_extention.h"
#include "spvm_runtime.h"
#include "spvm_sv.h"

void SPVM_EXTENTION_std__test_call1(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int32_t value =  SPVM_RUNTIME_get_var_int(spvm, runtime, 0);
  
  int32_t value2 = value * 2;
  
  // Call subroutine
  SPVM_RUNTIME* new_runtime = SPVM_RUNTIME_new(spvm);
  SPVM_RUNTIME_push_var_int(spvm, new_runtime, value2);
  SPVM_RUNTIME_call_sub(spvm, new_runtime, "std::test_call2");
  int32_t value3 = SPVM_RUNTIME_pop_return_value_int(spvm, new_runtime);
  SPVM_RUNTIME_free(spvm, new_runtime);
  
  SPVM_RUNTIME_push_return_value_int(spvm, runtime, value3);
}

void SPVM_EXTENTION_std__test_call2(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int32_t value =  SPVM_RUNTIME_get_var_int(spvm, runtime, 0);
  
  int32_t value2 = value * 3;
  
  SPVM_RUNTIME_push_return_value_int(spvm, runtime, value2);
}

void SPVM_EXTENTION_std__sum_int(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  void* int_array = SPVM_RUNTIME_get_var_address(spvm, runtime, 0);
  
  int32_t length = SPVM_RUNTIME_get_array_length(spvm, runtime, int_array);
  
  int32_t* int_array_data = SPVM_RUNTIME_get_array_int_values(spvm, runtime, int_array);
  
  int64_t total = 0;
  for (int32_t i = 0; i < length; i++) {
    total += int_array_data[i];
  }
  
  SPVM_RUNTIME_push_return_value_int(spvm, runtime, total);
}

void SPVM_EXTENTION_std__println(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  void* string = SPVM_RUNTIME_get_var_address(spvm, runtime, 0);
  
  SPVM_SV* sv = SPVM_RUNTIME_get_string_sv(spvm, runtime, string);
  
  char* buffer = sv->buffer;
  int32_t length = sv->length;
  
  for (int32_t i = 0; i < length; i++) {
    putchar((int)buffer[i]);
  }
  
  printf("\n");
}

void SPVM_EXTENTION_std__println_byte(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int8_t value = SPVM_RUNTIME_get_var_byte(spvm, runtime, 0);
  
  printf("%" PRId8 "\n", value);
}

void SPVM_EXTENTION_std__println_short(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int16_t value = SPVM_RUNTIME_get_var_short(spvm, runtime, 0);
  
  printf("%" PRId16 "\n", value);
}

void SPVM_EXTENTION_std__println_int(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int32_t value = SPVM_RUNTIME_get_var_int(spvm, runtime, 0);
  
  printf("%" PRId32 "\n", value);
}

void SPVM_EXTENTION_std__println_long(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int64_t value = SPVM_RUNTIME_get_var_long(spvm, runtime, 0);
  
  printf("%" PRId64 "\n", value);
}

void SPVM_EXTENTION_std__println_float(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  float value = SPVM_RUNTIME_get_var_float(spvm, runtime, 0);
  
  printf("%f\n", value);
}

void SPVM_EXTENTION_std__println_double(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  double value = SPVM_RUNTIME_get_var_double(spvm, runtime, 0);
  
  printf("%f\n", value);
}

void SPVM_EXTENTION_std__print(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  void* string = SPVM_RUNTIME_get_var_address(spvm, runtime, 0);
  
  SPVM_SV* sv = SPVM_RUNTIME_get_string_sv(spvm, runtime, string);
  
  char* buffer = sv->buffer;
  int32_t length = sv->length;
  
  for (int32_t i = 0; i < length; i++) {
    putchar((int)buffer[i]);
  }
  
  printf("\n");
}

void SPVM_EXTENTION_std__print_byte(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int8_t value = SPVM_RUNTIME_get_var_byte(spvm, runtime, 0);
  
  printf("%" PRId8, value);
}

void SPVM_EXTENTION_std__print_short(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int16_t value = SPVM_RUNTIME_get_var_short(spvm, runtime, 0);
  
  printf("%" PRId16, value);
}

void SPVM_EXTENTION_std__print_int(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int32_t value = SPVM_RUNTIME_get_var_int(spvm, runtime, 0);
  
  printf("%" PRId32, value);
}

void SPVM_EXTENTION_std__print_long(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int64_t value = SPVM_RUNTIME_get_var_long(spvm, runtime, 0);
  
  printf("%" PRId64, value);
}

void SPVM_EXTENTION_std__print_float(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  float value = SPVM_RUNTIME_get_var_float(spvm, runtime, 0);
  
  printf("%f", value);
}

void SPVM_EXTENTION_std__print_double(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  double value = SPVM_RUNTIME_get_var_double(spvm, runtime, 0);
  
  printf("%f", value);
}
